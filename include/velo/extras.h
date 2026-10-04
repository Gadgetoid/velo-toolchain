#ifndef VELO_EXTRAS_H
#define VELO_EXTRAS_H

#ifndef IDC_STATIC
#define IDC_STATIC (-1)
#endif

#define SYS_COLOR_INDEX_FLAG 0x40000000
#define COLOR_INDEX_MASK SYS_COLOR_INDEX_FLAG

#ifndef RC_INVOKED

BOOL WINAPI CheckPassword(LPWSTR lpszPassword);
HANDLE WINAPI GetClipboardDataAlloc(UINT uFormat);
void WINAPI GetCurrentFT(LPFILETIME lpFileTime);
LONG WINAPI InterlockedTestExchange(LPLONG Target, LONG oldValue, LONG newValue);
DWORD WINAPI Random(void);
void WINAPI SHAddToRecentDocs(UINT uFlags, LPCVOID pv);
int WINAPI SHShowOutOfMemory(HWND hwndOwner, UINT grfFlags);

void WINAPI BatteryNotifyOfTimeChange(BOOL fForward, FILETIME *pftDelta);
BOOL WINAPI DeleteAndRenameFile(LPCWSTR lpszDestFile, LPCWSTR lpszSourceFile);
BOOL WINAPI EnableHardwareKeyboard(BOOL fEnable);
DWORD WINAPI GetKeyboardStatus(void);
UINT WINAPI GetMessageSource(void);
BOOL WINAPI GetMouseMovePoints(PPOINT pptBuf, UINT nBufPoints, UINT *pnPointsRetrieved);
HANDLE WINAPI GetOwnerProcess(void);
BOOL WINAPI LoadFSD(HANDLE hDevice, LPCWSTR lpFSDName);
LPVOID WINAPI MapPtrToProcess(LPVOID lpv, HANDLE hProc);
LPVOID WINAPI MapUncompressedFileW(LPCWSTR pwszFileName, LPDWORD pLen);
BOOL WINAPI PostKeybdMessage(HWND hwnd, UINT VKey, UINT KeyStateFlags, UINT cCharacters, UINT *pShiftStateBuffer, UINT *pCharacterBuffer);
BOOL WINAPI SetACP(UINT uiACP);
BOOL WINAPI SetOEMCP(UINT uiOEMCP);
BOOL WINAPI SetSystemDefaultLCID(LCID Locale);
void WINAPI SetDaylightTime(DWORD dst);
void WINAPI SignalStarted(DWORD dw);
BOOL WINAPI TransparentImage(HDC hdcDest, int nXDest, int nYDest, int nWidthDest, int nHeightDest, HANDLE hImgSrc, int nXSrc, int nYSrc,
                             int nWidthSrc, int nHeightSrc, COLORREF crTransparentColor);

#endif

#endif
