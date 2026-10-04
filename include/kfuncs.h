#ifndef VELO_WRAPPED_KFUNCS_H
#define VELO_WRAPPED_KFUNCS_H

#ifdef VELO_INSIDE
#include_next <kfuncs.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <kfuncs.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
