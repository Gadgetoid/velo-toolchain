#ifndef VELO_EXTRAS_MSACM_H
#define VELO_EXTRAS_MSACM_H

#ifndef RC_INVOKED

DWORD WINAPI acmGetVersion(void);
MMRESULT WINAPI acmDriverID(HACMOBJ hao, LPHACMDRIVERID phadid, DWORD fdwDriverID);
MMRESULT WINAPI acmDriverPriority(HACMDRIVERID hadid, DWORD dwPriority, DWORD fdwPriority);
LRESULT WINAPI acmDriverMessage(HACMDRIVER had, UINT uMsg, LPARAM lParam1, LPARAM lParam2);
MMRESULT WINAPI acmStreamMessage(HACMSTREAM has, UINT uMsg, LPARAM lParam1, LPARAM lParam2);
MMRESULT WINAPI acmFormatDetails(HACMDRIVER had, LPACMFORMATDETAILSW pafd, DWORD fdwDetails);
MMRESULT WINAPI acmFormatTagDetails(HACMDRIVER had, LPACMFORMATTAGDETAILSW paftd, DWORD fdwDetails);

#endif

#endif
