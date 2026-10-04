import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..", "tests"))

import emulator

emulator.EXTRA_EVENTS = {"fetch.exe": ["--net=1", "--realtime"], "window.exe": ["--tap={at}:200:120"]}
emulator.ARGUMENTS = {"fetch.exe": "http://example.com/"}

if __name__ == "__main__":
    emulator.main("Run each primer example in velo-emu and screenshot it")
