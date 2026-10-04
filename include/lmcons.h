#ifndef VELO_WRAPPED_LMCONS_H
#define VELO_WRAPPED_LMCONS_H

#ifdef VELO_INSIDE
#include_next <lmcons.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <lmcons.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
