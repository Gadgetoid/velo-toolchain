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

## Import libraries

`velo::<dll>` links against a DLL in the Velo 1 ROM. `exports/ce1/` and `exports/ce2/` list each DLL's exports, taken from the CE 1.0 and CE 2.0 ROMs. Calling a function the ROM doesn't export fails at link time:

```
ld.lld: error: undefined symbol: GetModuleFileNameW
```

`velo::coredll` is always linked. Others: `commctrl`, `winsock`, and on CE 2.0 `commdlg`, `ole32`, `oleaut32` and `ceshell`. Only the functions a program calls end up in its import table.

Each import library is a set of weak stubs (`lui`/`lw`/`jr` through `__imp_<name>`) with a 4-byte slot per function in `.iat.<DLL>.<name>`, so unused stubs are dropped by `--gc-sections` and a program's own definition of a name overrides the import.

## Headers and runtime

- `include/windows.h`, `include/winsock.h`, `include/commctrl.h`: a subset of the Win32 API as the Velo has it. Declare anything missing yourself. `COLOR_*` include CE's `SYS_COLOR_INDEX_FLAG`, which `GetSysColor` needs.
- `include/string.h`: the runtime's `memcpy`, `memmove`, `memset` and `memcmp`.
- `velo::runtime`, always linked: compiler-rt builtins (soft float, 64-bit division) and `memcpy`, `memmove`, `memset`, `memcmp`. There is no C library.

## Notes

- CE 1.0 has no `GetModuleFileNameW`, and `LoadLibraryW` doesn't search the program's folder. DLLs normally go in `\Windows`, or pass a full path (see `examples/dll`).
- A DLL's import tables go in a 0x200 byte space before its IAT. `mkpe.py` says if a DLL needs more: `target_link_options(mylib PRIVATE --defsym=VELO_IMPORT_RESERVE=0x400)`.
- `tools/velo-cc` filters clang's "MIPS-I support is experimental" warning. Set `CMAKE_C_COMPILER_LAUNCHER` to replace it.

## Examples and tests

`examples/` has `hello` (message box), `window` (window, painting, taps, icon), `maths` (soft float and 64-bit integers) and `dll` (a DLL and a program that loads it).

```sh
make examples   # build for CE 1.0 and CE 2.0 into build/ce1 and build/ce2
make test       # also run each example in velo-emu, screenshots in build/ce*/screenshots
make screenshots  # update the screenshots below, in docs/screenshots
```

`make test` and `make screenshots` need these set:

- `VELO_EMU`: a built velo-emu checkout
- `VELO_APPS`: a velo-apps checkout, for the desktop states in `tools/`
- `VELO_CE2_ROM`, `VELO_CE2_SYSTEM_CARD`: the CE 2.0 `nk.bin` and `ce2_sys.img` (CE 2.0 only)

On Linux they also need `dosfstools` and `mtools`. `make screenshots` also needs `pngquant`.

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
