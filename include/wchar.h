#ifndef VELO_WCHAR_H
#define VELO_WCHAR_H

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 * Returns the length of a string in characters.
 *
 * @param string The string.
 * @return Number of characters, not counting the terminating null.
 */
size_t wcslen(const wchar_t *string);
/**
 * Copies a string, including the terminating null.
 *
 * @param destination Buffer that receives the copy. Must be large enough.
 * @param source String to copy.
 * @return destination.
 */
wchar_t *wcscpy(wchar_t *destination, const wchar_t *source);
/**
 * Copies up to count characters of a string.
 *
 * Pads with nulls if source is shorter than count. Doesn't add a
 * terminating null if source is count characters or longer.
 *
 * @param destination Buffer that receives the copy.
 * @param source String to copy.
 * @param count Number of characters to write.
 * @return destination.
 */
wchar_t *wcsncpy(wchar_t *destination, const wchar_t *source, size_t count);
/**
 * Appends one string to another.
 *
 * @param destination String to append to. Must have room for the result.
 * @param source String to append.
 * @return destination.
 */
wchar_t *wcscat(wchar_t *destination, const wchar_t *source);
/**
 * Appends up to count characters of one string to another.
 *
 * Always adds a terminating null.
 *
 * @param destination String to append to. Must have room for count + 1
 *        more characters.
 * @param source String to append.
 * @param count Maximum number of characters to append.
 * @return destination.
 */
wchar_t *wcsncat(wchar_t *destination, const wchar_t *source, size_t count);
/**
 * Compares two strings.
 *
 * @param first First string.
 * @param second Second string.
 * @return Negative, zero or positive as first sorts before, equal to or
 *         after second.
 */
int wcscmp(const wchar_t *first, const wchar_t *second);
/**
 * Compares up to count characters of two strings.
 *
 * @param first First string.
 * @param second Second string.
 * @param count Maximum number of characters to compare.
 * @return Negative, zero or positive as first sorts before, equal to or
 *         after second.
 */
int wcsncmp(const wchar_t *first, const wchar_t *second, size_t count);
/**
 * Compares two strings, ignoring case.
 *
 * @param first First string.
 * @param second Second string.
 * @return Negative, zero or positive as first sorts before, equal to or
 *         after second.
 */
int _wcsicmp(const wchar_t *first, const wchar_t *second);
/**
 * Compares up to count characters of two strings, ignoring case.
 *
 * @param first First string.
 * @param second Second string.
 * @param count Maximum number of characters to compare.
 * @return Negative, zero or positive as first sorts before, equal to or
 *         after second.
 */
int _wcsnicmp(const wchar_t *first, const wchar_t *second, size_t count);
/**
 * Finds the first occurrence of a character in a string.
 *
 * @param string String to search.
 * @param character Character to find. The terminating null can be found.
 * @return Pointer to the character, or NULL if not found.
 */
wchar_t *wcschr(const wchar_t *string, wchar_t character);
/**
 * Finds the last occurrence of a character in a string.
 *
 * @param string String to search.
 * @param character Character to find.
 * @return Pointer to the character, or NULL if not found.
 */
wchar_t *wcsrchr(const wchar_t *string, wchar_t character);
/**
 * Finds the first occurrence of a substring.
 *
 * @param string String to search.
 * @param substring String to find.
 * @return Pointer to the match, or NULL if not found. An empty substring
 *         matches at the start of string.
 */
wchar_t *wcsstr(const wchar_t *string, const wchar_t *substring);
/**
 * Finds the first character in a string that is in a set.
 *
 * @param string String to search.
 * @param characters Characters to look for.
 * @return Pointer to the first match, or NULL if none.
 */
wchar_t *wcspbrk(const wchar_t *string, const wchar_t *characters);
/**
 * Returns the length of the initial part of a string made only of a set of
 * characters.
 *
 * @param string String to scan.
 * @param characters Characters allowed.
 * @return Index of the first character not in the set.
 */
