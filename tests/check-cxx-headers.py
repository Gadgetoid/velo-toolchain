import argparse
import glob
import json
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TARGET = ["--target=mipsel-unknown-none-elf", "-march=mips1", "-msoft-float", "-fshort-wchar", "-ffreestanding", "-Wno-experimental-option",
          "-std=c++20", "-fno-exceptions", "-fno-rtti", "-x", "c++"]


def vendored_headers():
    names = sorted(os.path.basename(path) for path in glob.glob(os.path.join(ROOT, "include", "w32api", "*.h")))
    return [name for name in names if not name.startswith(("pshpack", "poppack"))]


def sdk_headers():
    return sorted(os.path.relpath(path, os.path.join(ROOT, "include")) for path in glob.glob(os.path.join(ROOT, "include", "velo", "cxx", "*.h")))


def compile_source(clang, version, source, extra):
    command = [clang, *TARGET, "-D_WIN32_WCE=%d" % (version * 100), "-I", os.path.join(ROOT, "include"), "-I", os.path.join(ROOT, "include", "w32api"),
               *extra, "-"]
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


def check(clang, version):
    problems = []
    source = "".join("#include <%s>\n" % name for name in ["windows.h", "winsock.h", *vendored_headers()])
    result = compile_source(clang, version, source, ["-fsyntax-only", "-w", "-Xclang", "-ast-dump=json"])
    if result.returncode:
        problems.append("Windows headers don't compile as C++:\n" + result.stderr)
    else:
        for name in cplusplus_linkage(json.loads(result.stdout)):
            problems.append("%s has C++ linkage" % name)
    for header in sdk_headers():
        result = compile_source(clang, version, "#include <windows.h>\n#include <%s>\n" % header, ["-fsyntax-only", "-Wall", "-Wextra", "-Werror"])
        if result.returncode:
            problems.append("%s doesn't compile:\n%s" % (header, result.stderr))
    return problems


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Check the headers declare C functions with C linkage in C++, and the C++ SDK headers compile")
    parser.add_argument("--clang", default=os.environ.get("CLANG", "clang"))
    arguments = parser.parse_args()
    failed = False
    for version in (1, 2):
        problems = check(arguments.clang, version)
        for problem in problems:
            print("CE %d.0: %s" % (version, problem), file=sys.stderr)
        failed = failed or bool(problems)
        print("CE %d.0: %d problems" % (version, len(problems)))
    sys.exit(1 if failed else 0)
