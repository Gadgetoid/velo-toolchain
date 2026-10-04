import glob
import html
import os
import re
import sys
import textwrap

from highlight import highlight

SOURCE = os.path.dirname(os.path.abspath(__file__))
OUTPUT = os.path.dirname(SOURCE)
ROOT = os.path.dirname(os.path.dirname(OUTPUT))

CHAPTERS = [
    ("index.html", None, "Cover", "The Velo CE Primer"),
    ("1-velo.html", 1, "Meet the Velo", "Meet the Velo"),
    ("2-tools.html", 2, "Tools", "Toolchain and Emulator"),
    ("3-win32.html", 3, "Win32", "Win32 in a Hurry"),
    ("4-layout.html", 4, "Layout", "Screen Layout Cookbook"),
    ("5-system.html", 5, "System", "Strings, Memory, Files and the Registry"),
    ("6-network.html", 6, "Network", "Networking and Threads"),
    ("7-bluesky.html", 7, "Bluesky", "Project: A Bluesky Client"),
    ("8-appstore.html", 8, "App Store", "Project: An App Store"),
    ("9-reference.html", 9, "Reference", "Reference"),
]


def read_lines(path):
    full_path = os.path.normpath(os.path.join(ROOT, path))
    if not os.path.exists(full_path):
        sys.exit("%s not found: listings from ../velo-bluesky need a velo-bluesky checkout beside this one" % full_path)
    with open(full_path) as file:
        return file.read().split("\n")


def extract_function(path, name):
    lines = read_lines(path)
    pattern = re.compile(r"^[A-Za-z].*\b%s\(" % re.escape(name))
    for start, line in enumerate(lines):
        if pattern.match(line) and line.rstrip().endswith("{"):
            for end in range(start + 1, len(lines)):
                if lines[end] == "}":
                    return "\n".join(lines[start:end + 1])
    sys.exit("no function %s in %s" % (name, path))


def expand(code):
    code = code.strip("\n")
    match = re.fullmatch(r"@source (\S+)", code)
    if match:
        return "\n".join(read_lines(match.group(1))).rstrip("\n")
    match = re.fullmatch(r"@lines (\S+) (\d+)-(\d+)", code)
    if match:
        lines = read_lines(match.group(1))
        return textwrap.dedent("\n".join(lines[int(match.group(2)) - 1:int(match.group(3))]))
    match = re.fullmatch(r"@between (\S+) /(.+?)/ /(.+?)/", code)
    if match:
        lines = read_lines(match.group(1))
        start = next((index for index, line in enumerate(lines) if re.search(match.group(2), line)), None)
        if start is None:
            sys.exit("no %s in %s" % (match.group(2), match.group(1)))
        end = next(index for index in range(start + 1, len(lines)) if re.search(match.group(3), lines[index]))
        return textwrap.dedent("\n".join(lines[start:end + 1]))
    lines = code.split("\n")
    if len(lines) > 1 and all(line.startswith("@") for line in lines):
        return "\n\n".join(expand(line) for line in lines)
    match = re.fullmatch(r"@function (\S+) (\S+)", code)
    if match:
        return extract_function(match.group(1), match.group(2))
    if code.startswith("@"):
        sys.exit("unknown directive %s" % code)
    return code


def escape_code(text):
    def replace(match):
        file = re.search(r'data-file="([^"]+)"', match.group(1))
        code = expand(html.unescape(match.group(2)))
        return match.group(1) + highlight(code, file.group(1) if file else None, ROOT) + match.group(3)
    return re.sub(r"(<pre[^>]*><code>)(.*?)(</code></pre>)", replace, text, flags=re.S)


