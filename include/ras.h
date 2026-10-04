#ifndef VELO_WRAPPED_RAS_H
#define VELO_WRAPPED_RAS_H

#ifdef VELO_INSIDE
#include_next <ras.h>
#include <velo/extras-ras.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <ras.h>
#include <velo/extras-ras.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
