#ifndef VELO_WINDOWS_H
#define VELO_WINDOWS_H

#define WINAPI
#define CALLBACK
#define TRUE 1
#define FALSE 0
#define MAX_PATH 260

#ifndef RC_INVOKED
#include <stdint.h>
#include <stddef.h>

typedef int BOOL;
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint32_t UINT;
typedef int32_t LONG;
typedef uint16_t WCHAR;
typedef WCHAR *LPWSTR;
typedef const WCHAR *LPCWSTR;
typedef void *HANDLE;
typedef HANDLE HINSTANCE;
typedef HANDLE HWND;
typedef HANDLE HDC;
typedef HANDLE HGDIOBJ;
typedef HANDLE HFONT;
typedef HANDLE HBRUSH;
typedef HANDLE HICON;
typedef HANDLE HCURSOR;
typedef HANDLE HMENU;
typedef HANDLE HMODULE;
typedef HANDLE HBITMAP;
typedef HANDLE HPEN;
typedef HANDLE HRGN;
typedef HANDLE HACCEL;
typedef HANDLE HRSRC;
typedef HANDLE HGLOBAL;
typedef uint32_t WPARAM;
typedef int32_t LPARAM;
typedef int32_t LRESULT;
typedef uint32_t COLORREF;
typedef LRESULT (CALLBACK *WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef BOOL (CALLBACK *DLGPROC)(HWND, UINT, WPARAM, LPARAM);
typedef DWORD (WINAPI *LPTHREAD_START_ROUTINE)(void *parameter);

#define INVALID_HANDLE_VALUE ((HANDLE)-1)
#define RGB(r, g, b) ((COLORREF)((r) | ((g) << 8) | ((b) << 16)))

typedef struct {
    LONG left, top, right, bottom;
} RECT;

typedef struct {
    LONG x, y;
} POINT;

typedef struct {
    LONG cx, cy;
} SIZE;

typedef struct {
    UINT cbSize;
    UINT fMask;
    int nMin;
    int nMax;
    UINT nPage;
    int nPos;
    int nTrackPos;
} SCROLLINFO;

typedef struct {
    UINT lopnStyle;
    POINT lopnWidth;
    COLORREF lopnColor;
} LOGPEN;

typedef struct {
    void *DebugInfo;
    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    DWORD SpinCount;
} CRITICAL_SECTION;

typedef struct {
    HWND hwnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD time;
    POINT pt;
} MSG;

typedef struct {
    UINT style;
    WNDPROC lpfnWndProc;
    int cbClsExtra;
    int cbWndExtra;
    HINSTANCE hInstance;
    HICON hIcon;
    HCURSOR hCursor;
    HBRUSH hbrBackground;
    LPCWSTR lpszMenuName;
    LPCWSTR lpszClassName;
} WNDCLASSW;

typedef struct {
    HDC hdc;
    BOOL fErase;
    RECT rcPaint;
    BOOL fRestore;
    BOOL fIncUpdate;
    BYTE rgbReserved[32];
} PAINTSTRUCT;

typedef struct {
    LONG lfHeight;
    LONG lfWidth;
    LONG lfEscapement;
    LONG lfOrientation;
    LONG lfWeight;
    BYTE lfItalic;
    BYTE lfUnderline;
    BYTE lfStrikeOut;
    BYTE lfCharSet;
    BYTE lfOutPrecision;
    BYTE lfClipPrecision;
    BYTE lfQuality;
    BYTE lfPitchAndFamily;
    WCHAR lfFaceName[32];
} LOGFONTW;

typedef struct {
    LONG tmHeight;
    LONG tmAscent;
    LONG tmDescent;
    LONG tmInternalLeading;
    LONG tmExternalLeading;
    LONG tmAveCharWidth;
    LONG tmMaxCharWidth;
    LONG tmWeight;
    LONG tmOverhang;
    LONG tmDigitizedAspectX;
    LONG tmDigitizedAspectY;
    WCHAR tmFirstChar;
    WCHAR tmLastChar;
    WCHAR tmDefaultChar;
    WCHAR tmBreakChar;
    BYTE tmItalic;
    BYTE tmUnderlined;
    BYTE tmStruckOut;
    BYTE tmPitchAndFamily;
    BYTE tmCharSet;
} TEXTMETRICW;

typedef struct {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    DWORD dwTotalPhys;
    DWORD dwAvailPhys;
    DWORD dwTotalPageFile;
    DWORD dwAvailPageFile;
    DWORD dwTotalVirtual;
    DWORD dwAvailVirtual;
} MEMORYSTATUS;

typedef struct {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME;

typedef struct {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME;

typedef struct {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwOID;
    WCHAR cFileName[MAX_PATH];
} WIN32_FIND_DATAW;
#endif

#define WS_POPUP 0x80000000
#define WS_CHILD 0x40000000
#define WS_VISIBLE 0x10000000
#define WS_CAPTION 0x00C00000
#define WS_SYSMENU 0x00080000
#define WS_BORDER 0x00800000
#define WS_DLGFRAME 0x00400000
#define WS_VSCROLL 0x00200000
#define WS_HSCROLL 0x00100000
#define WS_GROUP 0x00020000
#define WS_TABSTOP 0x00010000
#define WS_CLIPSIBLINGS 0x04000000
#define WS_CLIPCHILDREN 0x02000000
#define WS_DISABLED 0x08000000
#define WS_EX_CLIENTEDGE 0x00000200
#define WS_EX_CONTEXTHELP 0x00000400
#define WS_EX_CAPTIONOKBTN 0x80000000

#define DS_MODALFRAME 0x80
#define DS_SETFONT 0x40
#define DS_CENTER 0x0800
#define ES_LEFT 0x0000
#define ES_MULTILINE 0x0004
#define ES_PASSWORD 0x0020
#define ES_AUTOVSCROLL 0x0040
#define ES_AUTOHSCROLL 0x0080
#define ES_WANTRETURN 0x1000
#define BS_PUSHBUTTON 0x00
#define BS_DEFPUSHBUTTON 0x01
#define BS_AUTOCHECKBOX 0x03
#define SS_LEFT 0x00
#define SS_CENTER 0x01
#define SS_RIGHT 0x02
#define SS_ICON 0x03
#define SS_NOPREFIX 0x80
#define BM_GETCHECK 0x00F0
#define BM_SETCHECK 0x00F1
#define BST_UNCHECKED 0
#define BST_CHECKED 1
#define CBS_DROPDOWNLIST 0x0003
#define CB_ADDSTRING 0x0143
#define CB_GETCURSEL 0x0147
#define CB_RESETCONTENT 0x014B
#define CB_SETCURSEL 0x014E
#define CB_SHOWDROPDOWN 0x014F
#define CB_GETITEMDATA 0x0150
#define CB_SETITEMDATA 0x0151
#define CBN_SELCHANGE 1
#define EM_SETSEL 0x00B1
#define EM_LIMITTEXT 0x00C5
#define EN_CHANGE 0x0300
#define BN_CLICKED 0

#define WM_CREATE 0x0001
#define WM_DESTROY 0x0002
#define WM_SIZE 0x0005
#define WM_SETFOCUS 0x0007
#define WM_KILLFOCUS 0x0008
#define WM_PAINT 0x000F
#define WM_CLOSE 0x0010
#define WM_QUIT 0x0012
#define WM_ERASEBKGND 0x0014
#define WM_SETTEXT 0x000C
#define WM_GETTEXT 0x000D
#define WM_GETTEXTLENGTH 0x000E
#define WM_ACTIVATE 0x0006
#define WM_SETFONT 0x0030
#define WM_SETICON 0x0080
#define ICON_SMALL 0
#define ICON_BIG 1
#define IMAGE_BITMAP 0
#define IMAGE_ICON 1
#define WM_GETFONT 0x0031
#define WM_NOTIFY 0x004E
#define WM_HELP 0x0053
#define WM_INITDIALOG 0x0110
#define WM_COMMAND 0x0111
#define WM_TIMER 0x0113
#define WM_HSCROLL 0x0114
#define WM_VSCROLL 0x0115
#define WM_INITMENUPOPUP 0x0117
#define WM_LBUTTONDBLCLK 0x0203
#define WM_USER 0x0400
#define WM_APP 0x8000
#define WM_KEYDOWN 0x0100
#define WM_CHAR 0x0102
#define WM_MOUSEMOVE 0x0200
#define WM_LBUTTONDOWN 0x0201
#define WM_LBUTTONUP 0x0202

#define VK_BACK 0x08
#define VK_TAB 0x09
#define VK_RETURN 0x0D
#define VK_ESCAPE 0x1B
#define VK_PRIOR 0x21
#define VK_NEXT 0x22
#define VK_END 0x23
#define VK_HOME 0x24
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_DELETE 0x2E
#define VK_F1 0x70
#define VK_F12 0x7B
#define VK_F2 0x71
#define VK_F5 0x74
#define VK_SPACE 0x20
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11

#define SB_LINEUP 0
#define SB_LINEDOWN 1
#define SB_PAGEUP 2
#define SB_PAGEDOWN 3
#define SB_THUMBPOSITION 4
#define SB_THUMBTRACK 5
#define SB_TOP 6
#define SB_BOTTOM 7
#define SB_ENDSCROLL 8
#define SB_HORZ 0
#define SB_VERT 1
#define SB_CTL 2
#define SIF_RANGE 0x0001
#define SIF_PAGE 0x0002
#define SIF_POS 0x0004
#define SIF_TRACKPOS 0x0010
#define SIF_ALL 0x0017

#define MF_STRING 0x0000
#define MF_ENABLED 0x0000
#define MF_UNCHECKED 0x0000
#define MF_BYCOMMAND 0x0000
#define MF_GRAYED 0x0001
#define MF_CHECKED 0x0008
#define MF_POPUP 0x0010
#define MF_SEPARATOR 0x0800

#define RT_BITMAP MAKEINTRESOURCEW(2)
#define RT_MENU MAKEINTRESOURCEW(4)
#define RT_DIALOG MAKEINTRESOURCEW(5)
#define RT_STRING MAKEINTRESOURCEW(6)
#define RT_ACCELERATOR MAKEINTRESOURCEW(9)

#define FVIRTKEY 0x01
#define FNOINVERT 0x02
#define FSHIFT 0x04
#define FCONTROL 0x08
#define FALT 0x10

#define SYS_COLOR_INDEX_FLAG 0x40000000
#define COLOR_SCROLLBAR (0 | SYS_COLOR_INDEX_FLAG)
#define COLOR_BACKGROUND (1 | SYS_COLOR_INDEX_FLAG)
#define COLOR_ACTIVECAPTION (2 | SYS_COLOR_INDEX_FLAG)
#define COLOR_WINDOW (5 | SYS_COLOR_INDEX_FLAG)
#define COLOR_WINDOWFRAME (6 | SYS_COLOR_INDEX_FLAG)
#define COLOR_WINDOWTEXT (8 | SYS_COLOR_INDEX_FLAG)
#define COLOR_HIGHLIGHT (13 | SYS_COLOR_INDEX_FLAG)
#define COLOR_HIGHLIGHTTEXT (14 | SYS_COLOR_INDEX_FLAG)
#define COLOR_BTNFACE (15 | SYS_COLOR_INDEX_FLAG)
#define COLOR_BTNSHADOW (16 | SYS_COLOR_INDEX_FLAG)
#define COLOR_GRAYTEXT (17 | SYS_COLOR_INDEX_FLAG)
#define COLOR_BTNTEXT (18 | SYS_COLOR_INDEX_FLAG)
#define COLOR_BTNHIGHLIGHT (20 | SYS_COLOR_INDEX_FLAG)

#define BDR_RAISEDOUTER 0x0001
#define BDR_SUNKENOUTER 0x0002
#define BDR_RAISEDINNER 0x0004
#define BDR_SUNKENINNER 0x0008
#define EDGE_RAISED (BDR_RAISEDOUTER | BDR_RAISEDINNER)
#define EDGE_SUNKEN (BDR_SUNKENOUTER | BDR_SUNKENINNER)
#define EDGE_ETCHED (BDR_SUNKENOUTER | BDR_RAISEDINNER)
#define BF_LEFT 0x0001
#define BF_TOP 0x0002
#define BF_RIGHT 0x0004
#define BF_BOTTOM 0x0008
#define BF_RECT (BF_LEFT | BF_TOP | BF_RIGHT | BF_BOTTOM)

#define PS_SOLID 0
#define PS_DASH 1
#define PS_NULL 5
#define WHITE_PEN 6
#define SYSTEM_FONT 13
#define FW_BOLD 700
#define DT_RIGHT 0x0002
#define DT_BOTTOM 0x0008
#define DT_EXPANDTABS 0x0040
#define DT_NOCLIP 0x0100
#define DT_CALCRECT 0x0400
#define DT_NOPREFIX 0x0800
#define DT_END_ELLIPSIS 0x8000
#define PATCOPY 0x00F00021
#define DSTINVERT 0x00550009
#define SW_HIDE 0
#define SW_SHOWNORMAL 1
#define SWP_NOSIZE 0x0001
#define SWP_NOMOVE 0x0002
#define SWP_NOZORDER 0x0004
#define SWP_NOACTIVATE 0x0010
#define SW_SCROLLCHILDREN 0x0001
#define SW_INVALIDATE 0x0002
#define SW_ERASE 0x0004
#define GWL_STYLE (-16)
#define GWL_USERDATA (-21)
#define DWL_MSGRESULT 0
#define DWL_USER 8
#define INFINITE 0xFFFFFFFF
#define WAIT_OBJECT_0 0
#define WAIT_TIMEOUT 258
#define EVENT_PULSE 1
#define EVENT_RESET 2
#define EVENT_SET 3
#define CREATE_SUSPENDED 0x00000004
#define THREAD_PRIORITY_NORMAL 0
#define THREAD_PRIORITY_BELOW_NORMAL 4
#define LMEM_MOVEABLE 0x0002
#define LMEM_ZEROINIT 0x0040
#define LPTR (LMEM_FIXED | LMEM_ZEROINIT)
#define IDC_STATIC (-1)

#define PM_REMOVE 0x0001
#define SW_SHOW 5
#define SM_CXSCREEN 0
#define SM_CYSCREEN 1
#define WHITE_BRUSH 0
#define BLACK_BRUSH 4
#define BLACK_PEN 7
#define DT_CENTER 0x0001
#define DT_VCENTER 0x0004
#define DT_SINGLELINE 0x0020
#define ETO_OPAQUE 0x0002
#define FW_NORMAL 400
#define FIXED_PITCH 1
#define FF_MODERN 0x30
#define LMEM_FIXED 0x0000
#define MEM_COMMIT 0x1000
#define MEM_RESERVE 0x2000
#define PAGE_READWRITE 0x04
#define MB_OK 0x0
#define MB_OKCANCEL 0x1
#define MB_YESNO 0x4
#define MB_ICONERROR 0x10
#define MB_ICONQUESTION 0x20
#define MB_ICONEXCLAMATION 0x30
#define MB_ICONINFORMATION 0x40
#define IDOK 1
#define IDCANCEL 2
#define IDYES 6
#define IDNO 7
#define CW_USEDEFAULT ((int)0x80000000)
#define TRANSPARENT 1
#define OPAQUE 2
#define NULL_BRUSH 5
#define GRAY_BRUSH 2
#define DT_LEFT 0x0000
#define DT_WORDBREAK 0x0010
#define TEXT(text) L##text
#define LOWORD(value) ((WORD)((DWORD)(value) & 0xFFFF))
#define HIWORD(value) ((WORD)(((DWORD)(value) >> 16) & 0xFFFF))
#ifdef RC_INVOKED
#define MAKEINTRESOURCEW(id) id
#else
#define MAKEINTRESOURCEW(id) ((LPWSTR)(DWORD)(WORD)(id))
#endif
#define MAKELONG(low, high) ((LONG)(((WORD)(low)) | ((DWORD)((WORD)(high))) << 16))
#define MAKEWPARAM(low, high) ((WPARAM)MAKELONG(low, high))
#define MAKELPARAM(low, high) ((LPARAM)MAKELONG(low, high))

#define GENERIC_READ 0x80000000
#define GENERIC_WRITE 0x40000000
#define FILE_SHARE_READ 0x00000001
#define CREATE_NEW 1
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3
#define OPEN_ALWAYS 4
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#define FILE_ATTRIBUTE_NORMAL 0x00000080
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#define ERROR_FILE_NOT_FOUND 2
#define ERROR_PATH_NOT_FOUND 3
#define ERROR_ACCESS_DENIED 5
#define ERROR_FILE_EXISTS 80
#define ERROR_DISK_FULL 112
#define ERROR_ALREADY_EXISTS 183
#define FILE_BEGIN 0
#define FILE_CURRENT 1
#define FILE_END 2

#ifndef RC_INVOKED
void WINAPI ExitThread(DWORD exit_code);
int WINAPI MessageBoxW(HWND window, LPCWSTR text, LPCWSTR caption, UINT type);
DWORD WINAPI GetTickCount(void);
void WINAPI Sleep(DWORD milliseconds);
void *WINAPI LocalAlloc(UINT flags, UINT bytes);
void *WINAPI LocalFree(void *memory);
void *WINAPI VirtualAlloc(void *address, DWORD size, DWORD type, DWORD protection);
void WINAPI GlobalMemoryStatus(MEMORYSTATUS *status);
DWORD WINAPI GetLastError(void);
HMODULE WINAPI LoadLibraryW(LPCWSTR name);
#if _WIN32_WCE >= 200
DWORD WINAPI GetModuleFileNameW(HMODULE module, LPWSTR name, DWORD size);
#endif
int WINAPI wsprintfW(LPWSTR output, LPCWSTR format, ...);
BOOL WINAPI FreeLibrary(HMODULE module);
void *WINAPI GetProcAddressW(HMODULE module, LPCWSTR name);

WORD WINAPI RegisterClassW(const WNDCLASSW *window_class);
HWND WINAPI CreateWindowExW(DWORD extended_style, LPCWSTR class_name, LPCWSTR window_name, DWORD style, int x, int y, int width, int height, HWND parent, HMENU menu, HINSTANCE instance, void *parameter);
BOOL WINAPI DestroyWindow(HWND window);
BOOL WINAPI ShowWindow(HWND window, int show);
BOOL WINAPI UpdateWindow(HWND window);
HWND WINAPI SetFocus(HWND window);
BOOL WINAPI GetClientRect(HWND window, RECT *rect);
BOOL WINAPI InvalidateRect(HWND window, const RECT *rect, BOOL erase);
LRESULT WINAPI DefWindowProcW(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
BOOL WINAPI GetMessageW(MSG *message, HWND window, UINT first, UINT last);
BOOL WINAPI PeekMessageW(MSG *message, HWND window, UINT first, UINT last, UINT remove);
BOOL WINAPI TranslateMessage(const MSG *message);
LRESULT WINAPI DispatchMessageW(const MSG *message);
void WINAPI PostQuitMessage(int exit_code);
int WINAPI GetSystemMetrics(int index);
short WINAPI GetKeyState(int key);

HDC WINAPI BeginPaint(HWND window, PAINTSTRUCT *paint);
BOOL WINAPI EndPaint(HWND window, const PAINTSTRUCT *paint);
HDC WINAPI GetDC(HWND window);
int WINAPI ReleaseDC(HWND window, HDC dc);
HGDIOBJ WINAPI GetStockObject(int object);
HGDIOBJ WINAPI SelectObject(HDC dc, HGDIOBJ object);
BOOL WINAPI DeleteObject(HGDIOBJ object);
HFONT WINAPI CreateFontIndirectW(const LOGFONTW *font);
int WINAPI GetObjectW(HGDIOBJ object, int size, void *buffer);
BOOL WINAPI GetTextMetricsW(HDC dc, TEXTMETRICW *metrics);
COLORREF WINAPI SetTextColor(HDC dc, COLORREF color);
COLORREF WINAPI SetBkColor(HDC dc, COLORREF color);
BOOL WINAPI ExtTextOutW(HDC dc, int x, int y, UINT options, const RECT *rect, LPCWSTR text, UINT count, const int *spacing);
int WINAPI FillRect(HDC dc, const RECT *rect, HBRUSH brush);
BOOL WINAPI Rectangle(HDC dc, int left, int top, int right, int bottom);
BOOL WINAPI Ellipse(HDC dc, int left, int top, int right, int bottom);
int WINAPI SetBkMode(HDC dc, int mode);
int WINAPI DrawTextW(HDC dc, LPCWSTR text, int count, RECT *rect, UINT format);
BOOL WINAPI InflateRect(RECT *rect, int dx, int dy);
BOOL WINAPI SetRect(RECT *rect, int left, int top, int right, int bottom);

HANDLE WINAPI CreateFileW(LPCWSTR name, DWORD access, DWORD share, void *security, DWORD disposition, DWORD flags, HANDLE template_file);
BOOL WINAPI ReadFile(HANDLE file, void *buffer, DWORD length, DWORD *read, void *overlapped);
BOOL WINAPI WriteFile(HANDLE file, const void *buffer, DWORD length, DWORD *written, void *overlapped);
BOOL WINAPI CloseHandle(HANDLE handle);
DWORD WINAPI GetFileAttributesW(LPCWSTR name);
DWORD WINAPI GetFileSize(HANDLE file, DWORD *high);
DWORD WINAPI SetFilePointer(HANDLE file, LONG distance, LONG *high, DWORD method);
void WINAPI GetLocalTime(SYSTEMTIME *time);
void WINAPI GetSystemTime(SYSTEMTIME *time);
HANDLE WINAPI FindFirstFileW(LPCWSTR pattern, WIN32_FIND_DATAW *data);
BOOL WINAPI FindNextFileW(HANDLE search, WIN32_FIND_DATAW *data);
BOOL WINAPI FindClose(HANDLE search);
BOOL WINAPI CreateDirectoryW(LPCWSTR name, void *security);
BOOL WINAPI RemoveDirectoryW(LPCWSTR name);
BOOL WINAPI DeleteFileW(LPCWSTR name);
BOOL WINAPI MoveFileW(LPCWSTR from, LPCWSTR to);
size_t WINAPI wcslen(LPCWSTR text);
void *WINAPI LocalReAlloc(void *memory, UINT bytes, UINT flags);
UINT WINAPI LocalSize(void *memory);

HANDLE WINAPI CreateThread(void *security, DWORD stack_size, LPTHREAD_START_ROUTINE start, void *parameter, DWORD flags, DWORD *thread_id);
BOOL WINAPI SetThreadPriority(HANDLE thread, int priority);
HANDLE WINAPI CreateEventW(void *security, BOOL manual_reset, BOOL initial_state, LPCWSTR name);
BOOL WINAPI EventModify(HANDLE event, DWORD function);
#define SetEvent(event) EventModify(event, EVENT_SET)
#define ResetEvent(event) EventModify(event, EVENT_RESET)
DWORD WINAPI WaitForSingleObject(HANDLE handle, DWORD milliseconds);
void WINAPI InitializeCriticalSection(CRITICAL_SECTION *section);
void WINAPI DeleteCriticalSection(CRITICAL_SECTION *section);
void WINAPI EnterCriticalSection(CRITICAL_SECTION *section);
void WINAPI LeaveCriticalSection(CRITICAL_SECTION *section);
LONG WINAPI InterlockedIncrement(LONG *value);
LONG WINAPI InterlockedDecrement(LONG *value);
LONG WINAPI InterlockedExchange(LONG *target, LONG value);

HRSRC WINAPI FindResource(HMODULE module, LPCWSTR name, LPCWSTR type);
HGLOBAL WINAPI LoadResource(HMODULE module, HRSRC resource);
DWORD WINAPI SizeofResource(HMODULE module, HRSRC resource);
#define LockResource(resource) ((void *)(resource))
HICON WINAPI LoadIconW(HINSTANCE instance, LPCWSTR name);
HANDLE WINAPI LoadImageW(HINSTANCE instance, LPCWSTR name, UINT type, int width, int height, UINT flags);
HBITMAP WINAPI LoadBitmapW(HINSTANCE instance, LPCWSTR name);
int WINAPI LoadStringW(HINSTANCE instance, UINT id, LPWSTR buffer, int size);
HACCEL WINAPI LoadAcceleratorsW(HINSTANCE instance, LPCWSTR name);
int WINAPI TranslateAcceleratorW(HWND window, HACCEL accelerators, MSG *message);
int WINAPI DialogBoxIndirectParamW(HINSTANCE instance, const void *dialog_template, HWND parent, DLGPROC procedure, LPARAM parameter);
HWND WINAPI CreateDialogIndirectParamW(HINSTANCE instance, const void *dialog_template, HWND parent, DLGPROC procedure, LPARAM parameter);
BOOL WINAPI EndDialog(HWND dialog, int result);
BOOL WINAPI IsDialogMessageW(HWND dialog, MSG *message);
HWND WINAPI GetDlgItem(HWND dialog, int id);

LRESULT WINAPI SendMessageW(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
BOOL WINAPI PostMessageW(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
int WINAPI GetWindowTextW(HWND window, LPWSTR text, int size);
int WINAPI GetWindowTextLengthW(HWND window);
BOOL WINAPI SetWindowTextW(HWND window, LPCWSTR text);
LONG WINAPI GetWindowLongW(HWND window, int index);
LONG WINAPI SetWindowLongW(HWND window, int index, LONG value);
BOOL WINAPI MoveWindow(HWND window, int x, int y, int width, int height, BOOL repaint);
BOOL WINAPI SetWindowPos(HWND window, HWND after, int x, int y, int width, int height, UINT flags);
BOOL WINAPI GetWindowRect(HWND window, RECT *rect);
BOOL WINAPI EnableWindow(HWND window, BOOL enable);
BOOL WINAPI IsWindowVisible(HWND window);
HWND WINAPI GetFocus(void);
HWND WINAPI GetParent(HWND window);
BOOL WINAPI SetForegroundWindow(HWND window);
HWND WINAPI FindWindowW(LPCWSTR class_name, LPCWSTR window_name);
BOOL WINAPI ScreenToClient(HWND window, POINT *point);
BOOL WINAPI ClientToScreen(HWND window, POINT *point);
HWND WINAPI GetCapture(void);
BOOL WINAPI MessageBeep(UINT type);
UINT WINAPI SetTimer(HWND window, UINT id, UINT milliseconds, void *callback);
BOOL WINAPI KillTimer(HWND window, UINT id);
int WINAPI SetScrollInfo(HWND window, int bar, const SCROLLINFO *info, BOOL redraw);
BOOL WINAPI GetScrollInfo(HWND window, int bar, SCROLLINFO *info);
int WINAPI ScrollWindowEx(HWND window, int dx, int dy, const RECT *scroll, const RECT *clip, HRGN update, RECT *updated, UINT flags);
HCURSOR WINAPI SetCursor(HCURSOR cursor);

HMENU WINAPI CreateMenu(void);
HMENU WINAPI CreatePopupMenu(void);
BOOL WINAPI AppendMenuW(HMENU menu, UINT flags, UINT id, LPCWSTR text);
DWORD WINAPI CheckMenuItem(HMENU menu, UINT id, UINT check);
BOOL WINAPI EnableMenuItem(HMENU menu, UINT id, UINT enable);
HMENU WINAPI GetSubMenu(HMENU menu, int position);
BOOL WINAPI DestroyMenu(HMENU menu);
BOOL WINAPI TrackPopupMenuEx(HMENU menu, UINT flags, int x, int y, HWND window, void *parameters);

DWORD WINAPI GetSysColor(int index);
HBRUSH WINAPI GetSysColorBrush(int index);
HBRUSH WINAPI CreateSolidBrush(COLORREF color);
HPEN WINAPI CreatePenIndirect(const LOGPEN *pen);
BOOL WINAPI Polyline(HDC dc, const POINT *points, int count);
BOOL WINAPI DrawEdge(HDC dc, RECT *rect, UINT edge, UINT flags);
BOOL WINAPI DrawFocusRect(HDC dc, const RECT *rect);
BOOL WINAPI PatBlt(HDC dc, int x, int y, int width, int height, DWORD operation);
BOOL WINAPI GetTextExtentExPointW(HDC dc, LPCWSTR text, int count, int maximum, int *fit, int *widths, SIZE *size);
int WINAPI IntersectClipRect(HDC dc, int left, int top, int right, int bottom);
int WINAPI SelectClipRgn(HDC dc, HRGN region);
BOOL WINAPI StretchBlt(HDC destination, int x, int y, int width, int height, HDC source, int source_x, int source_y, int source_width, int source_height, DWORD operation);
BOOL WINAPI DrawIconEx(HDC dc, int x, int y, HICON icon, int width, int height, UINT step, HBRUSH brush, UINT flags);
BOOL WINAPI SystemTimeToFileTime(const SYSTEMTIME *system_time, FILETIME *file_time);
BOOL WINAPI FileTimeToSystemTime(const FILETIME *file_time, SYSTEMTIME *system_time);
BOOL WINAPI FileTimeToLocalFileTime(const FILETIME *file_time, FILETIME *local_time);

#endif

#define SND_SYNC 0x0000
#define SND_ASYNC 0x0001
#define SND_NODEFAULT 0x0002
#define SND_MEMORY 0x0004

#ifndef RC_INVOKED
BOOL WINAPI sndPlaySoundW(LPCWSTR sound, UINT flags);

typedef HANDLE HKEY;
#endif

#define HKEY_CLASSES_ROOT ((HKEY)0x80000000u)
#define HKEY_CURRENT_USER ((HKEY)0x80000001u)
#define HKEY_LOCAL_MACHINE ((HKEY)0x80000002u)
#define HKEY_USERS ((HKEY)0x80000003u)
#define REG_SZ 1
#define REG_BINARY 3
#define REG_DWORD 4
#define ERROR_SUCCESS 0
#define ERROR_MORE_DATA 234
#define KEY_ALL_ACCESS 0xF003F
#define KEY_READ 0x20019

#ifndef RC_INVOKED
LONG WINAPI RegCreateKeyExW(HKEY key, LPCWSTR subkey, DWORD reserved, LPWSTR key_class, DWORD options, DWORD access, void *security, HKEY *result, DWORD *disposition);
LONG WINAPI RegOpenKeyExW(HKEY key, LPCWSTR subkey, DWORD options, DWORD access, HKEY *result);
LONG WINAPI RegCloseKey(HKEY key);
LONG WINAPI RegSetValueExW(HKEY key, LPCWSTR name, DWORD reserved, DWORD type, const BYTE *data, DWORD size);
LONG WINAPI RegQueryValueExW(HKEY key, LPCWSTR name, DWORD *reserved, DWORD *type, BYTE *data, DWORD *size);
LONG WINAPI RegDeleteKeyW(HKEY key, LPCWSTR subkey);
LONG WINAPI RegDeleteValueW(HKEY key, LPCWSTR name);

typedef struct {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

#endif

#define BI_RGB 0
#define DIB_RGB_COLORS 0
#define SRCCOPY 0x00CC0020

#ifndef RC_INVOKED
HBITMAP WINAPI CreateDIBSection(HDC dc, const void *info, UINT usage, void **bits, HANDLE section, DWORD offset);
HDC WINAPI CreateCompatibleDC(HDC dc);
BOOL WINAPI DeleteDC(HDC dc);
BOOL WINAPI BitBlt(HDC destination, int x, int y, int width, int height, HDC source, int source_x, int source_y, DWORD operation);
HWND WINAPI SetCapture(HWND window);
BOOL WINAPI ReleaseCapture(void);
#endif

#endif
