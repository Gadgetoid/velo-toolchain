import argparse
import glob
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
UNWRAPPED = {"pshpack1.h", "pshpack2.h", "pshpack4.h", "pshpack8.h", "poppack.h", "README.w32api", "VENDOR.md"}
WINDOWS_EXTRAS = ["stdint.h", "mmsystem.h", "shellapi.h", "wchar.h", "stdlib.h", "string.h", "windbase.h"]
WINDOWS_VERSION_EXTRAS = [("VELO_CE >= 2", "tchar.h")]
OWN_HEADERS = ["wchar.h", "stdlib.h", "string.h", "windbase.h", "tchar.h"]
MACRO_HEADERS = {"windowsx.h"}
VERSIONS = {1: "1.0", 2: "2.0"}
ENTRY_POINTS = {"WinMain", "wWinMain", "DllMain", "DllEntryPoint"}


def vendored():
    return sorted(name for name in os.listdir(os.path.join(ROOT, "include", "w32api")) if name not in UNWRAPPED)


def write_wrappers():
    for header in vendored():
        guard_name = "VELO_WRAPPED_%s" % re.sub(r"\W", "_", header).upper()
        if header == "windows.h":
            includes = ["#include <%s>" % name for name in WINDOWS_EXTRAS]
            includes += ["#if %s\n#include <%s>\n#endif" % (condition, name) for condition, name in WINDOWS_VERSION_EXTRAS]
            extra = "\n#ifndef RC_INVOKED\n%s\n#endif\n#include <velo/extras.h>" % "\n".join(includes)
        elif os.path.exists(os.path.join(ROOT, "include", "velo", "extras-" + header)):
            extra = "\n#include <velo/extras-%s>" % header
        else:
            extra = ""
        windows = "" if header == "windows.h" else "#include <windows.h>\n"
        text = ("#ifndef %s\n#define %s\n\n#ifdef VELO_INSIDE\n#include_next <%s>%s\n#else\n%s#define VELO_INSIDE\n"
                "#include <velo/begin.h>\n#include_next <%s>%s\n#include <velo/end.h>\n#undef VELO_INSIDE\n#endif\n\n#endif\n") % (
            guard_name, guard_name, header, extra, windows, header, extra)
        with open(os.path.join(ROOT, "include", header), "w") as wrapper:
            wrapper.write(text)


def declarations(clang, version):
    source = "#include <windows.h>\n#include <winsock.h>\n#include <tchar.h>\n" + "".join("#include <%s>\n" % header for header in vendored())
    command = [clang, "--target=mipsel-unknown-none-elf", "-march=mips1", "-msoft-float", "-fshort-wchar", "-ffreestanding",
               "-Wno-experimental-option", "-fsyntax-only", "-Xclang", "-ast-dump=json", "-D_WIN32_WCE=%d" % (version * 100),
               "-DVELO_ALL_DECLARATIONS", "-I", os.path.join(ROOT, "include"), "-I", os.path.join(ROOT, "include", "w32api"), "-x", "c", "-"]
    tree = json.loads(subprocess.run(command, input=source, capture_output=True, text=True, check=True).stdout)
    macro_command = [argument for argument in command if argument not in ("-fsyntax-only", "-Xclang", "-ast-dump=json")] + ["-E", "-dM"]
    macros = set(re.findall(r"^#define (\w+)", subprocess.run(macro_command, input=source, capture_output=True, text=True, check=True).stdout, re.M))
    found = set()
    for node in tree["inner"]:
        if node.get("kind") != "FunctionDecl" or node.get("isImplicit") or node.get("storageClass") == "static":
            continue
        if any(child.get("kind") == "CompoundStmt" for child in node.get("inner", [])):
            continue
        if node["name"] not in macros:
            found.add(node["name"])
    return found


def header_paths():
    paths = sorted(path for path in glob.glob(os.path.join(ROOT, "include", "w32api", "*.h")) if os.path.basename(path) not in MACRO_HEADERS)
    paths += [os.path.join(ROOT, "include", name) for name in OWN_HEADERS]
    paths += sorted(glob.glob(os.path.join(ROOT, "include", "velo", "extras*.h")))
    return paths