EXPORT_GROUPS = [
    ("Text", ["wsprintfW", "wcslen", "wcscpy", "wcscmp", "_wcsicmp", "lstrcmpW", "lstrcmpiW", "CharUpperW", "CharLowerW", "towlower",
              "MultiByteToWideChar", "WideCharToMultiByte", "FormatMessageW"]),
    ("Dialogs", ["DialogBoxIndirectParamW", "CreateDialogIndirectParamW", "GetDlgItem", "SetDlgItemTextW", "GetDlgItemTextW",
                 "SendDlgItemMessageW", "SetDlgItemInt", "CheckRadioButton", "GetOpenFileNameW", "GetSaveFileNameW"]),
    ("Drawing", ["CreatePen", "CreatePenIndirect", "CreateSolidBrush", "CreatePatternBrush", "CreateDIBSection", "StretchBlt",
                 "TransparentImage", "DrawFrameControl", "CreateRectRgn", "SetROP2"]),
    ("Windows and input", ["LoadCursorW", "SetCursor", "RegisterWindowMessageW", "GetClassLongW", "DrawMenuBar", "PlaySoundW",
                           "sndPlaySoundW", "MessageBeep"]),
    ("Files and system", ["GetModuleFileNameW", "GetTempPathW", "GetDiskFreeSpaceExW", "GetStoreInformation", "CopyFileW",
                          "SHCreateShortcut", "ShellExecuteEx", "GetVersionEx", "GetSystemPowerStatusEx", "QueryPerformanceCounter"]),
    ("Processes and threads", ["CreateProcessW", "OpenProcess", "GetExitCodeProcess", "CreateMutexW", "CreateEventW", "CreateThread",
                               "TerminateThread"]),
    ("Scheduling and databases", ["CeRunAppAtTime", "PegRunAppAtTime", "CeRunAppAtEvent", "CeCreateDatabase", "CeOpenDatabase",
                                  "PegCreateDatabase", "PegOpenDatabase"]),
]


def exports(version, dll=None):
    names = set()
    for path in glob.glob(os.path.join(ROOT, "exports", "ce%d" % version, "%s.txt" % (dll or "*"))):
        names.update(line.strip() for line in open(path) if line.strip())
    return names


def exports_table():
    first, second = exports(1), exports(2)
    rows = []
    for title, functions in EXPORT_GROUPS:
        rows.append('<tr><th colspan="3" scope="rowgroup">%s</th></tr>' % title)
        for function in functions:
            rows.append("<tr><td><code>%s</code></td><td>%s</td><td>%s</td></tr>"
                        % (function, "Yes" if function in first else "", "Yes" if function in second else ""))
    return "\n".join(rows)


def fill_exports(text):
    text = text.replace("<!-- @exports-table -->", exports_table())
    return re.sub(r"<!-- @export-count (\d) (\w+) -->", lambda match: str(len(exports(int(match.group(1)), match.group(2)))), text)


def navigation(current):
    items = []
    for file, number, short, _ in CHAPTERS:
        label = short if number is None else "%d. %s" % (number, short)
        marker = ' aria-current="page"' if file == current else ""
        items.append('<li><a href="%s"%s>%s</a></li>' % (file, marker, label))
    return '<nav aria-label="Chapters">\n<ol>\n%s\n</ol>\n</nav>' % "\n".join(items)


def footer(index):
    links = []
    if index > 0:
        file, number, short, _ = CHAPTERS[index - 1]
        label = short if number is None else "%d. %s" % (number, short)
        links.append('<li><a href="%s" rel="prev">&lt; %s</a></li>' % (file, label))
    if index + 1 < len(CHAPTERS):
        file, number, short, _ = CHAPTERS[index + 1]
        links.append('<li><a href="%s" rel="next">%d. %s &gt;</a></li>' % (file, number, short))
    return ("<footer>\n<ul>\n%s\n</ul>\n<p>The Velo CE Primer, for velo-toolchain. Screenshots are from velo-emu.</p>\n</footer>"
            % "\n".join(links))


def page(index):
    file, number, short, title = CHAPTERS[index]
    with open(os.path.join(SOURCE, file)) as source:
        body = escape_code(fill_exports(source.read())).strip("\n")
    main = "<main>\n%s\n</main>" % body
    if number is None:
        head_title = "The Velo CE Primer"
        top = ""
    else:
        head_title = "%s - The Velo CE Primer" % title
        top = ('<header>\n<p>The Velo CE Primer</p>\n<h1><small>Chapter %d</small>%s</h1>\n%s\n</header>\n'
               % (number, title, navigation(file)))
    return ("<!doctype html>\n<html lang=\"en\">\n<head>\n<meta charset=\"utf-8\">\n"
            "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
            "<title>%s</title>\n<link rel=\"stylesheet\" href=\"style.css\">\n</head>\n<body>\n%s%s\n%s\n</body>\n</html>\n"
            % (head_title, top, main, footer(index)))


def check(text, file):
    for character in text:
        if ord(character) > 126 and character not in "\n":
            sys.exit("%s: non-ASCII character %r" % (file, character))
    if re.search(r"[.?!]  +[A-Z]", re.sub(r"<pre.*?</pre>", "", text, flags=re.S)):
        sys.exit("%s: double space after a sentence" % file)


if __name__ == "__main__":
    for index, (file, _, _, _) in enumerate(CHAPTERS):
        text = page(index)
        check(text, file)
        with open(os.path.join(OUTPUT, file), "w") as output:
            output.write(text)
        print(file)
