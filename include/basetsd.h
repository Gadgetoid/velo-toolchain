#ifndef VELO_WRAPPED_BASETSD_H
#define VELO_WRAPPED_BASETSD_H

#ifdef VELO_INSIDE
#include_next <basetsd.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <basetsd.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
