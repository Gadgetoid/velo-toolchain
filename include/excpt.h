#ifndef VELO_WRAPPED_EXCPT_H
#define VELO_WRAPPED_EXCPT_H

#ifdef VELO_INSIDE
#include_next <excpt.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <excpt.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
