#ifndef VELO_EXTRAS_COMMCTRL_H
#define VELO_EXTRAS_COMMCTRL_H

#ifndef RC_INVOKED

BOOL WINAPI IsCommandBarMessage(HWND hwndCB, LPMSG lpMsg);
void WINAPI ImageList_CopyDitherImage(HIMAGELIST himlDst, WORD iDst, int xDst, int yDst, HIMAGELIST himlSrc, int iSrc, UINT fStyle);
HBITMAP WINAPI CreateColorBitmap(int cx, int cy);
HBITMAP WINAPI CreateMonoBitmap(int cx, int cy);

#if VELO_CE == 1
typedef LV_KEYDOWN NMLVKEYDOWN, *LPNMLVKEYDOWN;
#endif
typedef TV_KEYDOWN NMTVKEYDOWN, *LPNMTVKEYDOWN;
typedef TC_KEYDOWN NMTCKEYDOWN;

#if VELO_CE >= 2

typedef struct tagNMKEY {
    NMHDR hdr;
    WORD wVKey;
    UINT flags;
} NMKEY, *LPNMKEY;

typedef struct tagNMTTCUSTOMDRAW {
    NMCUSTOMDRAW nmcd;
    UINT uDrawFlags;
} NMTTCUSTOMDRAW, *LPNMTTCUSTOMDRAW;

typedef struct tagNMDATETIMESTRINGW {
    NMHDR nmhdr;
    LPCWSTR pszUserString;
    SYSTEMTIME st;
    DWORD dwFlags;
} NMDATETIMESTRINGW, *LPNMDATETIMESTRINGW;

typedef struct tagNMDATETIMESTRINGA {
    NMHDR nmhdr;
    LPCSTR pszUserString;
    SYSTEMTIME st;
    DWORD dwFlags;
} NMDATETIMESTRINGA, *LPNMDATETIMESTRINGA;

typedef struct tagNMDATETIMEWMKEYDOWNW {
    NMHDR nmhdr;
    int nVirtKey;
    LPCWSTR pszFormat;
    SYSTEMTIME st;
} NMDATETIMEWMKEYDOWNW, *LPNMDATETIMEWMKEYDOWNW;

typedef struct tagNMDATETIMEWMKEYDOWNA {
    NMHDR nmhdr;
    int nVirtKey;
    LPCSTR pszFormat;
    SYSTEMTIME st;
} NMDATETIMEWMKEYDOWNA, *LPNMDATETIMEWMKEYDOWNA;

typedef struct tagNMDATETIMEFORMATW {
    NMHDR nmhdr;
    LPCWSTR pszFormat;
    SYSTEMTIME st;
    LPCWSTR pszDisplay;
    WCHAR szDisplay[64];
} NMDATETIMEFORMATW, *LPNMDATETIMEFORMATW;

typedef struct tagNMDATETIMEFORMATA {
    NMHDR nmhdr;
    LPCSTR pszFormat;
    SYSTEMTIME st;
    LPCSTR pszDisplay;
    CHAR szDisplay[64];
} NMDATETIMEFORMATA, *LPNMDATETIMEFORMATA;

typedef struct tagNMDATETIMEFORMATQUERYW {
    NMHDR nmhdr;
    LPCWSTR pszFormat;
    SIZE szMax;
} NMDATETIMEFORMATQUERYW, *LPNMDATETIMEFORMATQUERYW;

typedef struct tagNMDATETIMEFORMATQUERYA {
    NMHDR nmhdr;
    LPCSTR pszFormat;
    SIZE szMax;
} NMDATETIMEFORMATQUERYA, *LPNMDATETIMEFORMATQUERYA;

#endif

#endif

#endif
