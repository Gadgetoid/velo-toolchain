#ifndef VELO_WRAPPED_WINSOCK_H
#define VELO_WRAPPED_WINSOCK_H

#ifdef VELO_INSIDE
#include_next <winsock.h>
#include <velo/extras-winsock.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <winsock.h>
#include <velo/extras-winsock.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
