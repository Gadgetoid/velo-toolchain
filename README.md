# velo-toolchain

A CMake toolchain for Windows CE 1.0 and 2.0 MIPS programs and DLLs, aimed at the Philips Velo 1 (Toshiba TX3912, R3910 core). It uses clang and lld, then `tools/mkpe.py` turns the linked ELF into a CE PE.

## Requirements

macOS:

```sh
brew install llvm lld cmake
```

Debian/Ubuntu:

```sh
sudo apt install clang lld llvm cmake
```

`RESOURCES` also needs `llvm-rc`, which is in both.

Apple's clang has no MIPS backend. The toolchain uses Homebrew's LLVM if present, otherwise the `clang` on `PATH`. Set `VELO_LLVM_ROOT` to use another LLVM install.

## Usage

```cmake
cmake_minimum_required(VERSION 3.20)
project(myapp C)
include(VeloCE)

velo_add_executable(myapp main.c ICON myapp.ico RESOURCES myapp.rc resource.h)
target_link_libraries(myapp PRIVATE velo::commctrl)

velo_add_library(mylib mylib.c EXPORTS Add Greeting=greeting_impl)
```

```sh
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=/path/to/velo-toolchain/cmake/velo-ce.cmake -DVELO_CE_VERSION=1
cmake --build build
```

This gives `build/myapp.exe` and `build/mylib.dll`, with the linked `.elf` beside each.

- `velo_add_executable(target [EXCLUDE_FROM_ALL] [OUTPUT file.exe] [ICON file.ico] [RESOURCES files...] sources...)`: entry point `WinMain`.
- `velo_add_library(target [EXCLUDE_FROM_ALL] [OUTPUT file.dll] [RESOURCES files...] EXPORTS name[=symbol]... | EXPORTS_FILE file sources...)`: entry point `DllMain`. `EXPORTS_FILE` has one export per line.
- `RESOURCES`: `.rc` files are compiled with `llvm-rc` and linked into `.rsrc` with the icon. Other files listed (headers, bitmaps) are dependencies. The `.rc` can include `windows.h` and `commctrl.h`. CE has no `DialogBoxParamW`: use `FindResource`, `LoadResource` and `DialogBoxIndirectParamW`.
- `VELO_CE_VERSION`: `1` (default) or `2`. Sets `_WIN32_WCE` to `100` or `200` and picks the import libraries.
- `CMAKE_BUILD_TYPE` defaults to `MinSizeRel` (`-Os`). `Debug` gives `-O0 -g`.

## Import libraries

`velo::<dll>` links against a DLL in the Velo 1 ROM. `exports/ce1/` and `exports/ce2/` list each DLL's exports, taken from the CE 1.0 and CE 2.0 ROMs. Calling a function the ROM doesn't export fails at link time:

```
ld.lld: error: undefined symbol: GetModuleFileNameW
```

`velo::coredll` is always linked. Others: `commctrl`, `winsock`, and on CE 2.0 `commdlg`, `ole32`, `oleaut32` and `ceshell`. Only the functions a program calls end up in its import table.

Each import library is a set of weak stubs (`lui`/`lw`/`jr` through `__imp_<name>`) with a 4-byte slot per function in `.iat.<DLL>.<name>`, so unused stubs are dropped by `--gc-sections` and a program's own definition of a name overrides the import.

## Headers and runtime

The Win32 headers are CeGCC's w32api (public domain) in `include/w32api`, patched so every constant, structure layout and function signature they share with the Windows CE 1.0 and 2.0 SDKs matches. `include/` wraps them:

- `#include <windows.h>` gives the API as the target CE version has it: `_WIN32_WCE` is `100` or `200`, `UNICODE` is defined, and it brings in the C runtime functions coredll exports (`wchar.h`, `stdlib.h`, `string.h`) and the object store database (`windbase.h`, with both the `Ce` and `Peg` names on both versions). `commctrl.h`, `commdlg.h`, `winsock.h`, `notify.h`, `ras.h`, `msacm.h`, `imm.h`, `tlhelp32.h` and the rest include it.
- Calling a function the target ROM doesn't export is a compile error, e.g. `'GetModuleFileNameW' is unavailable: not in Windows CE 1.0`.
- CE exports some wide functions without the `W` (`FindResource`, `GetVersionEx`, `GetClassLong`, the `Ras` and `acm` functions): either name works.
- Every function the ROMs export has a docstring and named parameters, for editor hover and signature help. They're written from the CE 1.0 and 2.0 SDK references, with CE restrictions and the version that has the function.
- `velo::runtime`, always linked: compiler-rt builtins (soft float, 64-bit division) and `memcpy`, `memmove`, `memset`, `memcmp`. Beyond coredll's wide-string functions there is no C library: use `LocalAlloc`, `wsprintfW` and your own maths.

Not covered, so declare them yourself if you need them:

