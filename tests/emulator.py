import argparse
import concurrent.futures
import os
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EMULATOR = os.path.expanduser(os.environ.get("VELO_EMU", "~/Development/misc/velo-emu"))
VELO_APPS = os.path.expanduser(os.environ.get("VELO_APPS", "~/Development/misc/velo-apps"))
CE2_BUNDLE = os.path.expanduser("~/Development/misc/cerf-bundles/philips_velo_1_ce2/rom")
TARGETS = {
    "1": {
        "rom": os.path.join(EMULATOR, "rom", "nk.bin"),
        "state": os.path.join(VELO_APPS, "tools", "clean-desktop.state"),
        "card_root": "\\PC Card",
        "card_ready": 3,
        "base_image": None,
    },
    "2": {
        "rom": os.environ.get("VELO_CE2_ROM", os.path.join(CE2_BUNDLE, "nk.bin")),
        "state": os.path.join(VELO_APPS, "tools", "clean-desktop-ce2.state"),
        "card_root": "\\Storage Card",
        "card_ready": 6,
        "base_image": os.environ.get("VELO_CE2_SYSTEM_CARD", os.path.join(CE2_BUNDLE, "ce2_sys.img")),
    },
}
CARD_FOLDER = "EXAMPLES"
SETTLE_SECONDS = 10
EXTRA_EVENTS = {"window.exe": ["--tap={at}:200:120"]}
ARGUMENTS = {"greeter.exe": "{folder}\\greet.dll"}


def write_png(pgm_path, png_path):
    data = open(pgm_path, "rb").read()
    fields = data.split(maxsplit=4)
    width, height, maximum = int(fields[1]), int(fields[2]), int(fields[3])
    pixels = fields[4][-width * height:]
    rows = b"".join(b"\0" + bytes(value * 255 // maximum for value in pixels[row * width:(row + 1) * width]) for row in range(height))

    def chunk(kind, body):
        return struct.pack(">I", len(body)) + kind + body + struct.pack(">I", zlib.crc32(kind + body))

    header = struct.pack(">IIBBBBB", width, height, 8, 0, 0, 0, 0)
    open(png_path, "wb").write(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", header) + chunk(b"IDAT", zlib.compress(rows)) + chunk(b"IEND", b""))


def copy_base_image(image, destination):
    attached = subprocess.run(["hdiutil", "attach", "-readonly", "-nobrowse", "-imagekey", "diskimage-class=CRawDiskImage", image],
                              capture_output=True, text=True, check=True).stdout
    mount = attached.strip().splitlines()[-1].split("\t")[-1]
    items = []
    try:
        for name in sorted(os.listdir(mount)):
            if name.startswith("."):
                continue
            source = os.path.join(mount, name)
            target = os.path.join(destination, name)
            if os.path.isdir(source):
                shutil.copytree(source, target)
            else:
                shutil.copyfile(source, target)
            items.append(target)
    finally:
        subprocess.run(["hdiutil", "detach", "-quiet", mount])
    return items


def make_card(build, target, work):
    folder = os.path.join(work, "stage", CARD_FOLDER)
    os.makedirs(folder)
    programs = []
    for directory, _, files in os.walk(build):
        for name in files:
            if name.endswith((".exe", ".dll")) and "CMakeFiles" not in directory:
                shutil.copyfile(os.path.join(directory, name), os.path.join(folder, name))
                if name.endswith(".exe"):
                    programs.append(name)
    items = [folder]
    if target["base_image"]:
        base = os.path.join(work, "base")
        os.makedirs(base)
        items += copy_base_image(target["base_image"], base)
    image = os.path.join(work, "card.img")
    subprocess.run([os.path.join(EMULATOR, "tools", "mkcard.sh"), image, "16", *items], check=True, capture_output=True)
    return image, sorted(programs)


def run_program(target, image, program, screenshot):
    launch = target["card_ready"]
    folder = "%s\\%s" % (target["card_root"], CARD_FOLDER)
    path = '"%s\\%s"' % (folder, program)
    if program in ARGUMENTS:
        path += " " + ARGUMENTS[program].format(folder=folder)
    typed_at = launch + 2
    started = typed_at + 0.04 * len(path) + 0.4
    events = ["--tap=%d:15:227" % launch, "--type=%d:r" % (launch + 1), "--type=%.2f:%s" % (typed_at, path), "--type=%.2f:\\n" % started]
    events += [event.format(at="%.2f" % (started + 6)) for event in EXTRA_EVENTS.get(program, [])]
    with tempfile.TemporaryDirectory() as work:
        card = os.path.join(work, "card.img")
        pgm = os.path.join(work, "screen.pgm")
        shutil.copyfile(image, card)
        subprocess.run([os.path.join(EMULATOR, "headless"), target["rom"], "--seconds=%d" % (started + SETTLE_SECONDS),
                        "--load=%s" % target["state"], "--card=%s" % card, *events, "--pgm=%s" % pgm],
                       capture_output=True, timeout=600, check=True)
        write_png(pgm, screenshot)
    return screenshot


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run each built example in velo-emu and screenshot it")
    parser.add_argument("build", help="CMake build directory")
    parser.add_argument("--ce", choices=sorted(TARGETS), default="1")
    parser.add_argument("--output", help="screenshot directory, default <build>/screenshots")
    arguments = parser.parse_args()
    target = TARGETS[arguments.ce]
    output = arguments.output or os.path.join(arguments.build, "screenshots")
    os.makedirs(output, exist_ok=True)
    with tempfile.TemporaryDirectory() as work:
        image, programs = make_card(arguments.build, target, work)
        if not programs:
            sys.exit("no programs in %s" % arguments.build)
        with concurrent.futures.ThreadPoolExecutor() as pool:
            screenshots = pool.map(lambda program: run_program(target, image, program, os.path.join(output, program[:-4] + ".png")), programs)
            for screenshot in screenshots:
                print(screenshot)
