/*
 * VASocket.cpp
 * Author: gdwang@iflytek.com, 2017-09-03
 *
 * socketͨ����ؽӿ�
 * Windowsϵͳ����Winsock��Linuxϵͳ����BSD Socket
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

/* ����socket */
VASocket_t VASocketCreate(int nDomain, int nType, int nProtocol)
{
    VASocket_t SockFd = VA_INVALID_SOCKET;

#if VA_VC
	WSADATA wsaData;                  /* �����洢ϵͳ���صĹ���Winsock����Ϣ */
	WORD sockVersion = MAKEWORD(2,0); /* ��������Winsock�汾Ϊ2.0�汾 */
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

/* �ر�socket */
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

/* �ͻ��������������socket���� */
int VASocketConnect(VASocket_t SockFd, const struct sockaddr *pSockAddr, int nSockAddrLen)
{
    int ret = VA_SOCKET_ERROR;

    /* ��κϷ���δ���м�飬����ϵͳ�ӿ������м�� */
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

    /* ��κϷ���δ���м�飬����ϵͳ�ӿ������м�� */
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

    /* ��κϷ���δ���м�飬����ϵͳ�ӿ������м�� */
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
