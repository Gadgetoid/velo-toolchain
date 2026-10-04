#ifndef VELO_WRAPPED_MSWSOCK_H
#define VELO_WRAPPED_MSWSOCK_H

#ifdef VELO_INSIDE
#include_next <mswsock.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <mswsock.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
