#ifndef VELO_EXTRAS_WINSOCK_H
#define VELO_EXTRAS_WINSOCK_H

#ifndef RC_INVOKED

int WINAPI WSAIoctl(SOCKET s, DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer, LPVOID lpvOutBuffer, DWORD cbOutBuffer,
                    LPDWORD lpcbBytesReturned, LPVOID lpOverlapped, LPVOID lpCompletionRoutine);

#endif

#endif
