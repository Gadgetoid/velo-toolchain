import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HEADERS = ["windows.h", "commctrl.h", "commdlg.h", "winsock.h", "notify.h", "tlhelp32.h", "mmreg.h", "msacm.h", "imm.h", "ras.h", "af_irda.h", "windowsx.h", "winnetwk.h", "mmsystem.h", "lmcons.h"]
TARGET = ["--target=mipsel-unknown-none-elf", "-march=mips1", "-msoft-float", "-fshort-wchar", "-ffreestanding", "-Wno-experimental-option",
          "-w", "-ferror-limit=0"]
KNOWN_DIFFERENCES = re.compile(r"^(size|offset)_(struct_)?_?(WIN32_FIND_DATAA|tagREBARBANDINFOA|REBARBANDINFOA)(_|$)")
KNOWN_SIGNATURES = re.compile(r"^(ImageList_EndDrag|waveOutGetVolume|waveOutSetVolume|Random)$")
EXCLUDED_FUNCTIONS = re.compile(r"^(Co[A-Z]\w*|Ole\w+|Stg\w+|Var[A-Z]\w*|Variant\w+|SafeArray\w+|Sys[A-Z]\w*|Disp\w+|CLSIDFromString|"
                                r"StringFrom\w+|ReadClass\w+|WriteClass\w+|\w*TypeLib\w*|CreateErrorInfo|SetErrorInfo|CreateOleAdviseHolder|BstrFromVector|"
                                r"VectorFromBstr|SystemTimeToVariantTime|VariantTimeToSystemTime|line[A-Z]\w*|Dll[A-Z]\w*|ThisIsGwes|__C_specific_handler|"
                                r"acmFilter\w+|acmFormatChoose)$")
EXCLUDED_HEADERS = {"tapi.h", "mmreg.h", "mmddk.h", "oleauto.h", "oaidl.h", "objidl.h", "objbase.h", "oleidl.h", "ole2.h", "wtypes.h", "kfuncs.h",
                    "unknwn.h", "rpc.h", "rpcdce.h", "rpcndr.h", "rpcnsip.h", "rpcnterr.h", "olectl.h", "ocidl.h", "cguid.h", "coguid.h"}
EXCLUDED_TYPES = re.compile(r"^(P?U?INT128|RNAAPP_INFO|PRNAAPP_INFO|RASPPPADDR|RasCntlEnum_t|RUNQ_t|PRUNQ_t|PTHREAD|P?EXCEPTION_ROUTINE|"
                            r"_onexit_t|l?div_t|P?SOCKHAND|LPHICON)$|^(LP)?(LINE|PHONE|HLINE|HPHONE|HCALL|P?ACMFILTER|P?ACMFORMATCHOOSE|ACMDRIVERPROC|LPACMDRIVERPROC|I[A-Z]\w*Vtbl$|I[A-Z][a-z]\w*$|LP[A-Z]*VTBL$|PFN(CANUNLOADNOW|GETCLASSOBJECT)$)")
EXCLUDED_MACROS = re.compile(r"^(\w+[a-z0-9]A|\w+API|DEBUGMSG|ERRORMSG|RETAILMSG|DBGCHK|WSAStartup|WSACleanup|BASETYPES|cdecl|NETCONS_INCLUDED|"
                             r"SOCKHAND_DEFINED|BACKUP_MSG_FILENAME|IS_DISPATCHING|IS_UNWINDING|IS_TARGET_UNWIND|isleadbyte|iswascii|MB_CUR_MAX|"
                             r"ACMHELPMSG\w+|DEFINE_GUID|DIALOGEX|LPTBSAVEPARAMS)$")
EXCLUDED_CONSTANTS = re.compile(r"^(CERT_E_|CRYPT_E_|TRUST_E_|DIGSIG_E_|SPAPI_E_)")
SDK_DEFINES = ["-fms-extensions", "-D_WIN32", "-DUNDER_CE", "-DMIPS", "-D_MIPS_=1", "-D_M_MRX000=4000", "-DUNICODE", "-D_UNICODE", "-DWIN32_LEAN_AND_MEAN",
               "-D__asm=velo_sdk_asm", "-D__export=", "-DHUGEP="]


