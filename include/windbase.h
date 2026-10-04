#ifndef VELO_WINDBASE_H
#define VELO_WINDBASE_H

#include <windows.h>

#ifndef RC_INVOKED

typedef DWORD CEOID;
typedef CEOID *PCEOID;
typedef DWORD CEPROPID;
typedef CEPROPID *PCEPROPID;

typedef struct _CEFILEINFO {
    DWORD dwAttributes;
    CEOID oidParent;
    WCHAR szFileName[MAX_PATH];
    FILETIME ftLastChanged;
    DWORD dwLength;
} CEFILEINFO;

typedef struct _CEDIRINFO {
    DWORD dwAttributes;
    CEOID oidParent;
    WCHAR szDirName[MAX_PATH];
} CEDIRINFO;

typedef struct _CERECORDINFO {
    CEOID oidParent;
} CERECORDINFO;

typedef struct _SORTORDERSPEC {
    CEPROPID propid;
    DWORD dwFlags;
} SORTORDERSPEC;

#define CEDB_MAXDBASENAMELEN 32
#define CEDB_MAXSORTORDER 4

typedef struct _CEDBASEINFO {
    DWORD dwFlags;
    WCHAR szDbaseName[CEDB_MAXDBASENAMELEN];
    DWORD dwDbaseType;
    WORD wNumRecords;
    WORD wNumSortOrder;
    DWORD dwSize;
    FILETIME ftLastModified;
    SORTORDERSPEC rgSortSpecs[CEDB_MAXSORTORDER];
} CEDBASEINFO;

typedef struct _CEBLOB {
    DWORD dwCount;
    LPBYTE lpb;
} CEBLOB;

typedef union _CEVALUNION {
    short iVal;
    USHORT uiVal;
    long lVal;
    ULONG ulVal;
    FILETIME filetime;
    LPWSTR lpwstr;
    CEBLOB blob;
} CEVALUNION;

typedef struct _CEPROPVAL {
    CEPROPID propid;
    WORD wLenData;
    WORD wFlags;
    CEVALUNION val;
} CEPROPVAL;
typedef CEPROPVAL *PCEPROPVAL;

typedef struct _CEOIDINFO {
    WORD wObjType;
    WORD wPad;
    union {
        CEFILEINFO infFile;
        CEDIRINFO infDirectory;
        CEDBASEINFO infDatabase;
        CERECORDINFO infRecord;
    };
} CEOIDINFO;

typedef CEOID PEGOID;
typedef PCEOID PPEGOID;
typedef CEPROPID PEGPROPID;
typedef PCEPROPID PPEGPROPID;
typedef CEFILEINFO PEGFILEINFO;
typedef CEDIRINFO PEGDIRINFO;
typedef CERECORDINFO PEGRECORDINFO;
typedef CEDBASEINFO PEGDBASEINFO;
typedef CEBLOB PEGBLOB;
typedef CEVALUNION PEGVALUNION;
typedef CEPROPVAL PEGPROPVAL;
typedef PCEPROPVAL PPEGPROPVAL;
typedef CEOIDINFO PEGOIDINFO;

#endif

#define TypeFromPropID(propid) LOWORD(propid)

#define DB_CEOID_CREATED (WM_USER + 0x1)
#define DB_CEOID_DATABASE_DELETED (WM_USER + 0x2)
#define DB_CEOID_RECORD_DELETED (WM_USER + 0x3)
#define DB_CEOID_FILE_DELETED (WM_USER + 0x4)
#define DB_CEOID_DIRECTORY_DELETED (WM_USER + 0x5)
#define DB_CEOID_CHANGED (WM_USER + 0x6)
#define DB_PEGOID_CREATED DB_CEOID_CREATED
#define DB_PEGOID_RECORD_DELETED DB_CEOID_RECORD_DELETED
#define DB_PEGOID_CHANGED DB_CEOID_CHANGED

#define REPL_CHANGE_WILLCLEAR 0x00000001

