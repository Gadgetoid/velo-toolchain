#ifndef VELO_WRAPPED_NOTIFY_H
#define VELO_WRAPPED_NOTIFY_H

#ifdef VELO_INSIDE
#include_next <notify.h>
#include <velo/extras-notify.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <notify.h>
#include <velo/extras-notify.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
