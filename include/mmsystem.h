#ifndef VELO_WRAPPED_MMSYSTEM_H
#define VELO_WRAPPED_MMSYSTEM_H

#ifdef VELO_INSIDE
#include_next <mmsystem.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <mmsystem.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