def declaring_headers():
    texts = {}
    for path in header_paths():
        texts[path] = open(path, errors="replace").read()

    def find(function):
        pattern = re.compile(r"\b%s\s*\)?\s*\(" % re.escape(function))
        for header, text in texts.items():
            if pattern.search(text):
                return header
        raise LookupError(function)
    return find


def runtime_functions():
    names = set()
    for path in glob.glob(os.path.join(ROOT, "runtime", "*.c")):
        names.update(re.findall(r"^[A-Za-z_][\w \t*]*?\b(\w+)\s*\([^;]*\)\s*\{", open(path).read(), re.M))
    return names


def exports(version):
    names = set(runtime_functions())
    for path in glob.glob(os.path.join(ROOT, "exports", "ce%d" % version, "*.txt")):
        names.update(line.strip() for line in open(path) if line.strip())
    return names


def guard(header):
    text = open(header).read()
    match = re.search(r"#ifndef\s+(\w+)\s*\n\s*#define\s+\1\b", text)
    return match.group(1)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Write wrappers for the vendored w32api headers, and include/velo/unavailable.h from the ROM export lists")
    parser.add_argument("--clang", default="clang")
    parser.add_argument("--report", help="write functions the ROMs export that the headers don't declare to this file")
    arguments = parser.parse_args()
    write_wrappers()
    find_header = declaring_headers()
    lines = ["#if !defined(VELO_CE)", "#error \"include windows.h first\"", "#endif", ""]
    alias_lines = lines + ["#ifdef __cplusplus", "extern \"C\" {", "#endif", ""]
    report = []
    for version, name in VERSIONS.items():
        declared = declarations(arguments.clang, version)
        exported = exports(version)
        aliases = sorted(function[:-1] for function in declared
                         if function.endswith("W") and function not in exported and function[:-1] in exported and function[:-1] not in declared)
        alias_lines.append("#if VELO_CE == %d" % version)
        aliases_by_guard = {}
        for function in aliases:
            aliases_by_guard.setdefault(guard(find_header(function + "W")), []).append(function)
        for header_guard in sorted(aliases_by_guard):
            alias_lines.append("#ifdef %s" % header_guard)
            for function in aliases_by_guard[header_guard]:
                alias_lines += ["#undef %s" % function, "extern __typeof__(%sW) %s;" % (function, function), "#undef %sW" % function,
                                "#define %sW %s" % (function, function)]
            alias_lines.append("#endif")
        alias_lines += ["#endif", ""]
        missing = sorted(set(declared) - exported - ENTRY_POINTS - {function + "W" for function in aliases})
        by_guard = {}
        for function in missing:
            by_guard.setdefault(guard(find_header(function)), []).append(function)
        lines.append("#if VELO_CE == %d" % version)
        for header_guard in sorted(by_guard):
            lines.append("#ifdef %s" % header_guard)
            for function in by_guard[header_guard]:
                lines.append('extern __typeof__(%s) %s __attribute__((unavailable("not in Windows CE %s")));' % (function, function, name))
            lines.append("#endif")
        lines.append("#endif")
        lines.append("")
        undeclared = sorted(exported - set(declared))
        report.append("CE %s: %d declared, %d exported, %d declared but not exported, %d exported but not declared" %
                      (name, len(declared), len(exported), len(missing), len(undeclared)))
        report.extend("  %s" % function for function in undeclared)
    with open(os.path.join(ROOT, "include", "velo", "unavailable.h"), "w") as output:
        output.write("\n".join(lines))
    alias_lines += ["#ifdef __cplusplus", "}", "#endif", ""]
    with open(os.path.join(ROOT, "include", "velo", "aliases.h"), "w") as output:
        output.write("\n".join(alias_lines))
    if arguments.report:
        with open(arguments.report, "w") as output:
            output.write("\n".join(report) + "\n")
    print("\n".join(line for line in report if not line.startswith("  ")))
