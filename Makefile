TOOLCHAIN = $(CURDIR)/cmake/velo-ce.cmake
CLANG ?= $(firstword $(wildcard $(shell brew --prefix llvm 2>/dev/null)/bin/clang) clang)

.PHONY: examples test screenshots debugmgr debug-state headers check-headers clean

examples:
	cmake -S examples -B build/ce1 -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DVELO_CE_VERSION=1
	cmake --build build/ce1
	cmake -S examples -B build/ce2 -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DVELO_CE_VERSION=2
	cmake --build build/ce2

test: examples
	python3 tests/emulator.py build/ce1 --ce 1
	python3 tests/emulator.py build/ce2 --ce 2

screenshots: examples
	python3 tests/emulator.py build/ce1 --ce 1 --cell 2 --output docs/screenshots/ce1
	python3 tests/emulator.py build/ce2 --ce 2 --cell 2 --output docs/screenshots/ce2
	pngquant --force --strip --quality=60-80 --ext .png docs/screenshots/ce*/*.png

debugmgr:
	cmake -S debugmgr -B build/debugmgr/ce1 -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DVELO_CE_VERSION=1
	cmake --build build/debugmgr/ce1
	cmake -S debugmgr -B build/debugmgr/ce2 -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) -DVELO_CE_VERSION=2
	cmake --build build/debugmgr/ce2

debug-state: debugmgr
	python3 tools/mkdebugstate.py build/debugmgr/ce1 build/debugmgr/ce1/debug-desktop.state --ce 1
	python3 tools/mkdebugstate.py build/debugmgr/ce2 build/debugmgr/ce2/debug-desktop.state --ce 2

headers:
	python3 tools/vendor-w32api.py $(VELO_W32API)
	python3 tools/mkheaders.py --clang $(CLANG)
	python3 tools/mkconstants.py --clang $(CLANG)

check-headers:
	python3 tests/check-headers.py --clang $(CLANG)

clean:
	rm -rf build
