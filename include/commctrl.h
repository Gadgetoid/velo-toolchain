#ifndef VELO_WRAPPED_COMMCTRL_H
#define VELO_WRAPPED_COMMCTRL_H

#ifdef VELO_INSIDE
#include_next <commctrl.h>
#include <velo/extras-commctrl.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <commctrl.h>
#include <velo/extras-commctrl.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
