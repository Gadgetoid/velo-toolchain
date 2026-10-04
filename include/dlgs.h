#ifndef VELO_WRAPPED_DLGS_H
#define VELO_WRAPPED_DLGS_H

#ifdef VELO_INSIDE
#include_next <dlgs.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <dlgs.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