#define CEDB_SORT_DESCENDING 0x00000001
#define CEDB_SORT_CASEINSENSITIVE 0x00000002
#define CEDB_SORT_UNKNOWNFIRST 0x00000004
#define CEDB_SORT_GENERICORDER 0x00000008
#define CEDB_VALIDNAME 0x0001
#define CEDB_VALIDTYPE 0x0002
#define CEDB_VALIDSORTSPEC 0x0004
#define CEDB_VALIDMODTIME 0x0008
#define CEDB_AUTOINCREMENT 0x00000001
#define CEDB_SEEK_CEOID 0x00000001
#define CEDB_SEEK_BEGINNING 0x00000002
#define CEDB_SEEK_END 0x00000004
#define CEDB_SEEK_CURRENT 0x00000008
#define CEDB_SEEK_VALUESMALLER 0x00000010
#define CEDB_SEEK_VALUEFIRSTEQUAL 0x00000020
#define CEDB_SEEK_VALUEGREATER 0x00000040
#define CEDB_SEEK_VALUENEXTEQUAL 0x00000080
#define CEDB_PROPNOTFOUND 0x0100
#define CEDB_PROPDELETE 0x0200
#define CEDB_MAXDATABLOCKSIZE 4092
#if VELO_CE >= 2
#define CEDB_MAXPROPDATASIZE ((CEDB_MAXDATABLOCKSIZE * 16) - 1)
#else
#define CEDB_MAXPROPDATASIZE (CEDB_MAXDATABLOCKSIZE * 16)
#endif
#define CEDB_MAXRECORDSIZE (128 * 1024)
#define CEDB_ALLOWREALLOC 0x00000001

#define CEVT_I2 2
#define CEVT_UI2 18
#define CEVT_I4 3
#define CEVT_UI4 19
#define CEVT_FILETIME 64
#define CEVT_LPWSTR 31
#define CEVT_BLOB 65

#define OBJTYPE_INVALID 0
#define OBJTYPE_FILE 1
#define OBJTYPE_DIRECTORY 2
#define OBJTYPE_DATABASE 3
#define OBJTYPE_RECORD 4

#define PEGDB_SORT_DESCENDING CEDB_SORT_DESCENDING
#define PEGDB_SORT_CASEINSENSITIVE CEDB_SORT_CASEINSENSITIVE
#define PEGDB_SORT_UNKNOWNFIRST CEDB_SORT_UNKNOWNFIRST
#define PEGDB_SORT_GENERICORDER CEDB_SORT_GENERICORDER
#define PEGDB_MAXDBASENAMELEN CEDB_MAXDBASENAMELEN
#define PEGDB_MAXSORTORDER CEDB_MAXSORTORDER
#define PEGDB_VALIDNAME CEDB_VALIDNAME
#define PEGDB_VALIDTYPE CEDB_VALIDTYPE
#define PEGDB_VALIDSORTSPEC CEDB_VALIDSORTSPEC
#define PEGDB_VALIDMODTIME CEDB_VALIDMODTIME
#define PEGDB_AUTOINCREMENT CEDB_AUTOINCREMENT
#define PEGDB_SEEK_PEGOID CEDB_SEEK_CEOID
#define PEGDB_SEEK_BEGINNING CEDB_SEEK_BEGINNING
#define PEGDB_SEEK_END CEDB_SEEK_END
#define PEGDB_SEEK_CURRENT CEDB_SEEK_CURRENT
#define PEGDB_SEEK_VALUESMALLER CEDB_SEEK_VALUESMALLER
#define PEGDB_SEEK_VALUEFIRSTEQUAL CEDB_SEEK_VALUEFIRSTEQUAL
#define PEGDB_SEEK_VALUEGREATER CEDB_SEEK_VALUEGREATER
#define PEGDB_SEEK_VALUENEXTEQUAL CEDB_SEEK_VALUENEXTEQUAL
#define PEGDB_PROPNOTFOUND CEDB_PROPNOTFOUND
#define PEGDB_PROPDELETE CEDB_PROPDELETE
#define PEGDB_MAXDATABLOCKSIZE CEDB_MAXDATABLOCKSIZE
#define PEGDB_MAXPROPDATASIZE CEDB_MAXPROPDATASIZE
#define PEGDB_MAXRECORDSIZE CEDB_MAXRECORDSIZE
#define PEGDB_ALLOWREALLOC CEDB_ALLOWREALLOC
#define PEGVT_I2 CEVT_I2
#define PEGVT_UI2 CEVT_UI2
#define PEGVT_I4 CEVT_I4
#define PEGVT_UI4 CEVT_UI4
#define PEGVT_FILETIME CEVT_FILETIME
#define PEGVT_LPWSTR CEVT_LPWSTR
#define PEGVT_BLOB CEVT_BLOB

