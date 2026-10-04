#ifndef VELO_EXTRAS_RAS_H
#define VELO_EXTRAS_RAS_H

#ifndef RC_INVOKED

#ifndef VELO_VARSTRING
#define VELO_VARSTRING
typedef struct varstring_tag {
    DWORD dwTotalSize;
    DWORD dwNeededSize;
    DWORD dwUsedSize;
    DWORD dwStringFormat;
    DWORD dwStringSize;
    DWORD dwStringOffset;
} VARSTRING, *LPVARSTRING;
#endif

DWORD WINAPI RasGetEntryDevConfig(LPCWSTR szPhonebook, LPCWSTR szEntry, LPDWORD pdwDeviceID, LPDWORD pdwSize, LPVARSTRING pDeviceConfig);
DWORD WINAPI RasSetEntryDevConfig(LPCWSTR szPhonebook, LPCWSTR szEntry, DWORD dwDeviceID, LPVARSTRING lpDeviceConfig);
DWORD WINAPI RasHangup(HRASCONN Session);

#endif

#endif
