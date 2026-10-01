#ifndef VELO_WINDOWS_H
#define VELO_WINDOWS_H

#include <stdint.h>
#include <stddef.h>

#define WINAPI
#define CALLBACK

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
typedef uint32_t WPARAM;
typedef int32_t LPARAM;
typedef int32_t LRESULT;
typedef uint32_t COLORREF;
typedef LRESULT (CALLBACK *WNDPROC)(HWND, UINT, WPARAM, LPARAM);

#define TRUE 1
#define FALSE 0
#define MAX_PATH 260
#define INVALID_HANDLE_VALUE ((HANDLE)-1)
#define RGB(r, g, b) ((COLORREF)((r) | ((g) << 8) | ((b) << 16)))

typedef struct {
    LONG left, top, right, bottom;
} RECT;

typedef struct {
    LONG x, y;
} POINT;

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

#define WS_POPUP 0x80000000
#define WS_CHILD 0x40000000
#define WS_VISIBLE 0x10000000
#define WS_CAPTION 0x00C00000
#define WS_SYSMENU 0x00080000
#define WS_BORDER 0x00800000

#define WM_CREATE 0x0001
#define WM_DESTROY 0x0002
#define WM_SIZE 0x0005
#define WM_SETFOCUS 0x0007
#define WM_KILLFOCUS 0x0008
#define WM_PAINT 0x000F
#define WM_CLOSE 0x0010
#define WM_QUIT 0x0012
#define WM_ERASEBKGND 0x0014
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
#define MAKEINTRESOURCEW(id) ((LPWSTR)(DWORD)(WORD)(id))

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

#define SND_SYNC 0x0000
#define SND_ASYNC 0x0001
#define SND_NODEFAULT 0x0002
#define SND_MEMORY 0x0004

BOOL WINAPI sndPlaySoundW(LPCWSTR sound, UINT flags);

typedef HANDLE HKEY;

#define HKEY_CLASSES_ROOT ((HKEY)0x80000000u)
#define HKEY_CURRENT_USER ((HKEY)0x80000001u)
#define HKEY_LOCAL_MACHINE ((HKEY)0x80000002u)
#define HKEY_USERS ((HKEY)0x80000003u)
#define REG_SZ 1
#define REG_BINARY 3
#define REG_DWORD 4
#define ERROR_SUCCESS 0
#define ERROR_MORE_DATA 234

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

#define BI_RGB 0
#define DIB_RGB_COLORS 0
#define SRCCOPY 0x00CC0020

HBITMAP WINAPI CreateDIBSection(HDC dc, const void *info, UINT usage, void **bits, HANDLE section, DWORD offset);
HDC WINAPI CreateCompatibleDC(HDC dc);
BOOL WINAPI DeleteDC(HDC dc);
BOOL WINAPI BitBlt(HDC destination, int x, int y, int width, int height, HDC source, int source_x, int source_y, DWORD operation);
HWND WINAPI SetCapture(HWND window);
BOOL WINAPI ReleaseCapture(void);

#endif
