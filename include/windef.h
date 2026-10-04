#ifndef VELO_WRAPPED_WINDEF_H
#define VELO_WRAPPED_WINDEF_H

#ifdef VELO_INSIDE
#include_next <windef.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <windef.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
