#ifndef UNDER_CE
#define UNDER_CE
#endif
#ifndef _MIPS_
#define _MIPS_
#endif
#ifndef MIPS
#define MIPS
#endif
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef __stdcall
#define __stdcall
#endif
#ifndef __cdecl
#define __cdecl
#endif

#ifndef VELO_CE
#if defined(_WIN32_WCE) && _WIN32_WCE >= 200
#define VELO_CE 2
#else
#define VELO_CE 1
#endif
#endif


#ifndef _WIN32_IE
#if VELO_CE >= 2
#define _WIN32_IE 0x0400
#else
#define _WIN32_IE 0x0300
#endif
#endif

#pragma push_macro("_WIN32_WCE")
#undef _WIN32_WCE
#if VELO_CE >= 2
#define _WIN32_WCE 0x200
#else
#define _WIN32_WCE 0x100
#endif
