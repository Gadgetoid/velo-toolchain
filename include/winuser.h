#ifndef VELO_WRAPPED_WINUSER_H
#define VELO_WRAPPED_WINUSER_H

#ifdef VELO_INSIDE
#include_next <winuser.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <winuser.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
