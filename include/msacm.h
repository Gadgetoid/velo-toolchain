#ifndef VELO_WRAPPED_MSACM_H
#define VELO_WRAPPED_MSACM_H

#ifdef VELO_INSIDE
#include_next <msacm.h>
#include <velo/extras-msacm.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <msacm.h>
#include <velo/extras-msacm.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