size_t wcsspn(const wchar_t *string, const wchar_t *characters);
/**
 * Returns the length of the initial part of a string that contains none
 * of a set of characters.
 *
 * @param string String to scan.
 * @param characters Characters to stop at.
 * @return Index of the first character from the set, or the string's
 *         length if there is none.
 */
size_t wcscspn(const wchar_t *string, const wchar_t *characters);
/**
 * Splits a string into tokens.
 *
 * Pass the string on the first call and NULL on later calls to continue
 * from where the last one stopped. Writes nulls into the string and keeps
 * its position between calls.
 *
 * @param string String to tokenise, or NULL to continue.
 * @param delimiters Characters that separate tokens. Can differ between
 *        calls.
 * @return The next token, or NULL when there are no more.
 */
wchar_t *wcstok(wchar_t *string, const wchar_t *delimiters);
/**
 * Duplicates a string into newly allocated memory.
 *
 * Free the copy with free.
 *
 * @param string String to copy.
 * @return The copy, or NULL if out of memory.
 */
wchar_t *_wcsdup(const wchar_t *string);
/**
 * Converts a string to lowercase in place.
 *
 * @param string String to convert.
 * @return string.
 */
wchar_t *_wcslwr(wchar_t *string);
/**
 * Converts a string to uppercase in place.
 *
 * @param string String to convert.
 * @return string.
 */
wchar_t *_wcsupr(wchar_t *string);
/**
 * Reverses a string in place.
 *
 * @param string String to reverse.
 * @return string.
 */
wchar_t *_wcsrev(wchar_t *string);
/**
 * Sets every character of a string to one character.
 *
 * The terminating null is kept.
 *
 * @param string String to change.
 * @param character Character to fill with.
 * @return string.
 */
wchar_t *_wcsset(wchar_t *string, wchar_t character);
/**
 * Sets up to count characters of a string to one character.
 *
 * Stops at the terminating null if that comes first.
 *
 * @param string String to change.
 * @param character Character to fill with.
 * @param count Maximum number of characters to set.
 * @return string.
 */
wchar_t *_wcsnset(wchar_t *string, wchar_t character, size_t count);
/**
 * Tests a wide character against a character class.
 *
 * @param character Character to test.
 * @param type Class mask: _UPPER, _LOWER, _DIGIT, _SPACE, _PUNCT, _CONTROL,
 *        _BLANK, _HEX or _ALPHA, or a combination.
 * @return Nonzero if the character is in the class, 0 if not.
 */
int iswctype(wint_t character, wctype_t type);
/**
 * Converts a wide character to lowercase.
 *
 * @param character Character to convert.
 * @return The lowercase character, or character unchanged if it has none.
 *
 * @note Windows CE 2.0 only.
 */
wchar_t towlower(wchar_t character);
/**
 * Converts a wide character to uppercase.
 *
 * @param character Character to convert.
 * @return The uppercase character, or character unchanged if it has none.
 *
 * @note Windows CE 2.0 only.
 */
wchar_t towupper(wchar_t character);
/**
 * Formats values into a string buffer, printf style.
 *
 * A wrapper over wsprintfW in velo::runtime: coredll has no swprintf, and
 * the SDK links it from its static C library. Same formats and limits as
 * wsprintfW: no floating point, and at most 1023 characters plus the
 * terminating null.
 *
 * @param buffer Buffer that receives the output. Must be large enough.
 * @param format Format string, followed by the values to format.
 * @return Characters written, not counting the terminating null.
 */
int swprintf(wchar_t *buffer, const wchar_t *format, ...);
/**
 * Formats values from a va_list into a string buffer, printf style.
 *
 * A wrapper over wvsprintfW in velo::runtime, with the same formats and
 * limits as swprintf.
 *
 * @param buffer Buffer that receives the output. Must be large enough.
 * @param format Format string.
 * @param arguments Values to format, from va_start.
 * @return Characters written, not counting the terminating null.
 */
int vswprintf(wchar_t *buffer, const wchar_t *format, va_list arguments);

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

#ifdef __cplusplus
}
#endif

#endif
