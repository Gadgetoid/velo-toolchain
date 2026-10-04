#ifndef VELO_WRAPPED_PRSHT_H
#define VELO_WRAPPED_PRSHT_H

#ifdef VELO_INSIDE
#include_next <prsht.h>
#else
#include <windows.h>
#define VELO_INSIDE
#include <velo/begin.h>
#include_next <prsht.h>
#include <velo/end.h>
#undef VELO_INSIDE
#endif

#endif
