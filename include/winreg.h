#ifndef VELO_WRAPPED_WINREG_H
#define VELO_WRAPPED_WINREG_H

#ifdef VELO_INSIDE
#include_next <winreg.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <winreg.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
