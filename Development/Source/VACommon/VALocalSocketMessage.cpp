/*
 * VALocalSocketMessage.cpp
 *
 *  Created on: 2013-12-11
 *      Author: pffang
 */
#include "VALocalSocketMessage.h"

#if VA_VC
#pragma message ("Unix local socket can not use on Windows!")
#else

#if VA_DEBUG
#define DEBUG_TRACE printf
#else
#define DEBUG_TRACE(...) ((void)0)
#endif

namespace VA {

CLocalMessageClient::CLocalMessageClient(const char* szName)
        : socket_fd(-1) {
    assert(strlen(szName)<UNIX_PATH_MAX);
    struct sockaddr_un address = { 0 };
    socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        DEBUG_TRACE("Client: socket() failed:%s\n", strerror(errno));
        throw errno;
    }
    address.sun_family = AF_LOCAL;
    strncpy(address.sun_path, szName, UNIX_PATH_MAX-1);
//    address.sun_path[UNIX_PATH_MAX] = 0;

    if (connect(socket_fd, (struct sockaddr *) &address,
                sizeof(struct sockaddr_un)) != 0) {
        DEBUG_TRACE("Client: connect() failed:%s\n", strerror(errno));
        throw errno;
    }
}

CLocalMessageClient::~CLocalMessageClient() {
    if (socket_fd >= 0)
        close(socket_fd);
}

int CLocalMessageClient::PostMessage(UINT Msg, WPARAM wParam, LPARAM lParam) {
    assert(socket_fd >= 0);
    MSG msg;
    msg.msgtype = Msg;
    msg.wParam = wParam;
    msg.lParam = lParam;
    gettimeofday(&msg.time, NULL);
    int length = send(socket_fd, &msg, sizeof(MSG), MSG_DONTWAIT);
    return length;
}

void CLocalMessageServer::InitServiceSocket() {
    DEBUG_TRACE("Server:InitServiceSocket...\n");
    struct sockaddr_un address;

    if ((socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0) {
        DEBUG_TRACE("Server: socket() failed:%s\n", strerror(errno));
        throw(errno);
    }

    /* start with a clean address structure */
    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_LOCAL;
    strncpy(address.sun_path, m_szName.c_str(), UNIX_PATH_MAX-1);
//    address.sun_path[UNIX_PATH_MAX] = 0;

    if (bind(socket_fd, (struct sockaddr *) &address,
             sizeof(struct sockaddr_un)) != 0) {
        DEBUG_TRACE("Server: bind() failed:%s\n", strerror(errno));
        close(socket_fd);
        throw(errno);
    }

    if (listen(socket_fd, SOCK_STREAM) != 0) {
        DEBUG_TRACE("Server: listen() failed:%s\n", strerror(errno));
        close(socket_fd);
        throw(errno);
    }
}

void CLocalMessageServer::WaitForConnect() {
    struct sockaddr_in workaddr;
    socklen_t length = sizeof(workaddr);
    if (work_fd >= 0) {
        close(work_fd);
        work_fd = -1;
    }
    DEBUG_TRACE("Server:Waiting for client connect...\n");
    work_fd = accept(socket_fd, (struct sockaddr*) &workaddr, &length);
    if (work_fd < 0) {
        DEBUG_TRACE("Server: accept() failed:%s\n", strerror(errno));
    }
    DEBUG_TRACE("Server:Connect from client,IP:%s,Port:%d\n",
                inet_ntoa(workaddr.sin_addr), ntohs(workaddr.sin_port));
}

CLocalMessageServer::CLocalMessageServer(const char* szName)
        : socket_fd(-1),
          work_fd(-1) {
    assert(strlen(szName)<UNIX_PATH_MAX);
    DEBUG_TRACE("Server:CLocalMessageServer\n");
    m_szName = szName;
    if(m_szName.length() > UNIX_PATH_MAX)
        m_szName.resize(UNIX_PATH_MAX-1);
    unlink(m_szName.c_str());
    InitServiceSocket();
}

CLocalMessageServer::~CLocalMessageServer() {
    DEBUG_TRACE("Server:~CLocalMessageServer\n");
    if (work_fd >= 0) {
        close(work_fd);
        work_fd = -1;
        DEBUG_TRACE("Server:close(work_fd)\n");
    }
    if (socket_fd >= 0) {
        close(socket_fd);
        socket_fd = -1;
        DEBUG_TRACE("Server:close(socket_fd)\n");
    }
    unlink(m_szName.c_str());
}

int CLocalMessageServer::GetMessage(PMSG msg) {
    fd_set fds;
    MSG msgbuf = { 0 };
    int recvlength = 0;
    if (work_fd < 0)
        WaitForConnect();
    while (1) {
        FD_ZERO(&fds);    //每次循环都要清空集合，否则不能检测描述符变化
        FD_SET(work_fd, &fds);    //添加描述符

        int sret = 0;
        sret = select(work_fd + 1, &fds, NULL, NULL, NULL);
        if (-1 == sret) {
            throw -1;   //select错误，退出程序
            break;
        } else if (0 == sret) {
            continue;   //再次轮询
        } else {
            if (FD_ISSET(work_fd, &fds)) {     //测试work_fd是否可读，即是否网络上有数据
                memset(&msgbuf, 0, sizeof(MSG));
                recvlength = recv(work_fd, &msgbuf, sizeof(MSG), MSG_DONTWAIT);
                if (recvlength > 0) {
                    DEBUG_TRACE(
                            "Server:recv from client, msgtype:%d, wParam:%d, lParam:%ld\n",
                            msgbuf.msgtype, msgbuf.wParam, msgbuf.lParam);
                    if (WM_QUIT == msgbuf.msgtype)
                        return 0;     //退出GetMessage
                    memcpy(msg, &msgbuf, sizeof(MSG));
                } else if (recvlength == 0 || errno == ECONNRESET) {
                    DEBUG_TRACE("Server:client disconnected.\n");
                    if (work_fd >= 0) {
                        close(work_fd);
                        work_fd = -1;
                    }
                    return -1;
                } else if (recvlength < 0) {
                    DEBUG_TRACE("Server:recv error\n");
                    return -1;
                }
            }
        }
    }
    return recvlength;
}

}
/* namespace VA */
#endif
