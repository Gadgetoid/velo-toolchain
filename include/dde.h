#ifndef VELO_WRAPPED_DDE_H
#define VELO_WRAPPED_DDE_H

#ifdef VELO_INSIDE
#include_next <dde.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <dde.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
