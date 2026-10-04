#ifndef VELO_WRAPPED_WINNT_H
#define VELO_WRAPPED_WINNT_H

#ifdef VELO_INSIDE
#include_next <winnt.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <winnt.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
