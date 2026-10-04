#ifndef VELO_WRAPPED_TLHELP32_H
#define VELO_WRAPPED_TLHELP32_H

#ifdef VELO_INSIDE
#include_next <tlhelp32.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <tlhelp32.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
