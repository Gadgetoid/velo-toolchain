#ifndef VELO_WRAPPED_WINSVC_H
#define VELO_WRAPPED_WINSVC_H

#ifdef VELO_INSIDE
#include_next <winsvc.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <winsvc.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