class Side:
    def __init__(self, clang, version, include, defines, headers):
        self.clang = clang
        self.flags = TARGET + ["-D_WIN32_WCE=%d" % version] + defines + ["-I%s" % folder for folder in include]
        self.prelude = "".join("#include <%s>\n" % header for header in headers)

    def run(self, extra, body, check=False):
        return subprocess.run([self.clang] + self.flags + extra + ["-x", "c", "-"], input=self.prelude + body, capture_output=True, text=True,
                              check=check)

    def macros(self):
        found = {}
        for line in self.run(["-E", "-dM"], "", check=True).stdout.splitlines():
            match = re.match(r"#define (\w+) (.+)$", line)
            if match:
                found[match.group(1)] = match.group(2)
        return found

    def tree(self, body=""):
        return json.loads(self.run(["-fsyntax-only", "-Xclang", "-ast-dump=json"], body).stdout)

    def evaluate(self, expressions):
        lines = list(expressions.items())
        while True:
            body = "".join("long long velo_value_%s = (long long)(%s);\n" % (name, expression) for name, expression in lines)
            result = self.run(["-S", "-emit-llvm", "-o", "-"], body)
            bad = set()
            first = self.prelude.count("\n") + 1
            for match in re.finditer(r"<stdin>:(\d+):\d+: error", result.stderr):
                index = int(match.group(1)) - first
                if 0 <= index < len(lines):
                    bad.add(index)
            if not bad:
                break
            lines = [line for index, line in enumerate(lines) if index not in bad]
        values = {}
        for match in re.finditer(r"@velo_value_(\w+) = (?:dso_local )?global i64 (-?\d+)", result.stdout):
            values[match.group(1)] = int(match.group(2))
        return values


    def signatures(self, names):
        names = sorted(names)
        unusable = set()
        while True:
            body = "".join("void *velo_address_%s = (void *)&%s;\n" % (name, name) for name in names)
            result = self.run(["-S", "-emit-llvm", "-o", "-"], body)
            first = self.prelude.count("\n") + 1
            bad = set()
            for match in re.finditer(r"<stdin>:(\d+):\d+: error", result.stderr):
                index = int(match.group(1)) - first
                if 0 <= index < len(names):
                    bad.add(names[index])
            if not bad:
                break
            unusable |= bad
            names = [name for name in names if name not in bad]
        found = {}
        for match in re.finditer(r"^declare (?:dso_local )?(.*?) @(\w+)\((.*?)\)", result.stdout, re.M):
            signature = "%s (%s)" % (match.group(1), match.group(3))
            found[match.group(2)] = re.sub(r"\b(noundef|dso_local)\s*", "", signature).strip()
        return found, unusable


def exports(version):
    names = set()
    folder = os.path.join(ROOT, "exports", "ce%d" % version)
    for name in os.listdir(folder):
        names.update(line.strip() for line in open(os.path.join(folder, name)) if line.strip())
    return names


def sdk_folder(reference, version, work):
    source = os.path.join(reference, "include", "ce%d00" % version)
    folder = os.path.join(work, "ce%d" % version)
    os.makedirs(folder)
    for name in os.listdir(source):
        shutil.copyfile(os.path.join(source, name), os.path.join(folder, name.lower()))
    return folder


def macro_names(side):
    return set(re.findall(r"^#define (\w+)", side.run(["-E", "-dM"], "", check=True).stdout, re.M))


def declared_names(tree):
    found = set()
    for node in tree["inner"]:
        if node.get("name"):
            found.add(node["name"])
        if node.get("kind") == "EnumDecl":
            found.update(child["name"] for child in node.get("inner", []) if child.get("kind") == "EnumConstantDecl")
    return found


def records(tree):
    found = {}
    for node in tree["inner"]:
        if node.get("kind") == "TypedefDecl" and not node.get("isImplicit"):
            found.setdefault(node["name"], node["name"])
        if node.get("kind") == "RecordDecl" and node.get("name") and node.get("completeDefinition"):
            fields = [child["name"] for child in node.get("inner", []) if child.get("kind") == "FieldDecl" and child.get("name")]
            found["%s %s" % (node.get("tagUsed", "struct"), node["name"])] = fields
    return found


def defining_headers(folder):
    where = {}
    for name in sorted(os.listdir(folder)):
        text = open(os.path.join(folder, name), encoding="latin-1").read()
        for macro in re.findall(r"^\s*#\s*define\s+(\w+)", text, re.M):
            where.setdefault(macro, name)
    return where


def type_headers(tree):
    where = {}
    current = None
    for node in tree["inner"]:
        location = node.get("loc", {})
        for candidate in (location.get("file"), location.get("expansionLoc", {}).get("file"), location.get("spellingLoc", {}).get("file")):
            if candidate:
                current = os.path.basename(candidate)
        if node.get("kind") in ("TypedefDecl", "RecordDecl") and node.get("name"):
            key = node["name"] if node["kind"] == "TypedefDecl" else "%s %s" % (node.get("tagUsed", "struct"), node["name"])
            where.setdefault(key, current)
    return where


def excluded_constant(name, header):
    return header is None or header in EXCLUDED_HEADERS or bool(EXCLUDED_CONSTANTS.match(name))


def compare_values(label, sdk_values, our_values, sdk_names, report, excluded=frozenset()):
    missing = sorted(name for name in sdk_names if name not in our_values and name in sdk_values and name not in excluded)
    different = sorted(name for name in sdk_values if name in our_values and sdk_values[name] % 2 ** 32 != our_values[name] % 2 ** 32
                       and not KNOWN_DIFFERENCES.match(name))
    report.append("%s: %d compared, %d excluded, %d missing, %d different" % (label, len(sdk_values) - len(excluded), len(excluded), len(missing),
                                                                             len(different)))
    for name in different:
        report.append("  different %s: SDK %d, ours %d" % (name, sdk_values[name], our_values[name]))
    for name in missing:
        report.append("  missing %s" % name)
    return len(missing) + len(different)


