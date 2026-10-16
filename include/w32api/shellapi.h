#ifndef _SHELLAPI_H
#define _SHELLAPI_H
#if __GNUC__ >= 3
#pragma GCC system_header
#endif

#ifdef __cplusplus
extern "C" {
#endif
#define WINSHELLAPI DECLSPEC_IMPORT
#define ABE_LEFT	0
#define ABE_TOP	1
#define ABE_RIGHT	2
#define ABE_BOTTOM	3
#define ABS_AUTOHIDE	1
#define ABS_ALWAYSONTOP	2
#define SEE_MASK_CLASSNAME         0x1
#define SEE_MASK_CLASSKEY          0x3
#define SEE_MASK_IDLIST            0x4
#define SEE_MASK_INVOKEIDLIST      0xc
#define SEE_MASK_ICON              0x10
#define SEE_MASK_HOTKEY            0x20
#define SEE_MASK_NOCLOSEPROCESS    0x40
#define SEE_MASK_CONNECTNETDRV     0x80
#define SEE_MASK_FLAG_DDEWAIT      0x100
#define SEE_MASK_DOENVSUBST        0x200
#define SEE_MASK_FLAG_NO_UI        0x400
#define SEE_MASK_NO_CONSOLE        0x8000
#define SEE_MASK_UNICODE           0x10000
#define SEE_MASK_ASYNCOK           0x100000
#define SEE_MASK_HMONITOR          0x200000
#if (_WIN32_IE >= 0x0600)
#define SEE_MASK_NOZONECHECKS      0x800000
#define SEE_MASK_FLAG_LOG_USAGE    0x4000000
#endif
#define ABM_NEW	0
#define ABM_REMOVE	1
#define ABM_QUERYPOS	2
#define ABM_SETPOS	3
#define ABM_GETSTATE	4
#define ABM_GETTASKBARPOS	5
#define ABM_ACTIVATE	6
#define ABM_GETAUTOHIDEBAR	7
#define ABM_SETAUTOHIDEBAR	8
#define ABM_WINDOWPOSCHANGED	9
#define ABN_STATECHANGE		0
#define ABN_POSCHANGED		1
#define ABN_FULLSCREENAPP	2
#define ABN_WINDOWARRANGE	3
#define NIM_ADD	0
#define NIM_MODIFY	1
#define NIM_DELETE	2
#if (_WIN32_IE >= 0x0500)
#define NOTIFYICON_VERSION 3
#if (_WIN32_WINNT >= 0x600)
#define NOTIFYICON_VERSION_4  4
#endif
#define NIM_SETFOCUS	3
#define NIM_SETVERSION	4
#define NIN_POPUPOPEN WM_USER+6
#define NIN_POPUPCLOSE WM_USER+7
#endif
#define NIF_MESSAGE	0x00000001
#define NIF_ICON	0x00000002
#define NIF_TIP		0x00000004
#define NIF_STATE	0x00000008
#if (_WIN32_IE >= 0x0500)
#define NIF_INFO	0x00000010
#endif
#if (_WIN32_IE >= 0x0600)
#define NIF_GUID	0x00000020
#endif
#if (_WIN32_IE >= 0x0500)
#define NIIF_NONE	0x00000000
#define NIIF_INFO	0x00000001
#define NIIF_WARNING	0x00000002
#define NIIF_ERROR	0x00000003
#endif
#if (_WIN32_IE >= 0x0600)
#define NIIF_ICON_MASK	0x0000000F
#define NIIF_NOSOUND	0x00000010
#endif
#if (_WIN32_IE >= 0x0500)
#define NIS_HIDDEN	0x00000001
#define NIS_SHAREDICON	0x00000002
#endif
#define SE_ERR_FNF	2
#define SE_ERR_PNF	3
#define SE_ERR_ACCESSDENIED	5
#define SE_ERR_OOM	8
#define SE_ERR_DLLNOTFOUND	32
#define SE_ERR_SHARE	26
#define SE_ERR_ASSOCINCOMPLETE	27
#define SE_ERR_DDETIMEOUT	28
#define SE_ERR_DDEFAIL	29
#define SE_ERR_DDEBUSY	30
#define SE_ERR_NOASSOC	31
#define FO_MOVE	1
#define FO_COPY	2
#define FO_DELETE	3
#define FO_RENAME	4
#define FOF_MULTIDESTFILES	1
#define FOF_CONFIRMMOUSE	2
#define FOF_SILENT	4
#define FOF_RENAMEONCOLLISION	8
#define FOF_NOCONFIRMATION	16
#define FOF_WANTMAPPINGHANDLE	32
#define FOF_ALLOWUNDO	64
#define FOF_FILESONLY	128
#define FOF_SIMPLEPROGRESS	256
#define FOF_NOCONFIRMMKDIR	512
#define FOF_NOERRORUI	1024
#define FOF_NOCOPYSECURITYATTRIBS	2048
#define PO_DELETE 19
#define PO_RENAME 20
#define PO_PORTCHANGE 32
#define PO_REN_PORT 52
#define SHGFI_ICON	256
#define SHGFI_DISPLAYNAME	512
#define SHGFI_TYPENAME	1024
#define SHGFI_ATTRIBUTES	2048
#define SHGFI_ICONLOCATION	4096
#define SHGFI_EXETYPE 8192
#define SHGFI_SYSICONINDEX 16384
#define SHGFI_LINKOVERLAY 32768
#define SHGFI_SELECTED 65536
#define SHGFI_ATTR_SPECIFIED 131072
#define SHGFI_LARGEICON	0
#define SHGFI_SMALLICON	1
#define SHGFI_OPENICON	2
#define SHGFI_SHELLICONSIZE	4
#define SHGFI_PIDL	8
#define SHGFI_USEFILEATTRIBUTES	16
#define SHERB_NOCONFIRMATION 1
#define SHERB_NOPROGRESSUI 2
#define SHERB_NOSOUND 4
#ifdef UNICODE
#define NOTIFYICONDATA_V2_SIZE 936
#define NOTIFYICONDATA_V1_SIZE 152
#else
#define NOTIFYICONDATA_V2_SIZE 488
#define NOTIFYICONDATA_V1_SIZE 88
#endif

typedef WORD FILEOP_FLAGS;
typedef WORD PRINTEROP_FLAGS;
#include <pshpack2.h>
typedef struct _AppBarData {
	DWORD	cbSize;
	HWND	hWnd;
	UINT	uCallbackMessage;
	UINT	uEdge;
	RECT	rc;
	LPARAM lParam;
} APPBARDATA,*PAPPBARDATA;
DECLARE_HANDLE(HDROP);

typedef struct _NOTIFYICONDATAA {
  DWORD cbSize;
  HWND hWnd;
  UINT uID;
  UINT uFlags;
  UINT uCallbackMessage;
  HICON hIcon;
#if _WIN32_IE >= 0x0500
  CHAR szTip[128];
  DWORD dwState;
  DWORD dwStateMask;
  CHAR szInfo[256];
  _ANONYMOUS_UNION union {
    UINT uTimeout;
    UINT uVersion;
  } DUMMYUNIONNAME;
  CHAR szInfoTitle[64];
  DWORD dwInfoFlags;
#else
  CHAR szTip[64];
#endif
#if _WIN32_IE >= 0x600
  GUID guidItem;
#endif
#if _WIN32_WINNT >= 0x600
  HICON hBalloonIcon;
#endif
} NOTIFYICONDATAA,*PNOTIFYICONDATAA;

typedef struct _NOTIFYICONDATAW {
  DWORD cbSize;
  HWND hWnd;
  UINT uID;
  UINT uFlags;
  UINT uCallbackMessage;
  HICON hIcon;
#if _WIN32_IE >= 0x0500
  WCHAR szTip[128];
  DWORD dwState;
  DWORD dwStateMask;
  WCHAR szInfo[256];
  _ANONYMOUS_UNION union {
    UINT uTimeout;
    UINT uVersion;
  } DUMMYUNIONNAME;
  WCHAR szInfoTitle[64];
  DWORD dwInfoFlags;
#else
  WCHAR szTip[64];
#endif
#if _WIN32_IE >= 0x600
  GUID guidItem;
#endif
#if _WIN32_WINNT >= 0x600
  HICON hBalloonIcon;
#endif
} NOTIFYICONDATAW,*PNOTIFYICONDATAW;

typedef struct _SHELLEXECUTEINFOA {
	DWORD cbSize;
	ULONG fMask;
	HWND hwnd;
	LPCSTR lpVerb;
	LPCSTR lpFile;
	LPCSTR lpParameters;
	LPCSTR lpDirectory;
	int nShow;
	HINSTANCE hInstApp;
	PVOID lpIDList;
	LPCSTR lpClass;
	HKEY hkeyClass;
	DWORD dwHotKey;
	HANDLE hIcon;
	HANDLE hProcess;
} SHELLEXECUTEINFOA,*LPSHELLEXECUTEINFOA;
typedef struct _SHELLEXECUTEINFOW {
	DWORD cbSize;
	ULONG fMask;
	HWND hwnd;
	LPCWSTR lpVerb;
	LPCWSTR lpFile;
	LPCWSTR lpParameters;
	LPCWSTR lpDirectory;
	int nShow;
	HINSTANCE hInstApp;
	PVOID lpIDList;
	LPCWSTR lpClass;
	HKEY hkeyClass;
	DWORD dwHotKey;
	HANDLE hIcon;
	HANDLE hProcess;
} SHELLEXECUTEINFOW,*LPSHELLEXECUTEINFOW;
typedef struct _SHFILEOPSTRUCTA {
	HWND hwnd;
	UINT wFunc;
	LPCSTR pFrom;
	LPCSTR pTo;
	FILEOP_FLAGS fFlags;
	BOOL fAnyOperationsAborted;
	PVOID hNameMappings;
	LPCSTR lpszProgressTitle;
} SHFILEOPSTRUCTA,*LPSHFILEOPSTRUCTA;
typedef struct _SHFILEOPSTRUCTW {
	HWND hwnd;
	UINT wFunc;
	LPCWSTR pFrom;
	LPCWSTR pTo;
	FILEOP_FLAGS fFlags;
	BOOL fAnyOperationsAborted;
	PVOID hNameMappings;
	LPCWSTR lpszProgressTitle;
} SHFILEOPSTRUCTW,*LPSHFILEOPSTRUCTW;
typedef struct _SHFILEINFOA {
	HICON hIcon;
	int iIcon;
	DWORD dwAttributes;
	CHAR szDisplayName[MAX_PATH];
	CHAR szTypeName[80];
} SHFILEINFOA;
typedef struct _SHFILEINFOW {
	HICON hIcon;
	int iIcon;
	DWORD dwAttributes;
	WCHAR szDisplayName[MAX_PATH];
	WCHAR szTypeName[80];
} SHFILEINFOW;
typedef struct _SHQUERYRBINFO {
	DWORD   cbSize;
	__int64 i64Size;
	__int64 i64NumItems;
} SHQUERYRBINFO, *LPSHQUERYRBINFO;
#include <poppack.h>

LPWSTR * WINAPI CommandLineToArgvW(LPCWSTR,int*);
void WINAPI DragAcceptFiles(HWND,BOOL);
void WINAPI DragFinish(HDROP);
UINT WINAPI DragQueryFileA(HDROP,UINT,LPSTR,UINT);
UINT WINAPI DragQueryFileW(HDROP,UINT,LPWSTR,UINT);
BOOL WINAPI DragQueryPoint(HDROP,LPPOINT);
HICON WINAPI DuplicateIcon(HINSTANCE,HICON);
HICON WINAPI ExtractAssociatedIconA(HINSTANCE,LPCSTR,PWORD);
HICON WINAPI ExtractAssociatedIconW(HINSTANCE,LPCWSTR,PWORD);
HICON WINAPI ExtractIconA(HINSTANCE,LPCSTR,UINT);
HICON WINAPI ExtractIconW(HINSTANCE,LPCWSTR,UINT);
UINT WINAPI ExtractIconExA(LPCSTR,int,HICON*,HICON*,UINT);
/**
 * Extracts an icon from an executable, DLL or icon file.
 *
 * Free the icons with DestroyIcon.
 *
 * @param lpszExeFileName Path of the file.
 * @param nIconIndex 0 for the first icon, or -N for the icon with
 *        resource ID N. Other indexes are not supported.
 * @param phiconLarge Receives the large icon, or NULL.
 * @param phiconSmall Receives the small icon, or NULL.
 * @param nIcons Must be 1.
 * @return Nonzero on success, 0 on failure.
 */
UINT WINAPI ExtractIconExW(LPCWSTR lpszExeFileName,int nIconIndex,HICON*phiconLarge,HICON*phiconSmall,UINT nIcons);
HINSTANCE WINAPI FindExecutableA(LPCSTR,LPCSTR,LPSTR);
HINSTANCE WINAPI FindExecutableW(LPCWSTR,LPCWSTR,LPWSTR);
UINT WINAPI SHAppBarMessage(DWORD,PAPPBARDATA);
BOOL WINAPI Shell_NotifyIconA(DWORD,PNOTIFYICONDATAA);
BOOL WINAPI _WNAME(Shell_NotifyIcon)(DWORD,PNOTIFYICONDATAW);
int WINAPI ShellAboutA(HWND,LPCSTR,LPCSTR,HICON);
int WINAPI ShellAboutW(HWND,LPCWSTR,LPCWSTR,HICON);
#ifndef _WIN32_WCE
HINSTANCE WINAPI ShellExecuteA(HWND,LPCSTR,LPCSTR,LPCSTR,LPCSTR,INT);
HINSTANCE WINAPI ShellExecuteW(HWND,LPCWSTR,LPCWSTR,LPCWSTR,LPCWSTR,INT);
#endif /* _WIN32_WCE */
BOOL WINAPI ShellExecuteExA(LPSHELLEXECUTEINFOA);
BOOL WINAPI _WNAME(ShellExecuteEx)(LPSHELLEXECUTEINFOW);
int WINAPI SHFileOperationA(LPSHFILEOPSTRUCTA);
/**
 * Copies, moves, renames or deletes files, with optional progress and
 * confirmation UI.
 *
 * Paths in pFrom and pTo are lists of null-terminated strings ending with
 * an extra null.
 *
 * @param lpFileOp SHFILEOPSTRUCT with the operation (FO_COPY, FO_MOVE,
 *        FO_RENAME or FO_DELETE), source and destination lists and
 *        FOF_* flags.
 * @return 0 on success, or nonzero on failure. fAnyOperationsAborted is
 *         set if the user cancelled.
 *
 * @note Windows CE 2.0 only.
 */
int WINAPI SHFileOperationW(LPSHFILEOPSTRUCTW lpFileOp);
void WINAPI SHFreeNameMappings(HANDLE);
DWORD WINAPI SHGetFileInfoA(LPCSTR,DWORD,SHFILEINFOA*,UINT,UINT);
DWORD WINAPI _WNAME(SHGetFileInfo)(LPCWSTR,DWORD,SHFILEINFOW*,UINT,UINT);
HRESULT WINAPI SHQueryRecycleBinA(LPCSTR, LPSHQUERYRBINFO);
HRESULT WINAPI SHQueryRecycleBinW(LPCWSTR, LPSHQUERYRBINFO);
HRESULT WINAPI SHEmptyRecycleBinA(HWND,LPCSTR,DWORD);
HRESULT WINAPI SHEmptyRecycleBinW(HWND,LPCWSTR,DWORD);

#ifdef _WIN32_WCE
/**
 * Returns the target path of a shortcut file.
 *
 * @param szShortcut Path of the shortcut.
 * @param szTarget Receives the target path.
 * @param cbMax Size of szTarget in characters.
 * @return TRUE on success, FALSE on failure.
 */
BOOL WINAPI SHGetShortcutTarget(LPCTSTR szShortcut,LPTSTR szTarget,int cbMax); 
/**
 * Creates a shortcut file pointing at a target path.
 *
 * @param szShortcut Path of the shortcut to create. Must not be NULL.
 * @param szTarget Target path, up to 256 characters.
 * @return TRUE on success, FALSE on failure.
 */
DWORD WINAPI SHCreateShortcut(LPTSTR szShortcut,LPTSTR szTarget);
/**
 * Loads a .bmp file into a bitmap.
 *
 * Free with DeleteObject.
 *
 * @param szFileName Full path of the file.
 * @return The bitmap, or NULL on failure.
 */
HBITMAP WINAPI SHLoadDIBitmap(LPCTSTR szFileName);
#if (_WIN32_WCE >= 0x300)
BOOL WINAPI SHGetSpecialFolderPath(HWND,LPWSTR,int,BOOL);
#endif
#if (_WIN32_WCE >= 0x420)
DWORD WINAPI SHCreateShortcutEx(LPTSTR,LPTSTR,LPTSTR,LPDWORD);
#endif
#endif

#if (_WIN32_WINNT >= 0x600)
#define SHIL_LARGE 0x0
#define SHIL_SMALL 0x1
#define SHIL_EXTRALARGE 0x2
#define SHIL_SYSSMALL 0x3
#define SHIL_JUMBO 0x4
#define SHIL_LAST SHIL_JUMBO
HRESULT WINAPI SHGetImageList(int,REFIID,void**);
#endif

#ifdef UNICODE
typedef NOTIFYICONDATAW NOTIFYICONDATA,*PNOTIFYICONDATA;
typedef SHELLEXECUTEINFOW SHELLEXECUTEINFO,*LPSHELLEXECUTEINFO;
typedef SHFILEOPSTRUCTW SHFILEOPSTRUCT,*LPSHFILEOPSTRUCT;
typedef SHFILEINFOW SHFILEINFO;
#define DragQueryFile DragQueryFileW
#define ExtractAssociatedIcon ExtractAssociatedIconW
#define ExtractIcon ExtractIconW
/**
 * Extracts an icon from an executable, DLL or icon file.
 *
 * Free the icons with DestroyIcon.
 *
 * @param lpszExeFileName Path of the file.
 * @param nIconIndex 0 for the first icon, or -N for the icon with
 *        resource ID N. Other indexes are not supported.
 * @param phiconLarge Receives the large icon, or NULL.
 * @param phiconSmall Receives the small icon, or NULL.
 * @param nIcons Must be 1.
 * @return Nonzero on success, 0 on failure.
 */
#define ExtractIconEx ExtractIconExW
#define FindExecutable FindExecutableW
/**
 * This function sends a message to the system to add, modify, or delete an
 * icon from the taskbar status area.
 *
 * @param dwMessage [in] Specifies the message value to send. It is one of
 *        these values: Value Description NIM_ADD Adds an icon to the
 *        status area. NIM_DELETE Deletes an icon from the status area.
 *        NIM_MODIFY Modifies an icon in the status area.
 * @param pnid [in] Pointer to a NOTIFYICONDATA structure. The content of
 *        the structure depends on the value of dwMessage.
 * @return Nonzero indicates success. Zero indicates failure.
 */
#define Shell_NotifyIcon _WNAME(Shell_NotifyIcon)
#define ShellAbout ShellAboutW
#define ShellExecute ShellExecuteW
/**
 * This function performs an action on a file. The file can be an
 * executable file or a document.
 *
 * If the function succeeds, it sets the hInstApp member of the
 * SHELLEXECUTEINFO structure to the instance handle to the application
 * that the function started. If the function fails, hInstApp is one of the
 * SE_ERR_ error values indicating the cause of the failure. (An instance
 * handle will always be greater than 32 and an error value less than 32.)
 * Note that the SE_ERR_ error values are for compatibility with the
 * ShellExecute function; use the GetLastError function to retrieve error
 * information.
 *
 * The error value returned by GetLastError corresponds to the SE_ERR_
 * values and is one of the values described in the following table.
 *
 * Value Description ERROR_FILE_NOT_FOUND The specified file was not found.
 * ERROR_PATH_NOT_FOUND The specified path was not found. ERROR_DDE_FAIL
 * The DDE transaction failed. ERROR_NO_ASSOCIATION There is no application
 * associated with the given file name extension. ERROR_ACCESS_DENIED
 * Access to the specified file is denied. ERROR_DLL_NOT_FOUND One of the
 * library files necessary to run the application can't be found.
 * ERROR_CANCELLED The function prompted the user for the location of the
 * application, but the user canceled the request. ERROR_NOT_ENOUGH_MEMORY
 * There is not enough memory to perform the specified action.
 * ERROR_SHARING_VIOLATION A sharing violation occurred.
 *
 * @param lpExecInfo Long pointer to a SHELLEXECUTEINFO structure that
 *        contains and receives information about the application being
 *        executed.
 * @return Nonzero indicates success. Zero indicates failure. To get
 *        extended error information, call GetLastError.
 */
#define ShellExecuteEx _WNAME(ShellExecuteEx)
/**
 * Copies, moves, renames or deletes files, with optional progress and
 * confirmation UI.
 *
 * Paths in pFrom and pTo are lists of null-terminated strings ending with
 * an extra null.
 *
 * @param lpFileOp SHFILEOPSTRUCT with the operation (FO_COPY, FO_MOVE,
 *        FO_RENAME or FO_DELETE), source and destination lists and
 *        FOF_* flags.
 * @return 0 on success, or nonzero on failure. fAnyOperationsAborted is
 *         set if the user cancelled.
 *
 * @note Windows CE 2.0 only.
 */
#define SHFileOperation SHFileOperationW
/**
 * This function retrieves information about an object in the file system,
 * such as a file, a folder, a directory, or a drive root.
 *
 * For Windows CE versions 2.10 and later, SHGetFileInfo does not support
 * the following values for uFlags:
 *
 * SHGFI_ATTRIBUTES
 *
 * SHGFI_EXETYPE
 *
 * SHGFI_ICONLOCATION
 *
 * SHGFI_LINKOVERLAY
 *
 * SHGFI_OPENICON
 *
 * SHGFI_SELECTED
 *
 * SHGFI_SHELLICONSIZE
 *
 * Also for Windows CE versions 2.10 and later, SHGetFileInfo supports the
 * SHGFI_PIDL flag for the H/PC platform only.
 *
 * @param pszPath [in] Pointer to a buffer that contains the path and file
 *        name. Both absolute and relative paths are valid. If the uFlags
 *        parameter includes the SHGFI_PIDL flag, this parameter must be
 *        the address of an ITEMLIST (PIDL) structure that contains the
 *        list of item identifiers that uniquely identifies the file within
 *        the shell's namespace. The PIDL must be a fully qualified PIDL.
 *        Relative PIDLs are not allowed. If the uFlags parameter includes
 *        the SHGFI_USEFILEATTRIBUTES flag, this parameter does not have to
 *        be a valid file name. The function will proceed as if the file
 *        exists with the specified name and with the file attributes
 *        passed in the dwFileAttributes parameter. This allows you to
 *        obtain information about a file type by passing just the
 *        extension for pszPath and passing FILE_ATTRIBUTE_NORMAL in
 *        dwFileAttributes. This string can use either short (the 8.3 form)
 *        or long file names.
 * @param dwFileAttributes [in] Combination of one or more file attribute
 *        flags (FILE_ATTRIBUTE_* values). If uFlags does not include the
 *        SHGFI_USEFILEATTRIBUTES flag, this parameter is ignored. Windows
 *        CE supports the following additional values for the
 *        dwFileAttributes parameter: Value Description
 *        FILE_ATTRIBUTE_INROM The file is an operating system file stored
 *        in ROM. These files are read-only. FILE_ATTRIBUTE_ROMMODULE The
 *        file is an operating system file stored in ROM, designed to
 *        execute in place. In other words, code from this file is executed
 *        directly from ROM, rather than being first copied to RAM. The
 *        CreateFile function cannot be used to access this file, instead
 *        the LoadLibrary and CreateProcess functions must be used.
 * @param psfi [in] Pointer to a SHFILEINFO structure to receive the file
 *        information.
 * @param cbFileInfo [in] Size, in bytes, of the SHFILEINFO structure
 *        pointed to by the psfi parameter.
 * @param uFlags [in] Flags that specify the file information to retrieve.
 *        It can be a combination of the following values: Value
 *        Description SHGFI_ATTR_SPECIFIED Modifies SHGFI_ATTRIBUTES.
 *        Indicates that the dwAttributes member of the SHFILEINFO
 *        structure at psfi contains the specific attributes that are
 *        desired. SHGFI_ATTRIBUTES Retrieve the item attributes. The
 *        attributes are copied to the dwAttributes member of the structure
 *        specified in the psfi parameter. SHGFI_DISPLAYNAME Retrieve the
 *        display name for the file. The name is copied to the
 *        szDisplayName member of the structure specified in psfi. The
 *        returned display name uses the long file name, if there is one,
 *        rather than the 8.3 form of the file name. SHGFI_EXETYPE Retrieve
 *        the type of the executable file if pszPath identifies an
 *        executable file. This flag cannot be specified with any other
 *        flags. SHGFI_ICON Retrieve the handle to the icon that represents
 *        the file and the index of the icon within the system image list.
 *        The handle is copied to the hIcon member of the structure
 *        specified by psfi, and the index is copied to the iIcon member.
 *        The return value is the handle to the system image list. You must
 *        call the DestroyIcon function on this icon handle.
 *        SHGFI_ICONLOCATION Retrieve the name of the file that contains
 *        the icon representing the file. The name is copied to the
 *        szDisplayName member of the structure specified in psfi.
 *        SHGFI_LARGEICON Modify SHGFI_ICON, causing the function to
 *        retrieve the file's large icon. SHGFI_LINKOVERLAY Modify
 *        SHGFI_ICON, causing the function to add the link overlay to the
 *        file's icon. SHGFI_OPENICON Modify SHGFI_ICON, causing the
 *        function to retrieve the file's open icon. A container object
 *        displays an open icon to indicate that the container is open.
 *        SHGFI_PIDL Indicate that pszPath is the address of an ITEMIDLIST
 *        structure rather than a path name. SHGFI_SELECTED Modify
 *        SHGFI_ICON, causing the function to blend the file's icon with
 *        the system highlight color. SHGFI_SHELLICONSIZE Modify
 *        SHGFI_ICON, causing the function to retrieve a shell-sized icon.
 *        If this flag is not specified the function sizes the icon
 *        according to the system metric values. SHGFI_SMALLICON Modify
 *        SHGFI_ICON, causing the function to retrieve the file's small
 *        icon. SHGFI_SYSICONINDEX Retrieve the index of the icon within
 *        the system image list. The index is copied to the iIcon member of
 *        the structure specified by psfi. The return value is the handle
 *        to the system image list. SHGFI_TYPENAME Retrieve the string that
 *        describes the file's type. The string is copied to the szTypeName
 *        member of the structure specified in psfi.
 *        SHGFI_USEFILEATTRIBUTES Indicates that the function should not
 *        attempt to access the file specified by pszPath. Rather, it
 *        should act as if the file specified by pszPath exists with the
 *        file attributes passed in dwFileAttributes. This flag cannot be
 *        combined with the SHGFI_ATTRIBUTES, SHGFI_EXETYPE, or SHGFI_PIDL
 *        flags.
 * @return Returns a value whose meaning depends on the uFlags parameter.
 *        If uFlags contains the SHGFI_EXETYPE flag, the return value
 *        specifies the type of the executable file. It will be one of the
 *        following values: Value Executable File Type 0 Nonexecutable file
 *        or an error condition Low-order word = NE or PE and high-order
 *        word = 3.0, 3.5, or 4.0 Windows application Low-order word = MZ
 *        and high-order word = 0 MS-DOS .exe, .com, or .bat file Low-order
 *        word = PE and high-order word = 0 Win32 console application If
 *        uFlags contains SHGFI_SYSICONINDEX, the return value is the
 *        handle to the system image list that contains the large icon
 *        images. If SHGFI_SMALLICON is included with SHGFI_SYSICONINDEX,
 *        the return value is the handle to the image list that contains
 *        the small icon images. If uFlags does not contain SHGFI_EXETYPE
 *        or SHGFI_SYSICONINDEX, the return value is nonzero if successful,
 *        or zero otherwise.
 */
#define SHGetFileInfo _WNAME(SHGetFileInfo)
#define SHQueryRecycleBin SHQueryRecycleBinW
#define SHEmptyRecycleBin SHEmptyRecycleBinW

#else
typedef NOTIFYICONDATAA NOTIFYICONDATA,*PNOTIFYICONDATA;
typedef SHELLEXECUTEINFOA SHELLEXECUTEINFO,*LPSHELLEXECUTEINFO;
typedef SHFILEOPSTRUCTA SHFILEOPSTRUCT,*LPSHFILEOPSTRUCT;
typedef SHFILEINFOA SHFILEINFO;
#define DragQueryFile DragQueryFileA
#define ExtractAssociatedIcon ExtractAssociatedIconA
#define ExtractIcon ExtractIconA
#define ExtractIconEx ExtractIconExA
#define FindExecutable FindExecutableA
#define Shell_NotifyIcon Shell_NotifyIconA
#define ShellAbout ShellAboutA
#define ShellExecute ShellExecuteA
#define ShellExecuteEx ShellExecuteExA
#define SHFileOperation SHFileOperationA
#define SHGetFileInfo SHGetFileInfoA
#define SHQueryRecycleBin SHQueryRecycleBinA
#define SHEmptyRecycleBin SHEmptyRecycleBinA
#endif
#ifdef __cplusplus
}
#endif
#endif
