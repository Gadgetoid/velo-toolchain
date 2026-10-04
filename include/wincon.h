#ifndef VELO_WRAPPED_WINCON_H
#define VELO_WRAPPED_WINCON_H

#ifdef VELO_INSIDE
#include_next <wincon.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <wincon.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
