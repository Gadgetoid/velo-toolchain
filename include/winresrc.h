#ifndef VELO_WRAPPED_WINRESRC_H
#define VELO_WRAPPED_WINRESRC_H

#ifdef VELO_INSIDE
#include_next <winresrc.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <winresrc.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
