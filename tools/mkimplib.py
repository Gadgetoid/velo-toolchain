import argparse
import re

IDENTIFIER = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")


def stub(function, dll):
    return f"""    .section .text.{function},"ax",@progbits
    .weak {function}
    .type {function},@function
{function}:
    lui $25, %hi(__imp_{function})
    lw $25, %lo(__imp_{function})($25)
    nop
    jr $25
    nop

    .section .iat.{dll}.{function},"aw",@progbits
    .balign 4
    .weak __imp_{function}
    .weak "__velo_import${dll}${function}"
__imp_{function}:
"__velo_import${dll}${function}":
    .word 0

"""


def generate(functions, dll):
    lines = ["    .set noreorder", "    .set noat", ""]
    lines += [stub(function, dll) for function in functions]
    return "\n".join(lines)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate a MIPS import library for a Windows CE DLL from its export list")
    parser.add_argument("exports")
    parser.add_argument("dll")
    parser.add_argument("output")
    arguments = parser.parse_args()
    functions = []
    for line in open(arguments.exports):
        name = line.strip()
        if IDENTIFIER.match(name) and name not in functions:
            functions.append(name)
    with open(arguments.output, "w") as output:
        output.write(generate(functions, arguments.dll))