- COM and OLE (`ole32`, `oleaut32` on CE 2.0)
- telephony (`line` and `phone` functions)
- audio codec format IDs (`mmreg.h`), and the ACM filter and format chooser functions
- drivers and kernel internals: PC Card services, device loading, `kfuncs.h` handles
- the address book, mail store and transport APIs
- CE-internal types such as `INT128` and the RAS internals
- ANSI (`A`) structures, which can differ from the SDK's: CE 1.0 and 2.0 have no ANSI functions

`make check-headers` compares the headers with the SDK headers from `tools/fetch-reference` (`VELO_REFERENCE`). `make headers` regenerates them from a CeGCC checkout's w32api (`VELO_W32API`) and the SDK headers:

- `tools/vendor-w32api.py` copies and patches w32api; `include/w32api/VENDOR.md` lists the patches.
- `tools/mkheaders.py` writes the wrappers, `velo/aliases.h` and `velo/unavailable.h` from `exports/`.
- `tools/mkconstants.py` writes `velo/constants.h` and `velo/types.h`: SDK constants and typedefs w32api lacks.
- `tools/mkdocs.py` (also `make docstrings`) writes the docstrings from `docs/api` above each declaration, and names its parameters.

Both need a clang with the MIPS backend (`CLANG`, default Homebrew's).

## Notes

- CE 1.0 has no `GetModuleFileNameW`, and `LoadLibraryW` doesn't search the program's folder. DLLs normally go in `\Windows`, or pass a full path (see `examples/dll`).
- A DLL's import tables go in a 0x200 byte space before its IAT. `mkpe.py` says if a DLL needs more: `target_link_options(mylib PRIVATE --defsym=VELO_IMPORT_RESERVE=0x400)`.
- `tools/velo-cc` filters clang's "MIPS-I support is experimental" warning. Set `CMAKE_C_COMPILER_LAUNCHER` to replace it.

## Debugging

`-DCMAKE_BUILD_TYPE=Debug` builds with `-O0 -g`. The debug info stays in the `.elf`; the `.exe` or `.dll` is the same.

`OutputDebugStringW` output, and the kernel's register dump when a program crashes, show with velo-emu's `--debug-output`. `tools/velo-symbolize` adds functions and source lines to the addresses in it:

```sh
headless rom/nk.bin ... --debug-output 2>&1 | tools/velo-symbolize build/myapp.exe
# debug: AKY=00000003 PC=000110bc [deep crash.c:7] RA=000110ac [deep crash.c:7] BVA=00000000
tools/velo-symbolize build/myapp.exe -a 110bc
```

It reads the `.elf` beside each `.exe` or `.dll` given. A DLL needs the address CE loaded it at: `build/mylib.dll@01f00000`. Addresses in other process slots are matched slot-relative. It uses `llvm-symbolizer`, from `VELO_LLVM_ROOT`, `PATH` or Homebrew's LLVM.

### debugmgr

`debugmgr/` is an agent for velo-emu that copies files and starts and stops programs, much faster than RAPI and without a PPP connection. It talks to the emulator through velo-emu's host mailbox (`break 0x51CE`, see velo-emu's README), so it only runs in the emulator.

```sh
make debug-state  # build/debugmgr/ce*/debug-desktop.state, with debugmgr running
headless rom/nk.bin --load=build/debugmgr/ce1/debug-desktop.state --agent=agent.sock ...
export VELO_AGENT=agent.sock
tools/velo-debug ping
tools/velo-debug put build/ce1/maths/maths.exe /Windows/maths.exe
tools/velo-debug run /Windows/maths.exe     # prints the process ID
tools/velo-debug kill 0x80013f74
```

`make debug-state` needs the same settings as `make test`. It starts debugmgr from a card, copies it to `\Windows`, hands over to that copy and saves the state, so loading it gives a Velo with debugmgr already answering. The card is out after loading, which for CE 2.0 with a separate system card includes the system card.

`velo-debug` also has `get`, `ls`, `rm`, `mkdir`, `rmdir`, `mv`, `quit` and `handover` (start another debugmgr, then stop). `kill` only ends programs debugmgr started. The message format is in [debugmgr/PROTOCOL.md](debugmgr/PROTOCOL.md).

velo-emu's GDB stub also uses debugmgr, for `remote put`, `remote get` and `run` after `target extended-remote`. GDB treats `\` as an escape in those paths, so use `/` (`remote put build/ce1/maths/maths.exe /Windows/maths.exe`) or double it. Unix socket paths are limited to about 100 characters, so keep the socket's path short or relative.

Run debugmgr from RAM (`\Windows`) rather than a card: CE loads its code from the EXE as it runs, and a card being remounted after `--load` fails that.

### GDB and VS Code

Each executable gets a GDB script beside its `.elf`, `<name>.elf.gdb`. It loads the symbols, sets the program to run as `\Windows\<name>.exe`, and defines `velo-load`, which uploads the `.exe` and the project's DLLs (all but `EXCLUDE_FROM_ALL` ones) to `\Windows` through debugmgr, where `LoadLibraryW` finds them by name. `tools/velo-emulator --ce 1|2` starts velo-emu from the debug desktop state with its GDB stub on port 2001 (CE 1.0) or 2002 (CE 2.0), or reports one already running there. Add `--headless` for no window.

