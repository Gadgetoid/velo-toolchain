#ifndef VELO_WCHAR_H
#define VELO_WCHAR_H

#include <stddef.h>

#ifndef VELO_WINT_T
#define VELO_WINT_T
typedef unsigned short wint_t;
typedef unsigned short wctype_t;
#endif

#define WEOF ((wint_t)0xFFFF)

#define _UPPER 0x0001
#define _LOWER 0x0002
#define _DIGIT 0x0004
#define _SPACE 0x0008
#define _PUNCT 0x0010
#define _CONTROL 0x0020
#define _BLANK 0x0040
#define _HEX 0x0080
#define _ALPHA 0x0100

size_t wcslen(const wchar_t *string);
wchar_t *wcscpy(wchar_t *destination, const wchar_t *source);
wchar_t *wcsncpy(wchar_t *destination, const wchar_t *source, size_t count);
wchar_t *wcscat(wchar_t *destination, const wchar_t *source);
wchar_t *wcsncat(wchar_t *destination, const wchar_t *source, size_t count);
int wcscmp(const wchar_t *first, const wchar_t *second);
int wcsncmp(const wchar_t *first, const wchar_t *second, size_t count);
int _wcsicmp(const wchar_t *first, const wchar_t *second);
int _wcsnicmp(const wchar_t *first, const wchar_t *second, size_t count);
wchar_t *wcschr(const wchar_t *string, wchar_t character);
wchar_t *wcsrchr(const wchar_t *string, wchar_t character);
wchar_t *wcsstr(const wchar_t *string, const wchar_t *substring);
wchar_t *wcspbrk(const wchar_t *string, const wchar_t *characters);
size_t wcsspn(const wchar_t *string, const wchar_t *characters);
size_t wcscspn(const wchar_t *string, const wchar_t *characters);
wchar_t *wcstok(wchar_t *string, const wchar_t *delimiters);
wchar_t *_wcsdup(const wchar_t *string);
wchar_t *_wcslwr(wchar_t *string);
wchar_t *_wcsupr(wchar_t *string);
wchar_t *_wcsrev(wchar_t *string);
wchar_t *_wcsset(wchar_t *string, wchar_t character);
wchar_t *_wcsnset(wchar_t *string, wchar_t character, size_t count);
int iswctype(wint_t character, wctype_t type);
wchar_t towlower(wchar_t character);
wchar_t towupper(wchar_t character);

#define iswalpha(character) iswctype(character, _ALPHA)
#define iswupper(character) iswctype(character, _UPPER)
#define iswlower(character) iswctype(character, _LOWER)
#define iswdigit(character) iswctype(character, _DIGIT)
#define iswxdigit(character) iswctype(character, _HEX)
#define iswspace(character) iswctype(character, _SPACE)
#define iswpunct(character) iswctype(character, _PUNCT)
#define iswalnum(character) iswctype(character, _ALPHA | _DIGIT)
#define iswprint(character) iswctype(character, _BLANK | _PUNCT | _ALPHA | _DIGIT)
#define iswgraph(character) iswctype(character, _PUNCT | _ALPHA | _DIGIT)
#define iswcntrl(character) iswctype(character, _CONTROL)

#endif
