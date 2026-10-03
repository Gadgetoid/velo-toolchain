import argparse
import concurrent.futures
import os
import shutil
import struct
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TARGETS = {
    "1": {
        "rom": lambda: os.path.join(environment("VELO_EMU"), "rom", "nk.bin"),
        "state": "clean-desktop.state",
        "card_root": "\\PC Card",
        "card_ready": 3,
        "base_image": lambda: None,
    },
    "2": {
        "rom": lambda: environment("VELO_CE2_ROM"),
        "state": "clean-desktop-ce2.state",
        "card_root": "\\Storage Card",
        "card_ready": 6,
        "base_image": lambda: environment("VELO_CE2_SYSTEM_CARD"),
    },
}
CARD_FOLDER = "EXAMPLES"
SETTLE_SECONDS = 10
EXTRA_EVENTS = {"window.exe": ["--tap={at}:200:120"]}
ARGUMENTS = {"greeter.exe": "{folder}\\greet.dll"}


def environment(name):
    value = os.environ.get(name)
    if not value:
        sys.exit("%s is not set" % name)
    return os.path.expanduser(value)


def load_target(ce):
    settings = TARGETS[ce]
    return {
        **settings,
        "emulator": environment("VELO_EMU"),
        "rom": settings["rom"](),
        "state": os.path.join(environment("VELO_APPS"), "tools", settings["state"]),
        "base_image": settings["base_image"](),
    }


def copy_mounted_image(image, destination):
    attached = subprocess.run(["hdiutil", "attach", "-readonly", "-nobrowse", "-imagekey", "diskimage-class=CRawDiskImage", image],
                              capture_output=True, text=True, check=True).stdout
    mount = attached.strip().splitlines()[-1].split("\t")[-1]
    try:
        for name in os.listdir(mount):
            if name.startswith("."):
                continue
            source = os.path.join(mount, name)
            target = os.path.join(destination, name)
            if os.path.isdir(source):
                shutil.copytree(source, target)
            else:
                shutil.copyfile(source, target)
    finally:
        subprocess.run(["hdiutil", "detach", "-quiet", mount])


def copy_image_with_mtools(image, destination):
    with open(image, "rb") as file:
        master_boot_record = file.read(512)
    partition_offset = struct.unpack_from("<I", master_boot_record, 0x1c6)[0] * 512
    subprocess.run(["mcopy", "-s", "-i", "%s@@%d" % (image, partition_offset), "::/*", destination], capture_output=True, check=True)


def copy_base_image(image, destination):
    if sys.platform == "darwin":
        copy_mounted_image(image, destination)
    else:
        copy_image_with_mtools(image, destination)
    return [os.path.join(destination, name) for name in sorted(os.listdir(destination)) if not name.startswith(".")]


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
    subprocess.run([os.path.join(target["emulator"], "tools", "mkcard.sh"), image, "16", *items], check=True, capture_output=True)
    return image, sorted(programs)


def run_program(target, image, program, screenshot, cell):
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
        shutil.copyfile(image, card)
        subprocess.run([os.path.join(target["emulator"], "headless"), target["rom"], "--seconds=%d" % (started + SETTLE_SECONDS),
                        "--load=%s" % target["state"], "--card=%s" % card, *events, "--png=%s" % screenshot, "--png-cell=%d" % cell],
                       capture_output=True, timeout=600, check=True)
    return screenshot


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run each built example in velo-emu and screenshot it")
    parser.add_argument("build", help="CMake build directory")
    parser.add_argument("--ce", choices=sorted(TARGETS), default="1")
    parser.add_argument("--output", help="screenshot directory, default <build>/screenshots")
    parser.add_argument("--cell", type=int, default=4, help="device pixels per LCD pixel")
    arguments = parser.parse_args()
    target = load_target(arguments.ce)
    output = arguments.output or os.path.join(arguments.build, "screenshots")
    os.makedirs(output, exist_ok=True)
    with tempfile.TemporaryDirectory() as work:
        image, programs = make_card(arguments.build, target, work)
        if not programs:
            sys.exit("no programs in %s" % arguments.build)
        with concurrent.futures.ThreadPoolExecutor() as pool:
            screenshots = pool.map(lambda program: run_program(target, image, program, os.path.join(output, program[:-4] + ".png"), arguments.cell), programs)
            for screenshot in screenshots:
                print(screenshot)
