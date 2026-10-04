#ifndef VELO_WRAPPED_WINDOWS_H
#define VELO_WRAPPED_WINDOWS_H

#ifdef VELO_INSIDE
#include_next <windows.h>
#ifndef RC_INVOKED
#include <stdint.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <windbase.h>
#if VELO_CE >= 2
#include <tchar.h>
#endif
#endif
#include <velo/extras.h>
#else
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <windows.h>
#ifndef RC_INVOKED
#include <stdint.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <windbase.h>
#if VELO_CE >= 2
#include <tchar.h>
#endif
#endif
#include <velo/extras.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