#ifndef RC_INVOKED

#if VELO_CE >= 2

HANDLE CeFindFirstDatabase(DWORD dwClassID);
CEOID CeFindNextDatabase(HANDLE hEnum);
CEOID CeCreateDatabase(LPWSTR lpszName, DWORD dwClassID, WORD wNumSortOrder, SORTORDERSPEC *rgSortSpecs);
BOOL CeSetDatabaseInfo(CEOID oidDbase, CEDBASEINFO *pNewInfo);
HANDLE CeOpenDatabase(PCEOID poid, LPWSTR lpszName, CEPROPID propid, DWORD dwFlags, HWND hwndNotify);
BOOL CeDeleteDatabase(CEOID oid);
CEOID CeSeekDatabase(HANDLE hDatabase, DWORD dwSeekType, DWORD dwValue, LPDWORD lpdwIndex);
BOOL CeDeleteRecord(HANDLE hDatabase, CEOID oidRecord);
CEOID CeReadRecordProps(HANDLE hDbase, DWORD dwFlags, LPWORD lpcPropID, CEPROPID *rgPropID, LPBYTE *lplpBuffer, LPDWORD lpcbBuffer);
CEOID CeWriteRecordProps(HANDLE hDbase, CEOID oidRecord, WORD cPropID, CEPROPVAL *rgPropVal);
BOOL CeOidGetInfo(CEOID oid, CEOIDINFO *oidInfo);

#define PegFindFirstDatabase CeFindFirstDatabase
#define PegFindNextDatabase CeFindNextDatabase
#define PegCreateDatabase CeCreateDatabase
#define PegSetDatabaseInfo CeSetDatabaseInfo
#define PegOpenDatabase CeOpenDatabase
#define PegDeleteDatabase CeDeleteDatabase
#define PegSeekDatabase CeSeekDatabase
#define PegDeleteRecord CeDeleteRecord
#define PegReadRecordProps CeReadRecordProps
#define PegWriteRecordProps CeWriteRecordProps
#define PegOidGetInfo CeOidGetInfo

#else

HANDLE PegFindFirstDatabase(DWORD dwClassID);
PEGOID PegFindNextDatabase(HANDLE hEnum);
PEGOID PegCreateDatabase(LPWSTR lpszName, DWORD dwClassID, WORD wNumSortOrder, SORTORDERSPEC *rgSortSpecs);
BOOL PegSetDatabaseInfo(PEGOID oidDbase, PEGDBASEINFO *pNewInfo);
HANDLE PegOpenDatabase(PPEGOID poid, LPWSTR lpszName, PEGPROPID propid, DWORD dwFlags, HWND hwndNotify);
BOOL PegDeleteDatabase(PEGOID oid);
PEGOID PegSeekDatabase(HANDLE hDatabase, DWORD dwSeekType, DWORD dwValue, LPDWORD lpdwIndex);
BOOL PegDeleteRecord(HANDLE hDatabase, PEGOID oidRecord);
PEGOID PegReadRecordProps(HANDLE hDbase, DWORD dwFlags, LPWORD lpcPropID, PEGPROPID *rgPropID, LPBYTE *lplpBuffer, LPDWORD lpcbBuffer);
PEGOID PegWriteRecordProps(HANDLE hDbase, PEGOID oidRecord, WORD cPropID, PEGPROPVAL *rgPropVal);
BOOL PegOidGetInfo(PEGOID oid, PEGOIDINFO *oidInfo);

#define CeFindFirstDatabase PegFindFirstDatabase
#define CeFindNextDatabase PegFindNextDatabase
#define CeCreateDatabase PegCreateDatabase
#define CeSetDatabaseInfo PegSetDatabaseInfo
#define CeOpenDatabase PegOpenDatabase
#define CeDeleteDatabase PegDeleteDatabase
#define CeSeekDatabase PegSeekDatabase
#define CeDeleteRecord PegDeleteRecord
#define CeReadRecordProps PegReadRecordProps
#define CeWriteRecordProps PegWriteRecordProps
#define CeOidGetInfo PegOidGetInfo

#endif

#endif

#endif
