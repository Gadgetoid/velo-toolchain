#ifndef VELO_STDLIB_H
#define VELO_STDLIB_H

#include <stddef.h>

long _wtol(const wchar_t *string);
long long _wtoll(const wchar_t *string);
size_t mbstowcs(wchar_t *destination, const char *source, size_t count);
size_t wcstombs(char *destination, const wchar_t *source, size_t count);

#endif
