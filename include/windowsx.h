#ifndef VELO_WRAPPED_WINDOWSX_H
#define VELO_WRAPPED_WINDOWSX_H

#ifdef VELO_INSIDE
#include_next <windowsx.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <windowsx.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
