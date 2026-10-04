# w32api

These headers are from CeGCC's w32api (MinGW's Win32 API headers with Windows CE support), as in
https://github.com/Gadgetoid/cegcc-build at 221c9c0, `w32api/include`. They're public domain: see `README.w32api`.

`tools/vendor-w32api.py` copies them from a CeGCC checkout and applies the patches below, so they match the
Windows CE 1.0 and 2.0 SDK headers where `tests/check-headers.py` found a difference. velo-toolchain's own
headers in `include/` configure and include them, and mark what each CE version doesn't export.

Not patched: ANSI (`A`) structures that differ from the SDK's, such as `WIN32_FIND_DATAA` and `REBARBANDINFOA`.
CE 1.0 and 2.0 have no ANSI functions, and their declarations are marked unavailable.

## Patches

- `ras.h`: RAS_MaxEntryName is 20 on CE
- `ras.h`: RAS_MaxDeviceName is 32 on CE
- `ras.h`: RAS_MaxCallbackNumber is 48 on CE
- `ras.h`: RASCONNW has no device or phonebook fields on CE
- `ras.h`: RASCONNW has no phonebook fields on CE
- `ras.h`: RASPPPIPW has no server address on CE
- `ras.h`: RASCONNA has no device or phonebook fields on CE
- `ras.h`: RASCONNA has no phonebook fields on CE
- `ras.h`: RASPPPIPA has no server address on CE
- `ras.h`: RASDEVINFOW's strings are CHAR on CE
- `ras.h`: RASAMBA's szNetBiosError is wide on CE
- `msacm.h`: ACMFORMATDETAILS_FORMAT_CHARS is 128 on CE
- `msacm.h`: ACMFORMATTAGDETAILS_FORMATTAG_CHARS is 48 on CE
- `msacm.h`: ACMDRIVERDETAILS_FEATURES_CHARS is 512 on CE
- `winsock.h`: AF_MAX is 23 on CE 1.0 and 24 on CE 2.0
- `winsock.h`: IOCPARM_MASK is 0x7ff on CE
- `winsock.h`: IPPROTO_GGP is 2 on CE
- `winnls.h`: CTRY_CZECH is 42 on CE
- `winnls.h`: CTRY_SLOVAK is 42 on CE
- `winuser.h`: DLGWINDOWEXTRA is 32 on CE
- `winuser.h`: WM_MOUSELAST is WM_MBUTTONDBLCLK on CE
- `winuser.h`: QS_ALLEVENTS and QS_ALLINPUT have CE's QS_ bits
- `winuser.h`: SW_MAX is 10 on CE
- `winuser.h`: WS_OVERLAPPED has a border and caption on CE
- `winuser.h`: ACCEL has a pad word on CE
- `winuser.h`: INPUT and its parts are declared on CE
- `winuser.h`: SendInput is declared on CE
- `winuser.h`: HARDWAREINPUT has dwExtraInfo on CE
- `mmsystem.h`: MAXERRORLENGTH is 128 on CE
- `mmsystem.h`: PATCHARRAY and KEYARRAY aren't declared on CE
- `wingdi.h`: GetTextExtentPointW and GetTextExtentPoint32W are macros on CE 1.0 too
- `winnetwk.h`: RESOURCEUSAGE_ALL has no RESOURCEUSAGE_ATTACHED on CE
- `winnt.h`: REG_LEGAL_OPTION is 7 on CE
- `wingdi.h`: DEVMODEW has no dmDisplayOrientation on CE 1.0 and 2.0
- `commctrl.h`: TB_SETTOOLTIPS is WM_USER+81 on CE
- `commctrl.h`: RB_GETBANDINFO is RB_GETBANDINFOW (WM_USER+28) on CE
- `commctrl.h`: TB_INSERTBUTTON and TB_ADDBUTTONS are the W messages on CE 2.0
- `commctrl.h`: DTM_SETFORMATW is DTM_FIRST+50 (w32api had 0x1050)
- `commctrl.h`: CDRF_SKIPDEFAULT is 1 on CE 1.0
- `commctrl.h`: DTN_LAST is -769 on CE 1.0
- `commctrl.h`: MCS_NOTODAY is 0x10 on CE 2.0
- `commctrl.h`: NMCUSTOMDRAW has no lItemlParam on CE 1.0
- `commctrl.h`: NMLVCUSTOMDRAW has iSubItem on CE 2.0
- `commctrl.h`: REBARBANDINFOW has the IE 4.0 fields on CE 2.0
- `prsht.h`: PROPSHEETPAGEA has no IE 4.0 fields on CE
- `prsht.h`: PROPSHEETPAGEW has no IE 4.0 fields on CE
- `prsht.h`: PROPSHEETHEADERA has no IE 4.0 fields on CE
- `prsht.h`: PROPSHEETHEADERW has no IE 4.0 fields on CE
- `commctrl.h`: TVINSERTSTRUCTA has no itemex on CE
- `commctrl.h`: TVINSERTSTRUCTW has no itemex on CE
- `commctrl.h`: NMTVCUSTOMDRAW has no iLevel on CE
- `commctrl.h`: NMMOUSE has no dwHitInfo on CE
- `commctrl.h`: NMREBAR is CE's layout
- `commctrl.h`: SBN_LAST is -900 on CE
- `commdlg.h`: PD_ flags are CE's
- `commdlg.h`: PRINTDLGW is CE's print dialog structure
