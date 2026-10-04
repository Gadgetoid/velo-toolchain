#ifndef VELO_WRAPPED_SHELLAPI_H
#define VELO_WRAPPED_SHELLAPI_H

#ifdef VELO_INSIDE
#include_next <shellapi.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <shellapi.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
