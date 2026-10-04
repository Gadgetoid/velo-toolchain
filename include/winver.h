#ifndef VELO_WRAPPED_WINVER_H
#define VELO_WRAPPED_WINVER_H

#ifdef VELO_INSIDE
#include_next <winver.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <winver.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
