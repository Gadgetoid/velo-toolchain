#ifndef VELO_WRAPPED_WINERROR_H
#define VELO_WRAPPED_WINERROR_H

#ifdef VELO_INSIDE
#include_next <winerror.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <winerror.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
