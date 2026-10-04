import argparse
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
    parser = argparse.ArgumentParser(description="Save a velo-emu state with debugmgr running from \\Windows")
    parser.add_argument("build", help="debugmgr build directory")
    parser.add_argument("output", help="state file to write")
    parser.add_argument("--ce", choices=sorted(emulator.TARGETS), default="1")
    arguments = parser.parse_args()
    target = emulator.load_target(arguments.ce)
    output = os.path.abspath(arguments.output)
    build = os.path.abspath(arguments.build)
    agent_path = os.path.join(build, AGENT)
    with tempfile.TemporaryDirectory() as work:
        os.chdir(work)
        made_image, _ = emulator.make_card(build, target, work)
        image = os.path.join(os.path.dirname(output), "debug-card.img")
        shutil.move(made_image, image)
        events, _ = emulator.launch_events(target, AGENT, "")
        command = [os.path.join(target["emulator"], "headless"), target["rom"], "--load=%s" % target["state"], "--card=%s" % image, *events,
                   "--agent=%s" % SOCKET, "--seconds=100000", "--save=%s" % output]
        emulator_process = subprocess.Popen(command, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        try:
            agent = connect(emulator_process)
            agent.put(agent_path, INSTALLED)
            agent.handover(INSTALLED)
            agent.ping()
            emulator_process.terminate()
            emulator_process.wait(timeout=60)
        finally:
            if emulator_process.poll() is None:
                emulator_process.kill()
    if not os.path.exists(output):
        sys.exit("no state saved")
    print(output)
