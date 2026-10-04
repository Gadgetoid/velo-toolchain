#ifndef VELO_WRAPPED_DBGAPI_H
#define VELO_WRAPPED_DBGAPI_H

#ifdef VELO_INSIDE
#include_next <dbgapi.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <dbgapi.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
