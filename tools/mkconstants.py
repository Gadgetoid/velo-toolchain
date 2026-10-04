import argparse
import os
import re
import sys
import tempfile
from importlib.machinery import SourceFileLoader

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
check = SourceFileLoader("check_headers", os.path.join(ROOT, "tests", "check-headers.py")).load_module()


SDK_HEADER_HOMES = {"types.h": "windef.h", "tchar.h": "winnt.h"}


def our_guard(header):
    header = SDK_HEADER_HOMES.get(header, header)
    for path in (os.path.join(ROOT, "include", "w32api", header), os.path.join(ROOT, "include", header)):
        if os.path.exists(path):
            match = re.search(r"#ifndef\s+(\w+)\s*\n\s*#define\s+\1\b", open(path).read())
            if match:
                return match.group(1)
    return None


def literal(value):
    if value < 0:
        return "(%d)" % value
    if value > 0x7FFFFFFF:
        return "0x%XU" % value
    if value >= 0x100:
        return "0x%X" % value
    return "%d" % value


def missing_constants(clang, reference, version, work):
    sdk_include = check.sdk_folder(reference, version, work)
    headers = [header for header in check.HEADERS if os.path.exists(os.path.join(sdk_include, header))]
    our_headers = [header for header in headers if os.path.exists(os.path.join(ROOT, "include", header))]
    sdk = check.Side(clang, version * 100, [sdk_include], check.SDK_DEFINES, headers)
    ours = check.Side(clang, version * 100, [os.path.join(ROOT, "include"), os.path.join(ROOT, "include", "w32api")], ["-DVELO_NO_CONSTANTS"],
                      our_headers)
    candidates = {name: name for name in sdk.macros() if not name.startswith("_")}
    sdk_values = sdk.evaluate(candidates)
    our_macros = ours.macros()
    our_values = ours.evaluate({name: name for name in sdk_values if name in our_macros})
    where = check.defining_headers(sdk_include)
    found = {}
    for name, value in sdk_values.items():
        if name in our_values or name in our_macros or check.excluded_constant(name, where.get(name)):
            continue
        found.setdefault(where[name], {})[name] = value
    return found


C_WORDS = {"const", "volatile", "struct", "union", "enum", "unsigned", "signed", "char", "short", "int", "long", "float", "double", "void",
           "_Bool", "__attribute__", "__stdcall__", "stdcall"}


def missing_types(clang, reference, version, work):
    sdk_include = os.path.join(work, "ce%d" % version)
    headers = [header for header in check.HEADERS if os.path.exists(os.path.join(sdk_include, header))]
    our_headers = [header for header in headers if os.path.exists(os.path.join(ROOT, "include", header))]
    sdk = check.Side(clang, version * 100, [sdk_include], check.SDK_DEFINES, headers)
    ours = check.Side(clang, version * 100, [os.path.join(ROOT, "include"), os.path.join(ROOT, "include", "w32api")], ["-DVELO_NO_TYPES"],
                      our_headers)
    sdk_tree = sdk.tree()
    where = check.type_headers(sdk_tree)
    our_names = set(ours.macros())
    for node in ours.tree()["inner"]:
        if node.get("kind") == "TypedefDecl":
            our_names.add(node["name"])
        elif node.get("kind") == "RecordDecl" and node.get("name"):
            our_names.add(node["name"])
    tag_names = {}
    for node in sdk_tree["inner"]:
        if node.get("kind") == "TypedefDecl":
            match = re.match(r"^(struct|union) (\w+)$", node["type"]["qualType"])
            if match:
                tag_names.setdefault(match.group(0), []).append(node["name"])
    candidates = []
    for node in sdk_tree["inner"]:
        if node.get("kind") != "TypedefDecl" or node.get("isImplicit") or node["name"] in our_names:
            continue
        name = node["name"]
        if where.get(name) in check.EXCLUDED_HEADERS or check.EXCLUDED_TYPES.match(name):
            continue
        spelled = re.sub(r"\b(struct|union) \w+\b",
                         lambda match: next((alias for alias in tag_names.get(match.group(0), []) if alias in our_names), match.group(0)),
                         node["type"]["qualType"])
        candidates.append((name, spelled))
    found = []
    progress = True
    while progress:
        progress = False
        remaining = []
        for name, spelled in candidates:
            referenced = set(re.findall(r"[A-Za-z_]\w*", spelled)) - C_WORDS
            if referenced <= our_names and ("(" not in spelled or "(*)" in spelled):
                found.append((where.get(name), name, spelled.replace("(*)", "(*%s)" % name, 1) if "(*)" in spelled else spelled))
                our_names.add(name)
                progress = True
            else:
                remaining.append((name, spelled))
        candidates = remaining
    unresolved = [name for name, _ in candidates]
    return found, sorted(set(unresolved))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Write include/velo/constants.h: SDK constants the vendored headers lack")
    parser.add_argument("--reference", default=os.environ.get("VELO_REFERENCE"))
    parser.add_argument("--clang", default=os.environ.get("CLANG", "clang"))
    arguments = parser.parse_args()
    if not arguments.reference:
        sys.exit("mkconstants: set VELO_REFERENCE or pass --reference")
    lines = []
    skipped = []
    with tempfile.TemporaryDirectory() as work:
        for version in (1, 2):
            lines.append("#if VELO_CE == %d" % version)
            for header, constants in sorted(missing_constants(arguments.clang, os.path.expanduser(arguments.reference), version, work).items()):
                guard = our_guard(header)
                if not guard:
                    skipped.append("CE %d.0 %s: %d" % (version, header, len(constants)))
                    continue
                lines.append("#ifdef %s" % guard)
                for name, value in sorted(constants.items()):
                    lines += ["#ifndef %s" % name, "#define %s %s" % (name, literal(value)), "#endif"]
                lines.append("#endif")
            lines += ["#endif", ""]
        type_lines = []
        for version in (1, 2):
            type_lines.append("#if VELO_CE == %d" % version)
            found, unresolved = missing_types(arguments.clang, os.path.expanduser(arguments.reference), version, work)
            for header, name, spelled in found:
                guard = our_guard(header) if header else None
                if not guard:
                    skipped.append("CE %d.0 type %s in %s" % (version, name, header))
                    continue
                declaration = "typedef %s;" % spelled if "(*%s)" % name in spelled else "typedef %s %s;" % (spelled, name)
                type_lines += ["#ifdef %s" % guard, declaration, "#endif"]
            type_lines += ["#endif", ""]
            print("mkconstants: CE %d.0 types needing definitions: %s" % (version, " ".join(unresolved)))
    with open(os.path.join(ROOT, "include", "velo", "constants.h"), "w") as output:
        output.write("\n".join(lines))
    with open(os.path.join(ROOT, "include", "velo", "types.h"), "w") as output:
        output.write("\n".join(type_lines))
    print("mkconstants: %d constants" % sum(1 for line in lines if line.startswith("#define")))
    for line in skipped:
        print("mkconstants: no header of ours for %s" % line)