```sh
tools/velo-emulator --ce 1 &
gdb -x build/myapp.elf.gdb -ex "target extended-remote :2001" -ex "set target-wide-charset UTF-16LE" -ex velo-load -ex "break WinMain" -ex run
```

Set the character set after connecting, since connecting resets it, so `WCHAR` strings show as text.

For VS Code, copy `vscode/launch.json`, `vscode/tasks.json` and `vscode/settings.json` into a project's `.vscode`. The settings have IntelliSense take include paths from CMake Tools. They need:

- the C/C++ and CMake Tools extensions, with the project configured with this toolchain, `CMAKE_BUILD_TYPE=Debug`, and the `VELO_CE_VERSION` that matches the launch configuration you pick
- `gdb` with MIPS support (Homebrew's `gdb` on macOS, `gdb-multiarch` on Linux)
- `VELO_TOOLCHAIN` (this folder), `VELO_EMU`, and for CE 2.0 `VELO_CE2_ROM`, in VS Code's environment
- `make debug-state` run once here

F5 starts the emulator if it isn't running, uploads the selected target's `.exe` and runs it, stopping at breakpoints. The emulator keeps running between sessions.

## Reference material

`tools/fetch-reference FOLDER` (or `$VELO_REFERENCE`) downloads the Windows CE 1.0, 1.01 and 2.0 SDK headers, the CE 2.0 toolkit's Win32 samples and the toolkits' documentation (InfoViewer `.ivt` titles, including the CE 1.0 and 2.0 SDK references and the PR3910 processor reference) from archive.org, and extracts the titles to HTML with `tools/extract-ivt`. They're Microsoft's, for reference only.

## Examples and tests

`examples/` has `hello` (message box), `window` (window, painting, taps, icon), `maths` (soft float and 64-bit integers) and `dll` (a DLL and a program that loads it).

`docs/primer/index.html` is a beginner's guide to writing Velo programs with this toolchain. Its examples are in `docs/primer/examples`: `make primer` builds them, and `make primer-screenshots` updates its screenshots, with the same settings as `make test`.

```sh
make examples   # build for CE 1.0 and CE 2.0 into build/ce1 and build/ce2
make test       # also run each example in velo-emu, screenshots in build/ce*/screenshots
make screenshots  # update the screenshots below, in docs/screenshots
```

`make test` and `make screenshots` need these set:

- `VELO_EMU`: a built velo-emu checkout
- `VELO_APPS`: a velo-apps checkout, for the desktop states in `tools/`
- `VELO_CE2_ROM`: the CE 2.0 `nk.bin`, or velo-emu's merged image (CE 2.0 only)
- `VELO_CE2_SYSTEM_CARD`: the CE 2.0 `ce2_sys.img`, if the ROM isn't merged
- `VELO_CE2_STATE`: a CE 2.0 desktop state, if not velo-apps' `clean-desktop-ce2.state` (which needs the separate ROM and system card)

On Linux they also need `dosfstools` and `mtools`. `make screenshots` also needs `pngquant`.

Other projects can use `tests/emulator.py` for their own programs: import it, set `ARGUMENTS` and `NETWORK_SETTLE_SECONDS`, and call `main()`, as velo-bluesky does. Programs in `NETWORK_SETTLE_SECONDS` get the PPP network and web proxy, are started over RAPI (`velo-rapi run`) once the Velo is online, and are screenshotted after that many seconds of real time. If the Velo doesn't answer over RAPI, as with the separate CE 2.0 ROM and system card, they're started from the Run dialog instead.

| | CE 1.0 | CE 2.0 |
| --- | --- | --- |
| `hello` | ![hello on CE 1.0](docs/screenshots/ce1/hello.png) | ![hello on CE 2.0](docs/screenshots/ce2/hello.png) |
| `window` | ![window on CE 1.0](docs/screenshots/ce1/window.png) | ![window on CE 2.0](docs/screenshots/ce2/window.png) |
| `maths` | ![maths on CE 1.0](docs/screenshots/ce1/maths.png) | ![maths on CE 2.0](docs/screenshots/ce2/maths.png) |
| `dll` | ![greeter on CE 1.0](docs/screenshots/ce1/greeter.png) | ![greeter on CE 2.0](docs/screenshots/ce2/greeter.png) |

## Sources

- `tools/mkpe.py` and the linker scripts: from velo-apps' installer build, extended for imports from more than one DLL.
- `include/windows.h`, `include/winsock.h`: from velo-micropython's `ce.h`.
- `exports/`: from velo-apps' ROM export tables (`tools/velo1_rom_exports.json`, `tools/velo1_ce2_*_exports.tsv`).
- `runtime/compiler-rt/`: LLVM compiler-rt builtins, Apache 2.0 with LLVM exceptions, see `runtime/compiler-rt/LICENSE.TXT`.
