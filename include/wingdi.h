#ifndef VELO_WRAPPED_WINGDI_H
#define VELO_WRAPPED_WINGDI_H

#ifdef VELO_INSIDE
#include_next <wingdi.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <wingdi.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
