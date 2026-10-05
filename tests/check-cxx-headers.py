import argparse
import glob
import json
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
COMMON_FLAGS = ["-fshort-wchar", "-ffreestanding", "-std=c++20", "-fno-exceptions", "-fno-rtti", "-x", "c++"]
ARCHITECTURES = {
    "mips": {"target": ["--target=mipsel-unknown-none-elf", "-march=mips1", "-msoft-float", "-Wno-experimental-option"], "defines": []},
    "sh3": {"target": ["--target=sh3el-unknown-none-wince"], "defines": ["-DSHx", "-DSH3", "-D_SH3_"]},
}
WIN32_WCE = {1: 100, 101: 101, 2: 200}
VERSION_NAMES = {1: "1.0", 101: "1.01", 2: "2.0"}


def vendored_headers():
    names = sorted(os.path.basename(path) for path in glob.glob(os.path.join(ROOT, "include", "w32api", "*.h")))
    return [name for name in names if not name.startswith(("pshpack", "poppack"))]


def sdk_headers():
    return sorted(os.path.relpath(path, os.path.join(ROOT, "include")) for path in glob.glob(os.path.join(ROOT, "include", "velo", "cxx", "*.h")))


def compile_source(clang, version, arch, target_arch, source, extra):
    command = [clang, *ARCHITECTURES[target_arch]["target"], *COMMON_FLAGS, *ARCHITECTURES[arch]["defines"], "-D_WIN32_WCE=%d" % WIN32_WCE[version],
               "-I", os.path.join(ROOT, "include"), "-I", os.path.join(ROOT, "include", "w32api"), *extra, "-"]
    return subprocess.run(command, input=source, capture_output=True, text=True)


def cplusplus_linkage(tree):
    in_c = set()
    outside = set()

    def walk(node, language):
        for child in node.get("inner", []):
            kind = child.get("kind")
            if kind == "LinkageSpecDecl":
                walk(child, child.get("language"))
            elif kind == "FunctionDecl" and not child.get("isImplicit") and child.get("storageClass") != "static":
                if any(grandchild.get("kind") == "CompoundStmt" for grandchild in child.get("inner", [])):
                    continue
                (in_c if language == "C" else outside).add(child["name"])
    walk(tree, None)
    return sorted(outside - in_c)


def check(clang, version, arch, target_arch):
    problems = []
    source = "".join("#include <%s>\n" % name for name in ["windows.h", "winsock.h", "tchar.h", *vendored_headers()])
    result = compile_source(clang, version, arch, target_arch, source, ["-fsyntax-only", "-w", "-Xclang", "-ast-dump=json"])
    if result.returncode:
        problems.append("Windows headers don't compile as C++:\n" + result.stderr)
    else:
        for name in cplusplus_linkage(json.loads(result.stdout)):
            problems.append("%s has C++ linkage" % name)
    for header in sdk_headers():
        result = compile_source(clang, version, arch, target_arch, "#include <windows.h>\n#include <%s>\n" % header, ["-fsyntax-only", "-Wall", "-Wextra", "-Werror"])
        if result.returncode:
            problems.append("%s doesn't compile:\n%s" % (header, result.stderr))
    with open(os.path.join(ROOT, "tests", "cxx-sdk.cpp")) as source:
        result = compile_source(clang, version, arch, target_arch, source.read(), ["-fsyntax-only", "-Wall", "-Wextra", "-Werror", "-Wno-unused-parameter"])
    if result.returncode:
        problems.append("tests/cxx-sdk.cpp doesn't compile:\n%s" % result.stderr)
    return problems


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Check the headers declare C functions with C linkage in C++, and the C++ SDK headers compile")
    parser.add_argument("--clang", default=os.environ.get("CLANG", "clang"))
    parser.add_argument("--arch", choices=sorted(ARCHITECTURES), default="mips", help="architecture defines, and CE versions, to check")
    parser.add_argument("--target-arch", choices=sorted(ARCHITECTURES), help="compile for this architecture's clang target instead, "
                        "to check with a clang that lacks --arch's backend")
    arguments = parser.parse_args()
    failed = False
    label = "" if arguments.arch == "mips" else " " + arguments.arch.upper()
    for version in ([1, 101, 2] if arguments.arch == "sh3" else [1, 2]):
        problems = check(arguments.clang, version, arguments.arch, arguments.target_arch or arguments.arch)
        for problem in problems:
            print("CE %s%s: %s" % (VERSION_NAMES[version], label, problem), file=sys.stderr)
        failed = failed or bool(problems)
        print("CE %s%s: %d problems" % (VERSION_NAMES[version], label, len(problems)))
    sys.exit(1 if failed else 0)
