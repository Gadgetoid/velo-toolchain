/*
	tlhelp32.h - Include file for Tool help functions.

	Written by Mumit Khan <khan@nanotech.wisc.edu>

	This file is part of a free library for the Win32 API. 

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*/
#ifndef _TLHELP32_H
#define _TLHELP32_H
#if __GNUC__ >=3
#pragma GCC system_header
#endif

#ifdef __cplusplus
extern "C" {
#endif
#define HF32_DEFAULT	1
#define HF32_SHARED	2
#define LF32_FIXED	0x1
#define LF32_FREE	0x2
#define LF32_MOVEABLE	0x4
#define MAX_MODULE_NAME32	255
#define TH32CS_SNAPHEAPLIST	0x1
#define TH32CS_SNAPPROCESS	0x2
#define TH32CS_SNAPTHREAD	0x4
#define TH32CS_SNAPMODULE	0x8
#define TH32CS_GETALLMODS      0x80000000
#define TH32CS_SNAPALL	(TH32CS_SNAPHEAPLIST|TH32CS_SNAPPROCESS|TH32CS_SNAPTHREAD|TH32CS_SNAPMODULE)
#define TH32CS_INHERIT	0x80000000
/**
 * This structure describes an entry from a list that enumerates the heaps
 * used by a specified process.
 */
typedef struct tagHEAPLIST32 {
	/**
	 * Specifies the length, in bytes, of the structure. Before
	 * calling the Heap32ListFirst function, set this member to
	 * sizeof(HEAPLIST32). If you do not initialize dwSize, Heap32First
	 * will fail.
	 */
	DWORD dwSize;
	/**
	 * Identifier of the process to examine. The contents
	 * of this member can be used by Win32 API elements.
	 */
	DWORD th32ProcessID;
	/**
	 * Heap identifier in the owning process context. The
	 * contents of this member has meaning only to the tool help
	 * functions. It is not a handle, nor is it usable by Win32 API
	 * elements.
	 */
	DWORD th32HeapID;
	/**
	 * Specifies a bitmask of flags. The following flag is
	 * currently defined: Value Description HF32_DEFAULT Process's
	 * default heap.
	 */
	DWORD dwFlags;
} HEAPLIST32,*PHEAPLIST32,*LPHEAPLIST32;
/**
 * This structure describes one entry (block) of a heap that is being
 * examined.
 *
 * Windows CE does not support movable blocks.
 */
typedef struct tagHEAPENTRY32 {
	/**
	 * Specifies the length, in bytes, of the structure. Before
	 * calling the Heap32First function, set this member to
	 * sizeof(HEAPENTRY32). If you do not initialize dwSize, Heap32First
	 * will fail.
	 */
	DWORD dwSize;
	/**
	 * Handle to the heap block.
	 */
	HANDLE hHandle;
	/**
	 * Specifies the linear address of the start of the
	 * block.
	 */
	DWORD dwAddress;
	/**
	 * Specifies the size, in bytes, of the heap block.
	 */
	DWORD dwBlockSize;
	/**
	 * Specifies a bitmask of fags. It is one of the following
	 * values: Value Description LF32_BIGBLOCK A large piece of memory
	 * that has been virtually allocated. LF32_DECOMMIT Decommited
	 * memory block. LF32_FIXED The memory block has a fixed (unmovable)
	 * location. LF32_FREE The memory block is not used.
	 */
	DWORD dwFlags;
	/**
	 * Specifies the lock count on the memory block. The
	 * lock count is incremented each time the LocalLock function is
	 * called on the block either by the application or the DLL that the
	 * heap belongs to.
	 */
	DWORD dwLockCount;
	/**
	 * Reserved; do not use.
	 */
	DWORD dwResvd;
	/**
	 * Identifier of the process to examine. The contents
	 * of this member can be used by Win32 API elements.
	 */
	DWORD th32ProcessID;
	/**
	 * Heap identifier in the owning process context. The
	 * contents of this member has meaning only to the tool help
	 * functions. It is not a handle, nor is it usable by Win32 API
	 * elements.
	 */
	DWORD th32HeapID;
} HEAPENTRY32,*PHEAPENTRY32,*LPHEAPENTRY32;
typedef struct tagPROCESSENTRY32W {
	DWORD dwSize;
	DWORD cntUsage;
	DWORD th32ProcessID;
	DWORD th32DefaultHeapID;
	DWORD th32ModuleID;
	DWORD cntThreads;
	DWORD th32ParentProcessID;
	LONG pcPriClassBase;
	DWORD dwFlags;
	WCHAR szExeFile[MAX_PATH];
} PROCESSENTRY32W,*PPROCESSENTRY32W,*LPPROCESSENTRY32W;
/**
 * This structure describes an entry from a list that enumerates the
 * processes residing in the system address space when a snapshot was
 * taken.
 *
 * All strings are Unicode.
 */
typedef struct tagPROCESSENTRY32 {
	/**
	 * Specifies the length, in bytes, of the structure. Before
	 * calling the Process32First function, set this member to
	 * sizeof(PROCESSENTRY32). If you do not initialize dwSize,
	 * Process32First will fail.
	 */
	DWORD dwSize;
	/**
	 * Number of references to the process. Must be 1.
	 */
	DWORD cntUsage;
	/**
	 * Identifier of the process. The contents of this
	 * member can be used by Win32 API elements.
	 */
	DWORD th32ProcessID;
	/**
	 * Identifier of the default heap for the
	 * process. The contents of this member has meaning only to the tool
	 * help functions. It is not a handle, nor is it usable by Win32 API
	 * elements.
	 */
	DWORD th32DefaultHeapID;
	/**
	 * Module identifier of the process. Must be 0.
	 */
	DWORD th32ModuleID;
	/**
	 * Number of execution threads started by the process.
	 */
	DWORD cntThreads;
	/**
	 * Identifier of the process that created the
	 * process being examined. Must be 0.
	 */
	DWORD th32ParentProcessID;
	/**
	 * Base priority of any threads created by this
	 * process. Value is always THREAD_PRIORITY_NORMAL
	 */
	LONG pcPriClassBase;
	/**
	 * Reserved; do not use.
	 */
	DWORD dwFlags;
#ifdef _WIN32_WCE
	/**
	 * Null-terminated string that contains the path and file
	 * name of the executable file for the process.
	 */
	WCHAR szExeFile[MAX_PATH];
	DWORD th32MemoryBase;
	/**
	 * Array of bits. Each bit signifies permission to
	 * see the address space of one process. For the PROCESSENTRY32
	 * structure, the key contains the bit required to see that address
	 * space.
	 */
	DWORD th32AccessKey;
#else
	CHAR  szExeFile[MAX_PATH];
#endif
} PROCESSENTRY32,*PPROCESSENTRY32,*LPPROCESSENTRY32;
/**
 * This structure describes an entry from a list that enumerates the
 * threads executing in the system when a snapshot was taken.
 *
 * Windows CE supports the following additional members:
 *
 * th32AccessKey: Array of bits. Each bit signifies permission to see the
 * address space of one process. In the THREADENTRY32 structure, the key
 * contains the bits the thread currently has, thus defining the processes
 * it can currently see.
 *
 * th32CurrentProcessID: Process identifier where the thread is executing.
 */
typedef struct tagTHREADENTRY32 {
	/**
	 * Specifies the length, in bytes, of the structure. Before
	 * calling the Thread32First function, set this member to
	 * sizeof(THREADENTRY32). If you do not initialize dwSize,
	 * Thread32First will fail.
	 */
	DWORD dwSize;
	/**
	 * Number of references to the thread. A thread exists as
	 * long as its usage count is nonzero. As soon as its usage count
	 * becomes zero, a thread terminates.
	 */
	DWORD cntUsage;
	/**
	 * Identifier of the thread. This identifier is
	 * compatible with the thread identifier returned by the
	 * CreateProcess function.
	 */
	DWORD th32ThreadID;
	/**
	 * Identifier of the process that created the
	 * thread. The contents of this member can be used by Win32 API
	 * elements.
	 */
	DWORD th32OwnerProcessID;
	/**
	 * Initial priority level assigned to a thread. This
	 * member can be one of the following values: THREAD_PRIORITY_IDLE:
	 * Indicates a base priority level of 1 for IDLE_PRIORITY_CLASS,
	 * NORMAL_PRIORITY_CLASS, or HIGH_PRIORITY_CLASS processes, and a
	 * base priority level of 16 for REALTIME_PRIORITY_CLASS processes.
	 * THREAD_PRIORITY_LOWEST: Indicates 2 points below normal priority
	 * for the priority class. THREAD_PRIORITY_BELOW_NORMAL: Indicates 1
	 * point below normal priority for the priority class.
	 * THREAD_PRIORITY_NORMAL: Indicates normal priority for the
	 * priority class. THREAD_PRIORITY_ABOVE_NORMAL: Indicates 1 point
	 * above normal priority for the priority class.
	 * THREAD_PRIORITY_HIGHEST: Indicates 2 points above normal priority
	 * for the priority class. THREAD_PRIORITY_TIME_CRITICAL: Indicates
	 * a base priority level of 15 for IDLE_PRIORITY_CLASS,
	 * NORMAL_PRIORITY_CLASS, or HIGH_PRIORITY_CLASS processes, and a
	 * base priority level of 31 for REALTIME_PRIORITY_CLASS processes.
	 */
	LONG tpBasePri;
	/**
	 * Change in the priority level of a thread. This value
	 * is a signed delta from the base priority level assigned to the
	 * thread.
	 */
	LONG tpDeltaPri;
	/**
	 * Reserved; do not use.
	 */
	DWORD dwFlags;
#ifdef _WIN32_WCE
	DWORD th32AccessKey;
	DWORD th32CurrentProcessID;
#endif
} THREADENTRY32,*PTHREADENTRY32,*LPTHREADENTRY32;
typedef struct tagMODULEENTRY32W {
	DWORD dwSize;
	DWORD th32ModuleID;
	DWORD th32ProcessID;
	DWORD GlblcntUsage;
	DWORD ProccntUsage;
	BYTE *modBaseAddr;
	DWORD modBaseSize;
	HMODULE hModule; 
	WCHAR szModule[MAX_MODULE_NAME32 + 1];
	WCHAR szExePath[MAX_PATH];
} MODULEENTRY32W,*PMODULEENTRY32W,*LPMODULEENTRY32W;
/**
 * This structure describes an entry from a list that enumerates the
 * modules used by a specified process.
 *
 * The modBaseAddr and hModule members are valid only in the context of the
 * process specified by th32ProcessID.
 */
typedef struct tagMODULEENTRY32 {
	/**
	 * Specifies the length, in bytes, of the structure. Before
	 * calling the Module32First function, set this member to
	 * sizeof(MODULEENTRY32). If you do not initialize dwSize,
	 * Module32First will fail.
	 */
	DWORD dwSize;
	/**
	 * Module identifier in the context of the owning
	 * process. The contents of this member has meaning only to the tool
	 * help functions. It is not a handle, nor is it usable by Win32 API
	 * elements.
	 */
	DWORD th32ModuleID;
	/**
	 * Identifier of the process being examined. The
	 * contents of this member can be used by Win32 API elements.
	 */
	DWORD th32ProcessID;
	/**
	 * Global usage count on the module.
	 */
	DWORD GlblcntUsage;
	/**
	 * Module usage count in the context of the owning
	 * process.
	 */
	DWORD ProccntUsage;
	/**
	 * Base address of the module in the context of the
	 * owning process.
	 */
	BYTE *modBaseAddr;
	/**
	 * Size, in bytes, of the module.
	 */
	DWORD modBaseSize;
	/**
	 * Handle to the module in the context of the owning
	 * process.
	 */
	HMODULE hModule;
#ifdef _WIN32_WCE
       /**
        * Null-terminated string that contains the module name.
        */
       WCHAR szModule[MAX_PATH];
	/**
	 * Null-terminated string that contains the location
	 * (path) of the module.
	 */
	WCHAR szExePath[MAX_PATH];
	/**
	 * Reserved.
	 */
	DWORD dwFlags;
#else
	CHAR szModule[MAX_MODULE_NAME32 + 1];
	CHAR szExePath[MAX_PATH];
#endif
} MODULEENTRY32,*PMODULEENTRY32,*LPMODULEENTRY32;
#ifdef _WIN32_WCE
/**
 * This function retrieves information about the first block of a heap that
 * has been allocated by a process.
 *
 * The calling application must set the dwSize member of HEAPENTRY32 to the
 * size, in bytes, of the structure. Heap32First changes dwSize to the
 * number of bytes written to the structure. This will never be greater
 * than the initial value of dwSize, but it may be smaller. If the value is
 * smaller, do not rely on the values of any members whose offsets are
 * greater than this value.
 *
 * To access subsequent blocks of the same heap, use the Heap32Next
 * function.
 *
 * @param hSnapshot Handle to the snapshot returned from a previous call to
 *        the CreateToolhelp32Snapshot function. This parameter is specific
 *        to Windows CE.
 * @param lphe Pointer to a HEAPENTRY32 structure.
 * @param th32ProcessID Identifier of the process context that owns the
 *        heap.
 * @param th32HeapID Identifier of the heap to enumerate.
 * @return TRUE indicates that information for the first heap block has
 *        been copied to the buffer. FALSE indicates success. The
 *        ERROR_NO_MORE_FILES error value, returned by GetLastError,
 *        indicates that the heap is invalid or empty.
 */
BOOL WINAPI Heap32First(HANDLE hSnapshot,LPHEAPENTRY32 lphe,DWORD th32ProcessID,DWORD th32HeapID);
#else
BOOL WINAPI Heap32First(LPHEAPENTRY32,DWORD,DWORD);
#endif
/**
 * This function retrieves information about the first heap that has been
 * allocated by a specified process.
 *
 * The calling application must set the dwSize member of HEAPLIST32 to the
 * size, in bytes, of the structure. Heap32ListFirst changes dwSize to the
 * number of bytes written to the structure. This will never be greater
 * than the initial value of dwSize, but it may be smaller. If the value is
 * smaller, do not rely on the values of any members whose offsets are
 * greater than this value.
 *
 * To retrieve information about other heaps in the heap list, use the
 * Heap32ListNext function.
 *
 * @param hSnapshot Handle to the snapshot returned from a previous call to
 *        the CreateToolhelp32Snapshot function.
 * @param lphl Pointer to a HEAPLIST32 structure.
 * @return TRUE indicates that the first entry of the heap list has been
 *        copied to the buffer. FALSE indicates failure. The
 *        ERROR_NO_MORE_FILES error value, returned by GetLastError,
 *        indicates that no heap list exists or the snapshot does not
 *        contain heap list information.
 */
BOOL WINAPI Heap32ListFirst(HANDLE hSnapshot,LPHEAPLIST32 lphl);
/**
 * This function retrieves information about the next heap that has been
 * allocated by a process.
 *
 * The calling application must set the dwSize member of HEAPLIST32 to the
 * size, in bytes, of the structure. Heap32ListFirst changes dwSize to the
 * number of bytes written to the structure. This will never be greater
 * than the initial value of dwSize, but it may be smaller. If the value is
 * smaller, do not rely on the values of any members whose offsets are
 * greater than this value.
 *
 * To retrieve information about the first heap in a heap list, use the
 * Heap32ListFirst function.
 *
 * @param hSnapshot Handle to the snapshot returned from a previous call to
 *        the CreateToolhelp32Snapshot function.
 * @param lphl Long pointer to a HEAPLIST32 structure.
 * @return TRUE indicates that the next entry of the heap list has been
 *        copied to the buffer. FALSE indicates failure. The
 *        ERROR_NO_MORE_FILES error value, returned by GetLastError,
 *        indicates that no more heap list entries exist.
 */
BOOL WINAPI Heap32ListNext(HANDLE hSnapshot,LPHEAPLIST32 lphl);
#ifdef _WIN32_WCE
/**
 * This function retrieves information about the next block of a heap that
 * has been allocated by a process.
 *
 * To retrieve information for the first block of a heap, use the
 * Heap32First function.
 *
 * @param hSnapShot Handle to the snapshot returned from a previous call to
 *        the CreateToolhelp32Snapshot function. This parameter is specific
 *        to Windows CE.
 * @param lphe Long pointer to a HEAPENTRY32 structure.
 * @return TRUE indicates that information about the next block in the heap
 *        has been copied to the buffer. FALSE indicates failure. The
 *        GetLastError function returns ERROR_NO_MORE_FILES when no more
 *        objects in the heap exist and ERROR_INVALID_DATA if the heap
 *        appears to be corrupt or is modified during the walk in such a
 *        way that Heap32Next cannot continue.
 */
BOOL WINAPI Heap32Next(HANDLE hSnapShot,LPHEAPENTRY32 lphe);
#else
BOOL WINAPI Heap32Next(LPHEAPENTRY32);
#endif
/**
 * This function retrieves information about the first module associated
 * with a process.
 *
 * The calling application must set the dwSize member of MODULEENTRY32 to
 * the size, in bytes, of the structure. Module32First changes dwSize to
 * the number of bytes written to the structure. This will never be greater
 * than the initial value of dwSize, but it may be smaller. If the value is
 * smaller, do not rely on the values of any members whose offsets are
 * greater than this value.
 *
 * To retrieve information about other modules associated with the
 * specified process, use the Module32Next function.
 *
 * @param hSnapshot Handle to the snapshot returned from a previous call to
 *        the CreateToolhelp32Snapshot function.
 * @param lpme Pointer to a MODULEENTRY32 structure.
 * @return TRUE indicates that the first entry of the module list has been
 *        copied to the buffer. FALSE indicates failure. The
 *        ERROR_NO_MORE_FILES error value, returned by GetLastError,
 *        indicates that no modules exist or the snapshot does not contain
 *        module information.
 */
BOOL WINAPI Module32First(HANDLE hSnapshot,LPMODULEENTRY32 lpme);
BOOL WINAPI Module32FirstW(HANDLE,LPMODULEENTRY32W);
/**
 * This function retrieves information about the next module associated
 * with a process or thread.
 *
 * To retrieve information about first module associated with a process,
 * use the Module32First function.
 *
 * @param hSnapshot Handle to the snapshot returned from a previous call to
 *        the CreateToolhelp32Snapshot function.
 * @param lpme Pointer to a MODULEENTRY32 structure.
 * @return TRUE indicates the next entry of the module list has been copied
 *        to the buffer. FALSE indicates failure. The ERROR_NO_MORE_FILES
 *        error value, returned by GetLastError, indicates that no more
 *        modules exist.
 */
BOOL WINAPI Module32Next(HANDLE hSnapshot,LPMODULEENTRY32 lpme);
BOOL WINAPI Module32NextW(HANDLE,LPMODULEENTRY32W);
/**
 * This function retrieves information about the first process encountered
 * in a system snapshot.
 *
 * The calling application must set the dwSize member of PROCESSENTRY32 to
 * the size, in bytes, of the structure. Process32First changes dwSize to
 * the number of bytes written to the structure. This will never be greater
 * than the initial value of dwSize, but it may be smaller. If the value is
 * smaller, do not rely on the values of any members whose offsets are
 * greater than this value.
 *
 * To retrieve information about other processes recorded in the same
 * snapshot, use the Process32Next function.
 *
 * @param hSnapshot Handle to the snapshot returned from a previous call to
 *        the CreateToolhelp32Snapshot function.
 * @param lppe Pointer to a PROCESSENTRY32 structure.
 * @return TRUE indicates that the first entry of the process list has been
 *        copied to the buffer. FALSE indicates failure. The
 *        ERROR_NO_MORE_FILES error value, returned by GetLastError,
 *        indicates that no processes exist or the snapshot does not
 *        contain process information.
 */
BOOL WINAPI Process32First(HANDLE hSnapshot,LPPROCESSENTRY32 lppe);
BOOL WINAPI Process32FirstW(HANDLE,LPPROCESSENTRY32W);
/**
 * This function retrieves information about the next process recorded in a
 * system snapshot.
 *
 * To retrieve information about the first process recorded in a snapshot,
 * use the Process32First function.
 *
 * @param hSnapshot Handle to the snapshot returned from a previous call to
 *        the CreateToolhelp32Snapshot function.
 * @param lppe Pointer to a PROCESSENTRY32 structure.
 * @return TRUE indicates that the next entry of the process list has been
 *        copied to the buffer. FALSE indicates failure. The
 *        ERROR_NO_MORE_FILES error value, returned by GetLastError,
 *        indicates that no processes exist or the snapshot does not
 *        contain process information.
 */
BOOL WINAPI Process32Next(HANDLE hSnapshot,LPPROCESSENTRY32 lppe);
BOOL WINAPI Process32NextW(HANDLE,LPPROCESSENTRY32W);
/**
 * This function retrieves information about the first thread of any
 * process encountered in a system snapshot.
 *
 * The calling application must set the dwSize member of THREADENTRY32 to
 * the size, in bytes, of the structure. Thread32First changes dwSize to
 * the number of bytes written to the structure. This will never be greater
 * than the initial value of dwSize, but it may be smaller. If the value is
 * smaller, do not rely on the values of any members whose offsets are
 * greater than this value.
 *
 * To retrieve information about other threads recorded in the same
 * snapshot, use the Thread32Next function.
 *
 * @param hSnapshot Handle to the snapshot returned from a previous call to
 *        the CreateToolhelp32Snapshot function.
 * @param lpte Pointer to a THREADENTRY32 structure.
 * @return TRUE indicates that the first entry of the thread list has been
 *        copied to the buffer. FALSE indicates failure. The
 *        ERROR_NO_MORE_FILES error value, returned by GetLastError,
 *        indicates that no threads exist or the snapshot does not contain
 *        thread information.
 */
BOOL WINAPI Thread32First(HANDLE hSnapshot,LPTHREADENTRY32 lpte);
/**
 * This function retrieves information about the next thread of any process
 * encountered in the system memory snapshot.
 *
 * To retrieve information about the first thread recorded in a snapshot,
 * use the Thread32First function.
 *
 * @param hSnapshot Handle to the snapshot returned from a previous call to
 *        the CreateToolhelp32Snapshot function.
 * @param lpte Pointer to a THREADENTRY32 structure.
 * @return TRUE indicates that the next entry of the thread list has been
 *        copied to the buffer. FALSE indicates failure. The
 *        ERROR_NO_MORE_FILES error value, returned by GetLastError,
 *        indicates that no threads exist or the snapshot does not contain
 *        thread information.
 */
BOOL WINAPI Thread32Next(HANDLE hSnapshot,LPTHREADENTRY32 lpte);
/**
 * This function copies memory allocated to another process into an
 * application-supplied buffer.
 *
 * @param th32ProcessID Identifier of the process whose memory is being
 *        copied. This parameter can be zero to copy the memory of the
 *        current process.
 * @param lpBaseAddress Base address in the specified process to read.
 *        Before transferring any data, the system verifies that all data
 *        in the base address and memory of the specified size is
 *        accessible for read access. If this is the case, the function
 *        proceeds. Otherwise, the function fails.
 * @param lpBuffer Pointer to a buffer that receives the contents of the
 *        address space of the specified process.
 * @param cbRead Number of bytes to read from the specified process.
 * @param lpNumberOfBytesRead Pointer to the number of bytes copied to the
 *        specified buffer. If this parameter is NULL, it is ignored.
 * @return TRUE indicates success. FALSE indicates failure.
 */
BOOL WINAPI Toolhelp32ReadProcessMemory(DWORD th32ProcessID,LPCVOID lpBaseAddress,LPVOID lpBuffer,DWORD cbRead,LPDWORD lpNumberOfBytesRead);
/**
 * This function takes a snapshot of the processes and the heaps, modules,
 * and threads used by the processes.
 *
 * The snapshot returned is a copy of the current state of the system. To
 * close a snapshot call the CloseToolhelp32Snapshot function.
 *
 * The snapshot taken by this function is examined by the other tool help
 * functions to provide their results. Access to the snapshot is read only.
 * The snapshot handle acts like an object handle and is subject to the
 * same rules regarding which processes and threads it is valid in.
 *
 * To retrieve an extended error status code generated by this function,
 * use the GetLastError function.
 *
 * @param dwFlags Specifies portions of the system to include in the
 *        snapshot. This parameter can be one of the following: Value
 *        Description TH32CS_SNAPALL Equivalent to specifying
 *        TH32CS_SNAPHEAPLIST, TH32CS_SNAPMODULE, TH32CS_SNAPPROCESS, and
 *        TH32CS_SNAPTHREAD. TH32CS_SNAPHEAPLIST Includes the heap list of
 *        the specified process in the snapshot. TH32CS_SNAPMODULE Includes
 *        the module list of the specified process in the snapshot.
 *        TH32CS_SNAPPROCESS Includes the process list in the snapshot.
 *        TH32CS_SNAPTHREAD Includes the thread list in the snapshot.
 *        THCS_GETALLMODES Returns a list of all the modules loaded in the
 *        system.
 * @param th32ProcessID Process identifier. This parameter can be zero to
 *        indicate the current process. This parameter is used when the
 *        TH32CS_SNAPHEAPLIST or TH32CS_SNAPMODULE value is specified.
 *        Otherwise, it is ignored.
 * @return An open handle to the specified snapshot indicates success. - 1
 *        indicates failure.
 */
HANDLE WINAPI CreateToolhelp32Snapshot(DWORD dwFlags,DWORD th32ProcessID);
#if (_WIN32_WCE >= 0x200)
/**
 * This function closes a handle to a snapshot.
 *
 * Close a snapshot in order to free valuable resources for the system.
 *
 * @param hSnapshot Handle to a snapshot returned from the
 *        CreateToolhelp32Snapshot function.
 */
BOOL WINAPI CloseToolhelp32Snapshot(HANDLE hSnapshot);
#endif
#if defined(UNICODE) && !defined(_WIN32_WCE)
#define LPMODULEENTRY32 LPMODULEENTRY32W
#define LPPROCESSENTRY32 LPPROCESSENTRY32W
#define MODULEENTRY32 MODULEENTRY32W
#define Module32First Module32FirstW
#define Module32Next Module32NextW
#define PMODULEENTRY32 PMODULEENTRY32W
#define PPROCESSENTRY32 PPROCESSENTRY32W
#define PROCESSENTRY32 PROCESSENTRY32W
#define Process32First Process32FirstW
#define Process32Next Process32NextW
#endif /* UNICODE && !_WIN32_WCE */
#ifdef __cplusplus
}
#endif
#endif /* _TLHELP32_H */

