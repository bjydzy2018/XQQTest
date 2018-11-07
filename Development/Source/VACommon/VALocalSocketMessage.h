/*
 * VALocalSocketMessage.h
 *
 *  Created on: 2013-12-11
 *      Author: pffang
 */
#pragma once
#ifndef VALOCALSOCKETMESSAGE_H_
#define VALOCALSOCKETMESSAGE_H_
#include "VACommon.h"

#if VA_VC
#pragma message ("Unix local socket can not use on Windows!")
#else

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "VADebug.h"
#include <assert.h>

namespace VA {

typedef unsigned int UINT;
typedef unsigned int WPARAM;
typedef unsigned long LPARAM;

#ifndef UNIX_PATH_MAX
#define UNIX_PATH_MAX (108)
#endif

enum E_LOCAL_SOCKET_MESSAGE{
    WM_QUIT = 0x0012,
    WM_USER = 0x0400
};

typedef struct tagMSG {
    UINT msgtype;
    WPARAM wParam;
    LPARAM lParam;
    struct timeval time;
} MSG, *PMSG;

#define VA_LSocketMsg_MaxLen (sizeof(MSG))

class CLocalMessageClient {
    int socket_fd;
 public:
    CLocalMessageClient(const char* szName);
    ~CLocalMessageClient();
    int PostMessage(UINT Msg, WPARAM wParam, LPARAM lParam);
};

class CLocalMessageServer {
    int socket_fd;
    int work_fd;
    std::string m_szName;

    void InitServiceSocket();
    void WaitForConnect();

 public:
    CLocalMessageServer(const char* szName);
    ~CLocalMessageServer();
    int GetMessage(PMSG msg);
};

#endif

} /* namespace VA */
#endif /* VALOCALSOCKETMESSAGE_H_ */
