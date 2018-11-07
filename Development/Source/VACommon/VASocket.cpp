/*
 * VASocket.cpp
 * Author: gdwang@iflytek.com, 2017-09-03
 *
 * socket通信相关接口
 * Windows系统采用Winsock，Linux系统采用BSD Socket
 *
 * History
 *
 */
#include "./VASocket.h"
#if VA_VC
#include <winsock.h>
#elif VA_GNUC
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#endif

#if VA_VC
#if !VA_VC_WINCE
#pragma comment(lib, "Ws2_32.lib" )
#endif
#endif

namespace VA {

/* 创建socket */
VASocket_t VASocketCreate(int nDomain, int nType, int nProtocol)
{
    VASocket_t SockFd = VA_INVALID_SOCKET;

#if VA_VC
	WSADATA wsaData;                  /* 用来存储系统传回的关于Winsock的信息 */
	WORD sockVersion = MAKEWORD(2,0); /* 声明调用Winsock版本为2.0版本 */
	if (WSAStartup(sockVersion, &wsaData) != 0) {
        return VA_INVALID_SOCKET;
    }

    SockFd = socket(nDomain, nType, nProtocol);
    if (SockFd == VA_INVALID_SOCKET) {
        WSACleanup();
    }
#elif VA_GNUC
    SockFd = socket(nDomain, nType, nProtocol);
#endif

    return SockFd;
}

/* 关闭socket */
void VASocketClose(VASocket_t SockFd)
{
#if VA_VC
    closesocket(SockFd);
    WSACleanup();
#elif VA_GNUC
    close(SockFd);
#endif

    return;
}

/* 客户端与服务器建立socket连接 */
int VASocketConnect(VASocket_t SockFd, const struct sockaddr *pSockAddr, int nSockAddrLen)
{
    int ret = VA_SOCKET_ERROR;

    /* 入参合法性未进行检查，依赖系统接口来进行检查 */
    ret = connect(SockFd, pSockAddr, nSockAddrLen);
#if VA_VC
    if (ret == VA_SOCKET_ERROR) {
        ret = WSAGetLastError();
    }
#elif VA_GNUC
    if (ret < 0) {
        ret = errno;
    }
#endif

    return ret;
}

int VASocketSend(VASocket_t SockFd, char *pBuf, int nLen, int nFlags, int *pErrId)
{
    int ret = VA_SOCKET_ERROR;

    /* 入参合法性未进行检查，依赖系统接口来进行检查 */
    ret = send(SockFd, pBuf, nLen, nFlags);
    if (pErrId) {
#if VA_VC
        if (ret == VA_SOCKET_ERROR) {
            *pErrId = WSAGetLastError();
        }
#elif VA_GNUC
        if (ret < 0) {
            *pErrId = errno;
        }
#endif
    }

    return ret;
}

int VASocketRecv(VASocket_t SockFd, char *pBuf, int nLen, int nFlags, int *pErrId)
{
    int ret = VA_SOCKET_ERROR;

    /* 入参合法性未进行检查，依赖系统接口来进行检查 */
    ret = recv(SockFd, pBuf, nLen, nFlags);
    if (pErrId) {
#if VA_VC
        if (ret == VA_SOCKET_ERROR) {
            *pErrId = WSAGetLastError();
        }
#elif VA_GNUC
        if (ret < 0) {
            *pErrId = errno;
        }
#endif
    }

    return ret;
}
    
} /* namespace VA */
