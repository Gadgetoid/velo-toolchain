#ifndef VELO_TCHAR_H
#include <windows.h>
#endif

#ifndef VELO_TCHAR_H
#define VELO_TCHAR_H

#define _tWinMain WinMain

#ifndef __T
#define __T(text) L##text
#endif
#define _T(text) __T(text)
#define _TEXT(text) __T(text)

#define _tprintf wprintf
#define _ftprintf fwprintf
#define _stprintf swprintf
#define _sntprintf _snwprintf
#define _vtprintf vwprintf
#define _vftprintf vfwprintf
#define _vstprintf vswprintf
#define _vsntprintf _vsnwprintf
#define _tscanf wscanf
#define _ftscanf fwscanf
#define _stscanf swscanf
#define _fgettc fgetwc
#define _fgettchar fgetwchar
#define _fgetts fgetws
#define _fputtc fputwc
#define _fputtchar fputwchar
#define _fputts fputws
#define _gettc getwc
#define _getts getws
#define _puttc putwc
#define _putts putws
#define _ungettc ungetwc
#define _tcsxfrm wcsxfrm
#define _tcscoll wcscoll
#define _tcsicoll _wcsicoll

#define _ttoi _wtoi
#define _ttol _wtol
#define _ttoll _wtoll
#define _tcstod wcstod
#define _tcstol wcstol
#define _tcstoul wcstoul
#define _tcscat wcscat
#define _tcschr wcschr
#define _tcscmp wcscmp
#define _tcscpy wcscpy
#define _tcscspn wcscspn
#define _tcslen wcslen
#define _tcsncat wcsncat
#define _tcsncmp wcsncmp
#define _tcsncpy wcsncpy
#define _tcspbrk wcspbrk
#define _tcsrchr wcsrchr
#define _tcsspn wcsspn
#define _tcsstr wcsstr
#define _tcstok wcstok
#define _tcslwr _wcslwr
#define _tcsupr _wcsupr
#define _tcsdup _wcsdup
#define _tcsicmp _wcsicmp
#define _tcsnicmp _wcsnicmp
#define _tcsnset _wcsnset
#define _tcsrev _wcsrev
#define _tcsset _wcsset
#define _totupper towupper
#define _totlower towlower

#define _istlegal(character) (1)
#define _istalpha iswalpha
#define _istupper iswupper
#define _istlower iswlower
#define _istdigit iswdigit
#define _istxdigit iswxdigit
#define _istspace iswspace
#define _istpunct iswpunct
#define _istalnum iswalnum
#define _istprint iswprint
#define _istgraph iswgraph
#define _istcntrl iswcntrl
#define _istascii iswascii

#ifndef RC_INVOKED

#ifdef __cplusplus
extern "C" {
#endif

#if VELO_CE == 1
typedef const TCHAR *PCTSTR;
#endif

struct _iobuf;

int wprintf(const wchar_t *format, ...);
int fwprintf(struct _iobuf *stream, const wchar_t *format, ...);
int _snwprintf(wchar_t *buffer, size_t count, const wchar_t *format, ...);
int vwprintf(const wchar_t *format, va_list arguments);
int vfwprintf(struct _iobuf *stream, const wchar_t *format, va_list arguments);
int _vsnwprintf(wchar_t *buffer, size_t count, const wchar_t *format, va_list arguments);
int wscanf(const wchar_t *format, ...);
int fwscanf(struct _iobuf *stream, const wchar_t *format, ...);
int swscanf(const wchar_t *input, const wchar_t *format, ...);
wint_t fgetwc(struct _iobuf *stream);
wint_t fgetwchar(void);
wchar_t *fgetws(wchar_t *buffer, int count, struct _iobuf *stream);
wint_t fputwc(wint_t character, struct _iobuf *stream);
wint_t fputwchar(wint_t character);
int fputws(const wchar_t *string, struct _iobuf *stream);
wint_t getwc(struct _iobuf *stream);
wchar_t *getws(wchar_t *buffer);
wint_t putwc(wint_t character, struct _iobuf *stream);
int putws(const wchar_t *string);
wint_t ungetwc(wint_t character, struct _iobuf *stream);
size_t wcsxfrm(wchar_t *destination, const wchar_t *source, size_t count);
int wcscoll(const wchar_t *first, const wchar_t *second);
int _wcsicoll(const wchar_t *first, const wchar_t *second);
double wcstod(const wchar_t *string, wchar_t **end);
long wcstol(const wchar_t *string, wchar_t **end, int base);
unsigned long wcstoul(const wchar_t *string, wchar_t **end, int base);
int iswascii(wint_t character);

#ifdef __cplusplus
}
#endif

#if !defined(VELO_INSIDE) && !defined(VELO_ALL_DECLARATIONS)
#include <velo/unavailable.h>
#endif

#endif

#endif
