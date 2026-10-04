#ifndef VELO_STDLIB_H
#define VELO_STDLIB_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Converts a wide-character decimal string to a long.
 *
 * Skips leading white space, accepts an optional sign and stops at the first
 * character that isn't a digit.
 *
 * @param string The string to convert.
 * @return The value, or 0 if nothing could be converted.
 */
long _wtol(const wchar_t *string);
/**
 * Converts a wide-character decimal string to a long long.
 *
 * Skips leading white space, accepts an optional sign and stops at the first
 * character that isn't a digit.
 *
 * @param string The string to convert.
 * @return The value, or 0 if nothing could be converted.
 */
long long _wtoll(const wchar_t *string);
/**
 * Converts a multibyte string to a wide-character string.
 *
 * The result is only null-terminated if there's room for the terminator
 * within count.
 *
 * @param destination Output buffer, or NULL to get the length needed.
 * @param source Null-terminated multibyte string.
 * @param count Size of destination, in wide characters.
 * @return Wide characters written, not counting the terminator, or
 *         (size_t)-1 if source holds an invalid multibyte character.
 */
size_t mbstowcs(wchar_t *destination, const char *source, size_t count);
/**
 * Converts a wide-character string to a multibyte string.
 *
 * The result is only null-terminated if there's room for the terminator
 * within count.
 *
 * @param destination Output buffer, or NULL to get the length needed.
 * @param source Null-terminated wide-character string.
 * @param count Size of destination, in bytes.
 * @return Bytes written, not counting the terminator, or (size_t)-1 if a
 *         character can't be converted.
 */
size_t wcstombs(char *destination, const wchar_t *source, size_t count);

#ifdef __cplusplus
}
#endif

#endif