def check(clang, reference, version, work):
    sdk_include = sdk_folder(reference, version, work)
    headers = [header for header in HEADERS if os.path.exists(os.path.join(sdk_include, header))]
    our_headers = [header for header in headers if os.path.exists(os.path.join(ROOT, "include", header))]
    sdk = Side(clang, version * 100, [sdk_include], SDK_DEFINES, headers)
    ours = Side(clang, version * 100, [os.path.join(ROOT, "include"), os.path.join(ROOT, "include", "w32api")], [], our_headers)
    report = ["CE %d.0, headers: %s" % (version, " ".join(headers))]

    sdk_macros = sdk.macros()
    our_macros = ours.macros()
    candidates = {name: name for name, value in sdk_macros.items() if not name.startswith("_") and "(" not in name}
    sdk_values = sdk.evaluate(candidates)
    our_values = ours.evaluate({name: name for name in sdk_values if name in our_macros or name in candidates})
    where = defining_headers(sdk_include)
    excluded = {name for name in sdk_values if name not in our_values and excluded_constant(name, where.get(name))}
    problems = compare_values("constants", sdk_values, our_values, sdk_values, report, excluded)

    sdk_tree = sdk.tree()
    sdk_records = records(sdk_tree)
    our_records = records(ours.tree())
    type_where = type_headers(sdk_tree)
    sizes = {}
    excluded_types = set()
    for name, fields in sdk_records.items():
        if name.startswith(("struct ", "union ")) or type_where.get(name) in EXCLUDED_HEADERS or EXCLUDED_TYPES.match(name):
            excluded_types.add(name)
            continue
        sizes["size_" + re.sub(r"\W", "_", name)] = "sizeof(%s)" % name
        if isinstance(fields, list):
            for field in fields:
                sizes["offset_%s_%s" % (re.sub(r"\W", "_", name), field)] = "__builtin_offsetof(%s, %s)" % (name, field)
    sdk_sizes = sdk.evaluate(sizes)
    our_sizes = ours.evaluate({key: expression for key, expression in sizes.items() if key in sdk_sizes})
    report.append("types: %d checked by name, %d excluded (tags and excluded areas)" % (len(sdk_records) - len(excluded_types), len(excluded_types)))
    problems += compare_values("layouts", sdk_sizes, our_sizes, sdk_sizes, report)

    exported = exports(version)
    sdk_functions = sorted(node["name"] for node in sdk.tree()["inner"] if node.get("kind") == "FunctionDecl" and node["name"] in exported)
    sdk_signatures, _ = sdk.signatures(sdk_functions)
    excluded = sorted(name for name in sdk_signatures if EXCLUDED_FUNCTIONS.match(name))
    our_signatures, unusable = ours.signatures(name for name in sdk_signatures if name not in excluded)
    different = sorted(name for name in our_signatures if our_signatures[name] != sdk_signatures[name] and not KNOWN_SIGNATURES.match(name))
    report.append("functions: %d compared, %d excluded, %d missing or unavailable, %d different" %
                  (len(sdk_signatures) - len(excluded), len(excluded), len(unusable), len(different)))
    for name in different:
        report.append("  different %s: SDK %s, ours %s" % (name, sdk_signatures[name], our_signatures[name]))
    for name in sorted(unusable):
        report.append("  missing %s" % name)
    problems += len(different) + len(unusable)

    our_names = macro_names(ours) | declared_names(ours.tree())
    unchecked = sorted(name for name in macro_names(sdk) - set(sdk_values) - our_names
                       if not name.startswith("_") and where.get(name) not in EXCLUDED_HEADERS and where.get(name) is not None
                       and not EXCLUDED_MACROS.match(name))
    report.append("other macros: %d missing" % len(unchecked))
    for name in unchecked:
        report.append("  missing %s" % name)
    problems += len(unchecked)
    return problems, report


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Compare velo-toolchain's headers with the Windows CE 1.0 and 2.0 SDK headers")
    parser.add_argument("--reference", default=os.environ.get("VELO_REFERENCE"), help="tools/fetch-reference folder, default $VELO_REFERENCE")
    parser.add_argument("--clang", default=os.environ.get("CLANG", "clang"))
    parser.add_argument("--ce", choices=["1", "2"], action="append")
    parser.add_argument("--report", help="write the full report here")
    arguments = parser.parse_args()
    if not arguments.reference:
        sys.exit("check-headers: set VELO_REFERENCE or pass --reference (see tools/fetch-reference)")
    total = 0
    lines = []
    with tempfile.TemporaryDirectory() as work:
        for version in sorted(int(ce) for ce in (arguments.ce or ["1", "2"])):
            problems, report = check(arguments.clang, os.path.expanduser(arguments.reference), version, work)
            total += problems
            lines += report
    if arguments.report:
        open(arguments.report, "w").write("\n".join(lines) + "\n")
    print("\n".join(line for line in lines if not line.startswith("  ")))
    sys.exit(1 if total else 0)
