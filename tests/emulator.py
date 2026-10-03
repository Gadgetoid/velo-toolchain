import argparse
import concurrent.futures
import os
import shutil
import struct
import subprocess
import sys
import shlex
import tempfile
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TARGETS = {
    "1": {
        "rom": lambda: os.path.join(environment("VELO_EMU"), "rom", "nk.bin"),
        "state": lambda: os.path.join(environment("VELO_APPS"), "tools", "clean-desktop.state"),
        "card_root": "\\PC Card",
        "card_ready": 3,
        "base_image": lambda: None,
    },
    "2": {
        "rom": lambda: environment("VELO_CE2_ROM"),
        "state": lambda: optional_environment("VELO_CE2_STATE") or os.path.join(environment("VELO_APPS"), "tools", "clean-desktop-ce2.state"),
        "card_root": "\\Storage Card",
        "card_ready": 6,
        "base_image": lambda: optional_environment("VELO_CE2_SYSTEM_CARD"),
    },
}
CARD_FOLDER = "EXAMPLES"
SETTLE_SECONDS = 10
EXTRA_EVENTS = {"window.exe": ["--tap={at}:200:120"]}
ARGUMENTS = {"greeter.exe": "{folder}\\greet.dll"}
NETWORK_SETTLE_SECONDS = {}
RAPI_WAIT_SECONDS = 60


def environment(name):
    value = optional_environment(name)
    if not value:
        sys.exit("%s is not set" % name)
    return value


def optional_environment(name):
    value = os.environ.get(name)
    return os.path.expanduser(value) if value else None


def load_target(ce):
    settings = TARGETS[ce]
    return {
        **settings,
        "emulator": environment("VELO_EMU"),
        "rom": settings["rom"](),
        "state": settings["state"](),
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


def headless(target, card, seconds, events, screenshot, cell):
    return [os.path.join(target["emulator"], "headless"), target["rom"], "--seconds=%d" % seconds, "--load=%s" % target["state"],
            "--card=%s" % card, *events, "--png=%s" % screenshot, "--png-cell=%d" % cell]


def run_typed(target, card, program, arguments, screenshot, cell):
    launch = target["card_ready"]
    path = '"%s\\%s\\%s"' % (target["card_root"], CARD_FOLDER, program)
    if arguments:
        path += " " + arguments
    typed_at = launch + 2
    started = typed_at + 0.04 * len(path) + 0.4
    events = ["--tap=%d:15:227" % launch, "--type=%d:r" % (launch + 1), "--type=%.2f:%s" % (typed_at, path), "--type=%.2f:\\n" % started]
    events += [event.format(at="%.2f" % (started + 6)) for event in EXTRA_EVENTS.get(program, [])]
    subprocess.run(headless(target, card, started + SETTLE_SECONDS, events, screenshot, cell), capture_output=True, timeout=600, check=True)


def rapi(target, socket, *command):
    return subprocess.run([os.path.join(target["emulator"], "velo-rapi"), "--socket=%s" % socket, *command], capture_output=True)


class Offline(Exception):
    pass


def run_networked(target, card, program, arguments, screenshot, cell, work):
    socket = os.path.join(work, "rapi.sock")
    events = ["--net=1", "--realtime", "--rapi=%s" % socket]
    seconds = RAPI_WAIT_SECONDS + NETWORK_SETTLE_SECONDS[program] + 30
    emulator = subprocess.Popen(headless(target, card, seconds, events, screenshot, cell), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    try:
        deadline = time.monotonic() + RAPI_WAIT_SECONDS
        while rapi(target, socket, "info").returncode:
            if time.monotonic() > deadline or emulator.poll() is not None:
                raise Offline()
            time.sleep(1)
        path = "\\".join((target["card_root"], CARD_FOLDER, program))
        launched = rapi(target, socket, "run", path, *shlex.split(arguments))
        if launched.returncode:
            raise RuntimeError("%s: velo-rapi run failed: %s" % (program, launched.stderr.decode().strip()))
        time.sleep(NETWORK_SETTLE_SECONDS[program])
        emulator.terminate()
        emulator.wait(timeout=60)
    finally:
        if emulator.poll() is None:
            emulator.kill()


def run_program(target, image, program, screenshot, cell, values=None):
    folder = "%s\\%s" % (target["card_root"], CARD_FOLDER)
    arguments = ARGUMENTS.get(program, "").format(folder=folder, **(values or {}))
    with tempfile.TemporaryDirectory() as work:
        card = os.path.join(work, "card.img")
        shutil.copyfile(image, card)
        if program in NETWORK_SETTLE_SECONDS:
            try:
                run_networked(target, card, program, arguments, screenshot, cell, work)
                return screenshot
            except Offline:
                print("%s: no RAPI answer, running offline" % program, file=sys.stderr)
                shutil.copyfile(image, card)
        run_typed(target, card, program, arguments, screenshot, cell)
    return screenshot


def main(description="Run each built example in velo-emu and screenshot it", values=None):
    parser = argparse.ArgumentParser(description=description)
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
            screenshots = pool.map(lambda program: run_program(target, image, program, os.path.join(output, program[:-4] + ".png"), arguments.cell, values), programs)
            for screenshot in screenshots:
                print(screenshot)


if __name__ == "__main__":
    main()
