/*
 * VASocket.h
 * Author: gdwang@iflytek.com, 2017-09-03
 *
 * socket通信相关接口
 *
 * History
 *
 */

#ifndef __VASOCKET_H__
#define __VASOCKET_H__

#include "VAPlatform.h"
#if VA_VC
#include <winsock.h>
#elif VA_GNUC
#include <sys/socket.h>
#endif

namespace VA {

#if VA_VC
typedef SOCKET VASocket_t;
#define VA_INVALID_SOCKET INVALID_SOCKET
#define VA_SOCKET_ERROR SOCKET_ERROR
#elif VA_GNUC
typedef int VASocket_t;
#define VA_INVALID_SOCKET (-1)
#define VA_SOCKET_ERROR (-1)
#endif

VASocket_t VASocketCreate(int nDomain, int nType, int nProtocol);
void VASocketClose(VASocket_t SockFd);
int VASocketConnect(VASocket_t SockFd, const struct sockaddr *pSockAddr, int nSockAddrLen);
int VASocketSend(VASocket_t SockFd, char *pBuf, int nLen, int nFlags, int *pErrId);
int VASocketRecv(VASocket_t SockFd, char *pBuf, int nLen, int nFlags, int *pErrId);
    
} /* namespace VA */

#endif /* __VASOCKET_H__ */