#ifndef VELO_WINSOCK_H
#define VELO_WINSOCK_H

#include <windows.h>

#define WINSOCKAPI

typedef uint32_t SOCKET;

#define INVALID_SOCKET ((SOCKET)~0u)
#define SOCKET_ERROR (-1)
#define AF_INET 2
#define SOCK_STREAM 1
#define SOCK_DGRAM 2
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17
#define SOL_SOCKET 0xFFFF
#define SO_REUSEADDR 0x0004
#define SO_BROADCAST 0x0020
#define SO_ERROR 0x1007
#define TCP_NODELAY 0x0001
#define FIONBIO 0x8004667Eu
#define FD_SETSIZE 64
#define WSAEWOULDBLOCK 10035
#define WSAEINPROGRESS 10036
#define WSAEALREADY 10037
#define WSAENOTSOCK 10038
#define WSAEADDRINUSE 10048
#define WSAEADDRNOTAVAIL 10049
#define WSAENETUNREACH 10051
#define WSAECONNABORTED 10053
#define WSAECONNRESET 10054
#define WSAEISCONN 10056
#define WSAENOTCONN 10057
#define WSAETIMEDOUT 10060
#define WSAECONNREFUSED 10061
#define WSAEHOSTUNREACH 10065
#define WSAHOST_NOT_FOUND 11001

struct in_addr {
    uint32_t s_addr;
};

struct sockaddr {
    uint16_t sa_family;
    char sa_data[14];
};

struct sockaddr_in {
    int16_t sin_family;
    uint16_t sin_port;
    struct in_addr sin_addr;
    char sin_zero[8];
};

struct hostent {
    char *h_name;
    char **h_aliases;
    int16_t h_addrtype;
    int16_t h_length;
    char **h_addr_list;
};

typedef struct {
    UINT fd_count;
    SOCKET fd_array[FD_SETSIZE];
} fd_set;

struct timeval {
    LONG tv_sec;
    LONG tv_usec;
};

SOCKET WINSOCKAPI socket(int family, int type, int protocol);
int WINSOCKAPI closesocket(SOCKET socket);
int WINSOCKAPI connect(SOCKET socket, const struct sockaddr *address, int length);
int WINSOCKAPI bind(SOCKET socket, const struct sockaddr *address, int length);
int WINSOCKAPI listen(SOCKET socket, int backlog);
SOCKET WINSOCKAPI accept(SOCKET socket, struct sockaddr *address, int *length);
int WINSOCKAPI send(SOCKET socket, const char *buffer, int length, int flags);
int WINSOCKAPI recv(SOCKET socket, char *buffer, int length, int flags);
int WINSOCKAPI sendto(SOCKET socket, const char *buffer, int length, int flags, const struct sockaddr *address, int address_length);
int WINSOCKAPI recvfrom(SOCKET socket, char *buffer, int length, int flags, struct sockaddr *address, int *address_length);
int WINSOCKAPI select(int count, fd_set *readable, fd_set *writable, fd_set *failed, const struct timeval *timeout);
int WINSOCKAPI ioctlsocket(SOCKET socket, long command, uint32_t *argument);
int WINSOCKAPI setsockopt(SOCKET socket, int level, int name, const char *value, int length);
int WINSOCKAPI getsockopt(SOCKET socket, int level, int name, char *value, int *length);
int WINSOCKAPI getsockname(SOCKET socket, struct sockaddr *address, int *length);
int WINSOCKAPI getpeername(SOCKET socket, struct sockaddr *address, int *length);
int WINSOCKAPI shutdown(SOCKET socket, int how);
struct hostent *WINSOCKAPI gethostbyname(const char *name);
int WINSOCKAPI gethostname(char *name, int length);
uint32_t WINSOCKAPI inet_addr(const char *text);
uint16_t WINSOCKAPI htons(uint16_t value);
uint16_t WINSOCKAPI ntohs(uint16_t value);
uint32_t WINSOCKAPI htonl(uint32_t value);
uint32_t WINSOCKAPI ntohl(uint32_t value);

#endif
