#ifndef VELO_WRAPPED_WINBASE_H
#define VELO_WRAPPED_WINBASE_H

#ifdef VELO_INSIDE
#include_next <winbase.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <winbase.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
