#ifndef VELO_WRAPPED_WINNETWK_H
#define VELO_WRAPPED_WINNETWK_H

#ifdef VELO_INSIDE
#include_next <winnetwk.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <winnetwk.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
