#ifndef VELO_WRAPPED_WINNLS_H
#define VELO_WRAPPED_WINNLS_H

#ifdef VELO_INSIDE
#include_next <winnls.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <winnls.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
