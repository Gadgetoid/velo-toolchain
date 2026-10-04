import argparse
import hashlib
import os
import shutil
import subprocess
import sys
import tempfile
import time
from importlib.machinery import SourceFileLoader

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tests"))
import emulator

velo_debug = SourceFileLoader("velo_debug", os.path.join(ROOT, "tools", "velo-debug")).load_module()

AGENT = "debugmgr.exe"
INSTALLED = "/Windows/debugmgr.exe"
SH3_AGENT = "velo-debugmgr.exe"
SH3_STAGED = "velo-debugmgr-build.exe"
SH3_INSTALLED = "/Windows/velo-debugmgr-build.exe"
SOCKET = "agent.sock"
CONNECT_SECONDS = 120


def connect(emulator_process):
    deadline = time.monotonic() + CONNECT_SECONDS
    while True:
        try:
            return velo_debug.Agent(SOCKET, 30)
        except (OSError, velo_debug.AgentError):
            if emulator_process.poll() is not None:
                sys.exit("the emulator stopped before debugmgr answered")
            if time.monotonic() > deadline:
                sys.exit("debugmgr didn't answer")
            time.sleep(0.2)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Save an emulator state with debugmgr running from \\Windows")
    parser.add_argument("build", help="debugmgr build directory")
    parser.add_argument("output", help="state file to write")
    parser.add_argument("--ce", choices=["1", "2"], default="1")
    parser.add_argument("--arch", choices=["mips", "sh3"], default="mips", help="sh3: the SH3 emulator (VELO_SH3_EMU, VELO_SH3_ROM)")
    arguments = parser.parse_args()
    target = emulator.load_target(arguments.ce, arguments.arch)
    agent_name, launched, installed = (SH3_AGENT, SH3_STAGED, SH3_INSTALLED) if arguments.arch == "sh3" else (AGENT, AGENT, INSTALLED)
    output = os.path.abspath(arguments.output)
    build = os.path.abspath(arguments.build)
    agent_path = os.path.join(build, agent_name)
    with tempfile.TemporaryDirectory() as work:
        os.chdir(work)
        if target.get("folder"):
            target["state"] = emulator.make_desktop_state(target, work)
        media, _ = emulator.make_media(build, target, work)
        if not target.get("folder"):
            image = os.path.join(os.path.dirname(output), "debug-card.img")
            shutil.move(media, image)
            media = image
        if launched != agent_name:
            os.rename(os.path.join(media, agent_name), os.path.join(media, launched))
        events, _ = emulator.launch_events(target, launched, "")
        command = [os.path.join(target["emulator"], "headless"), target["rom"], "--load=%s" % target["state"], emulator.media_option(target, media),
                   *events, "--agent=%s" % SOCKET, "--seconds=100000", "--save=%s" % output]
        emulator_process = subprocess.Popen(command, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        try:
            agent = connect(emulator_process)
            agent.put(agent_path, installed)
            agent.handover(installed)
            agent.ping()
            emulator_process.terminate()
            emulator_process.wait(timeout=60)
        finally:
            if emulator_process.poll() is None:
                emulator_process.kill()
    if not os.path.exists(output):
        sys.exit("no state saved")
    with open(target["rom"], "rb") as rom:
        digest = hashlib.sha256(rom.read()).hexdigest()
    with open(output + ".rom", "w") as record:
        record.write("%s %s\n" % (digest, os.path.abspath(target["rom"])))
    print(output)
