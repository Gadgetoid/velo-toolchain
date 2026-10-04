#ifndef VELO_WRAPPED_COMMDLG_H
#define VELO_WRAPPED_COMMDLG_H

#ifdef VELO_INSIDE
#include_next <commdlg.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <commdlg.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
