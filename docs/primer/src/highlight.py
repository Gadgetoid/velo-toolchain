import functools
import glob
import html
import os
import re

C_KEYWORDS = {
    "break", "case", "const", "continue", "default", "do", "else", "enum", "extern", "for", "goto", "if", "return", "sizeof",
    "static", "struct", "switch", "typedef", "union", "volatile", "while", "BEGIN", "END", "MENU", "POPUP", "MENUITEM",
    "SEPARATOR", "ACCELERATORS", "DIALOG", "STYLE", "CAPTION", "LTEXT", "EDITTEXT", "COMBOBOX", "AUTOCHECKBOX",
    "AUTORADIOBUTTON", "GROUPBOX", "DEFPUSHBUTTON", "PUSHBUTTON", "ICON", "BITMAP", "VIRTKEY", "CONTROL", "NOINVERT",
}
C_TYPES = {"void", "char", "short", "int", "long", "unsigned", "signed", "float", "double", "size_t", "uint8_t", "uint16_t",
           "uint32_t", "int16_t", "int32_t", "wchar_t", "fd_set"}
SHELL_COMMANDS = {"cmake", "make", "brew", "sudo", "apt", "export", "gdb", "mkdir", "cp", "cd", "git", "python3", "grep",
                  "pngquant", "headless"}


@functools.lru_cache(maxsize=None)
def header_types(root):
    types = set(C_TYPES)
    for path in glob.glob(os.path.join(root, "include", "**", "*.h"), recursive=True):
        text = open(path, errors="replace").read()
        types.update(re.findall(r"DECLARE_HANDLE\s*\(\s*(\w+)\s*\)", text))
        for body in re.findall(r"\btypedef\b([^;{}()]*);", text):
            types.update(re.findall(r"(\w+)\s*(?:,|$)", body.strip()))
        for names in re.findall(r"}\s*([\w\s,*]+);", text):
            types.update(re.findall(r"\w+", names))
        types.update(re.findall(r"typedef\s+[\w\s*]+\(\s*\w*\s*\*\s*(\w+)\s*\)", text))
    return frozenset(name for name in types if name in C_TYPES or name.endswith("_t") or re.search(r"[A-Z]", name))


C_PATTERN = re.compile(r"""
    (?P<comment>//[^\n]*|/\*.*?\*/)
  | (?P<preprocessor>^[ \t]*\#[ \t]*\w+)
  | (?P<string>L?"(?:\\.|[^"\\\n])*"|L?'(?:\\.|[^'\\\n])*'|(?<=\#include )<[^>\n]+>)
  | (?P<number>\b(?:0[xX][0-9A-Fa-f]+|\d+(?:\.\d+)?)[uUlL]*\b)
  | (?P<word>\b[A-Za-z_]\w*\b)
""", re.X | re.M | re.S)

SHELL_PATTERN = re.compile(r"""
    (?P<comment>(?<!\S)\#[^\n]*)
  | (?P<string>"(?:\\.|[^"\\])*"|'[^']*')
  | (?P<variable>\$\{?\w+\}?)
  | (?P<option>(?<![\w/.-])--?[A-Za-z][\w-]*(?:=)?)
  | (?P<command>(?:^|(?<=[|&;]\ )|(?<=&&\ ))[\w./-]+)
""", re.X | re.M)

CMAKE_PATTERN = re.compile(r"""
    (?P<comment>\#[^\n]*)
  | (?P<string>"(?:\\.|[^"\\])*")
  | (?P<variable>\$\{\w+\}|\$<[^>]+>)
  | (?P<command>\b\w+(?=\())
  | (?P<keyword>\b[A-Z][A-Z0-9_]{2,}\b)
""", re.X)

JSON_PATTERN = re.compile(r"""
    (?P<key>"(?:\\.|[^"\\])*"(?=\s*:))
  | (?P<string>"(?:\\.|[^"\\])*")
  | (?P<number>-?\b\d+(?:\.\d+)?\b)
  | (?P<keyword>\b(?:true|false|null)\b)
""", re.X)

PYTHON_PATTERN = re.compile(r"""
    (?P<comment>\#[^\n]*)
  | (?P<string>"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*')
  | (?P<keyword>\b(?:import|from|if|else|elif|for|in|def|return|not|and|or|None|True|False|with|as)\b)
  | (?P<function>\b\w+(?=\())
  | (?P<number>\b\d+\b)
""", re.X)

CLASSES = {"comment": "comment", "preprocessor": "preprocessor", "string": "string", "number": "number", "keyword": "keyword",
           "type": "type", "constant": "constant", "function": "function", "variable": "constant", "option": "keyword",
           "command": "function", "key": "type"}


def span(kind, text):
    return '<span class="%s">%s</span>' % (CLASSES[kind], html.escape(text, quote=False))


def render(pattern, code, classify):
    output = []
    position = 0
    for match in pattern.finditer(code):
        output.append(html.escape(code[position:match.start()], quote=False))
        kind = classify(match, code)
        output.append(span(kind, match.group()) if kind else html.escape(match.group(), quote=False))
        position = match.end()
    output.append(html.escape(code[position:], quote=False))
    return "".join(output)


def c_classifier(types):
    def classify(match, code):
        kind = match.lastgroup
        if kind != "word":
            return kind
        word = match.group()
        if word in C_KEYWORDS:
            return "keyword"
        if word in types or word.endswith("_t"):
            return "type"
        if re.fullmatch(r"[A-Z][A-Z0-9_]+", word):
            return "constant"
        if re.match(r"\s*\(", code[match.end():]):
            return "function"
        return None
    return classify


def plain_classifier(match, code):
    return match.lastgroup


def language(file, code):
    if file:
        name = os.path.basename(file)
        if name.endswith((".c", ".h", ".rc")):
            return "c"
        if name == "CMakeLists.txt":
            return "cmake"
        if name.endswith(".json"):
            return "json"
        if name.endswith(".py"):
            return "python"
        return None
    first = code.lstrip().split(None, 1)[0] if code.strip() else ""
    if code.lstrip().startswith("{") and '"' in code:
        return "json"
    if first.split("/")[-1] in SHELL_COMMANDS or first.startswith(("$", "tools/")):
        return "shell"
    if re.search(r";\s*$|^#(include|define|if)|^static |\)\s*\{", code, re.M):
        return "c"
    return None


def highlight(code, file, root):
    kind = language(file, code)
    if kind == "c":
        return render(C_PATTERN, code, c_classifier(header_types(root)))
    if kind == "shell":
        return render(SHELL_PATTERN, code, plain_classifier)
    if kind == "cmake":
        return render(CMAKE_PATTERN, code, plain_classifier)
    if kind == "json":
        return render(JSON_PATTERN, code, plain_classifier)
    if kind == "python":
        return render(PYTHON_PATTERN, code, plain_classifier)
    return html.escape(code, quote=False)
