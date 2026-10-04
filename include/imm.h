#ifndef VELO_WRAPPED_IMM_H
#define VELO_WRAPPED_IMM_H

#ifdef VELO_INSIDE
#include_next <imm.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <imm.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
