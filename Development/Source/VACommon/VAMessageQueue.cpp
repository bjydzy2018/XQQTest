/*
 * VAMessageQueue.cpp
 *
 *  Created on: 2013-8-22
 *      Author: pffang
 */

#include "VAMessageQueue.h"

namespace VA {
CMessageClient::~CMessageClient() {
}

#if defined __POSIX_QUEUE__ || VA_QNX
class CMessageClientImpl : public CMessageClient {

  mqd_t m_mqd;
  char * m_mqname;

  int Open();
  int Close();
public:
  CMessageClientImpl(const char* szMQName);
  ~CMessageClientImpl();

  int PostMessage(const char* pMessage, size_t nSize, unsigned int prio = 0);
};
int CMessageClientImpl::Open() {
  m_mqd = mq_open(m_mqname, O_RDWR | O_NONBLOCK | O_CREAT, 0644,
      NULL);
  if (-1 == m_mqd) {
    perror("mq_open error:");
    return errno;
  }
  struct mq_attr attr;
  mq_getattr(m_mqd, &attr);
  char *buff = new char[attr.mq_msgsize];
  ssize_t n;

  while ((n = mq_receive(m_mqd, buff, attr.mq_msgsize, NULL)) > 0) {
    //printf("erase exist message\n");
  }
  delete[] buff;
  return 0;
}

int CMessageClientImpl::Close() {
  int ret = 0;
  if (m_mqd != -1)
  ret = mq_close(m_mqd);
  if (-1 == ret) {
    perror("mq_close error:");
    return errno;
  }
  return 0;
}

CMessageClientImpl::CMessageClientImpl(const char* szMQName) {
  m_mqname = NULL;
  m_mqd = -1;
  int size = strlen(szMQName) + 2;
  m_mqname = new char[size];
  memset(m_mqname, 0, size);

  if ('/' != szMQName[0])
  m_mqname[0] = '/';
  strcat(m_mqname, szMQName);
  Open();
}

CMessageClientImpl::~CMessageClientImpl() {
  Close();
  delete[] m_mqname;
}

int CMessageClientImpl::PostMessage(const char* pMessage, size_t nSize,
    unsigned int prio) {

  int i = mq_send(m_mqd, pMessage, nSize, prio);
  if (i == -1) {
    printf("mq_send error:");
    switch (errno) {
      case EBADF:
      printf("The descriptor specified in mqdes was invalid. \n");
      Open();
      break;
      case EAGAIN:
      printf("The queue was full. \n");
      break;
      case EINTR:
      printf("The call was interrupted by a signal handler. \n");
      break;
      case EINVAL:
      printf("The call would have blocked, invalid parameter. \n");
      break;
      case EMSGSIZE:
      printf(
          "msg_len was greater than the mq_msgsize attribute of the message queue. \n");
      break;
      case ETIMEDOUT:
      printf("The call timed out before a message could be transferred. \n");
      break;
    }
    return errno;
  }
  return 0;
}

CMessageClient* CreateMessageClient(const char* szMQName) {
  if (NULL == szMQName)
  return NULL;
  int size = strlen(szMQName) + 2;
  if (size < 5) {
    printf("mq name too short\n");
    return NULL;
  } else if (size > 250) {
    printf("mq name too long\n");
    return NULL;
  }
  return new CMessageClientImpl(szMQName);
}

CMessageServer::~CMessageServer() {
}

class CMessageServerImpl : public CMessageServer {

  mqd_t m_mqd;
  char * m_mqname;
  PCallBackMessageRecv m_lpfnMessageRecv;
  bool m_bExitWaiting;
  bool m_bIsWaiting;
  char* m_msgbuff;
  size_t m_msgbuffsize;

  int Open();
  int Close();
public:
  CMessageServerImpl(const char* szMQName);
  ~CMessageServerImpl();

  int WaitingForMessage(PCallBackMessageRecv lpfnMessageRecv);
  void CancelWaiting();
};

int CMessageServerImpl::Open() {
  m_mqd = mq_open(m_mqname, O_RDWR | O_NONBLOCK | O_CREAT, 0644,
      NULL);
  if (-1 == m_mqd) {
    perror("mq_open error:");
    return errno;
  }
  struct mq_attr attr;
  mq_getattr(m_mqd, &attr);
  m_msgbuffsize = attr.mq_msgsize;
  if (m_msgbuff) {
    delete[] m_msgbuff;
    m_msgbuff = NULL;
  }
  m_msgbuff = new char[m_msgbuffsize];
  ssize_t n;
  while ((n = mq_receive(m_mqd, m_msgbuff, m_msgbuffsize, NULL)) > 0) {
    //printf("erase exist message\n");
  }
  return 0;
}

int CMessageServerImpl::Close() {
  int ret = 0;
  if (m_mqd != -1)
  ret = mq_close(m_mqd);
  if (-1 == ret) {
    perror("mq_close error:");
    return errno;
  }
  delete[] m_msgbuff;
  m_msgbuff = NULL;
  return 0;
}

CMessageServerImpl::CMessageServerImpl(const char* szMQName) {
  m_mqname = NULL;
  m_msgbuff = NULL;
  m_mqd = -1;
  m_bExitWaiting = false;
  m_lpfnMessageRecv = NULL;
  int size = strlen(szMQName) + 2;
  m_mqname = new char[size];
  memset(m_mqname, 0, size);

  if ('/' != szMQName[0])
  m_mqname[0] = '/';
  strcat(m_mqname, szMQName);
  Open();
  m_bIsWaiting = false;
}

CMessageServerImpl::~CMessageServerImpl() {
  Close();
  if (m_mqname)
  delete[] m_mqname;
}

int CMessageServerImpl::WaitingForMessage(
    PCallBackMessageRecv lpfnMessageRecv) {
  if (NULL == lpfnMessageRecv || m_bIsWaiting)
  return -1;
  m_lpfnMessageRecv = lpfnMessageRecv;

  fd_set fds;
  int nfds;
  unsigned int prio;
  ssize_t n;

  m_bIsWaiting = true;

  while (1) {
    FD_ZERO(&fds);    //每次循环都要清空集合，否则不能检测描述符变化
    FD_SET(m_mqd, &fds);//添加描述符
    nfds = select(m_mqd + 1, &fds, NULL, NULL, NULL);//select使用
    if (-1 == nfds) {      //select错误，退出程序
      perror("select error");
      m_bExitWaiting = false;
      m_bIsWaiting = false;
      return -1;
    } else if (0 == nfds) {      //再次轮询
      continue;
    } else {
      if (FD_ISSET(m_mqd, &fds))      //测试是否可读，即是否有数据
      {
        n = mq_receive(m_mqd, m_msgbuff, m_msgbuffsize, &prio);
        if (-1 == n) {
          printf("mq_receive error:");
          switch (errno) {
            case EAGAIN:
            printf("The queue was empty.");
            break;
            case EBADF:
            printf("The descriptor specified in mqdes was invalid.");
            break;
            case EINTR:
            printf("The call was interrupted by a signal handler.");
            break;
            case EINVAL:
            printf("The call would have blocked, ");
            break;
            case EMSGSIZE:
            printf(
                "msg_len was less than the mq_msgsize attribute of the message queue.");
            break;
            case ETIMEDOUT:
            printf(
                "The call timed out before a message could be transferred.");
            break;
          }
          printf("\n");
        } else {
          if (lpfnMessageRecv) {
            lpfnMessageRecv(m_msgbuff, n, prio);
          }
        }
        if (m_bExitWaiting)
        break;
      }
    }
  }
  m_bExitWaiting = false;
  m_bIsWaiting = false;
  return 0;
}

void CMessageServerImpl::CancelWaiting() {
  m_bExitWaiting = true;
}

CMessageServer* CreateMessageServer(const char* szMQName) {
  if (NULL == szMQName)
  return NULL;
  int size = strlen(szMQName) + 2;
  if (size < 5) {
    printf("mq name too short\n");
    return NULL;
  } else if (size > 250) {
    printf("mq name too long\n");
    return NULL;
  }
  return new CMessageServerImpl(szMQName);
}

int DeleteMessageQueue(const char* szMQName) {
  int size = strlen(szMQName) + 2;
  char* mqname = new char[size];
  memset(mqname, 0, size);

  if ('/' != szMQName[0])
  mqname[0] = '/';
  strcat(mqname, szMQName);
  int ret = mq_unlink(mqname);
  delete[] mqname;
  if (ret == -1) {
    perror("mq_unlink error:");
    return -1;
  }
  return 0;
}
#else

class CMessageClientImpl : public CMessageClient {

  int m_msgid;
  char * m_mqname;

  int Open();
  int Close();
 public:
  CMessageClientImpl(const char* szMQName);
  ~CMessageClientImpl();

  int PostMessage(const char* pMessage, size_t nSize);
};
int CMessageClientImpl::Open() {
  int fd;
  char filename[PATH_MAX];
  strcpy(filename, "/dev/shm");
  strcat(filename, m_mqname);
  fd = open(filename, O_RDWR | O_CREAT,
            (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
  int key;
  key = ftok(filename, 'v');
  close(fd);
  if (key < 0) {
    perror("ftok key error");
    return -1;
  }
  m_msgid = msgget(key, IPC_CREAT | 0644);
  if (-1 == m_msgid) {
    perror("msgget error:");
    return errno;
  }

  struct msqid_ds attr;
  msgctl(m_msgid, IPC_STAT, &attr);
//  printf("==IPC_STAT================\n"
//         "msg_perm.__key = %d\n",
//         attr.msg_perm.__key);
//  printf("msg_perm.uid = %d\n", attr.msg_perm.uid);
//  printf("msg_perm.gid = %d\n", attr.msg_perm.gid);
//  printf("msg_perm.cuid = %d\n", attr.msg_perm.cuid);
//  printf("msg_perm.cgid = %d\n", attr.msg_perm.cgid);
//  printf("msg_perm.mode = %#o, ", attr.msg_perm.mode);
//  printf("access permissions = %#o\n", attr.msg_perm.mode & 0777);
//  printf("msg_perm.__seq = %d\n", attr.msg_perm.__seq);
//  printf("msg_stime:%s", attr.msg_stime ? ctime(&attr.msg_stime) : "Not Set\n");
//  printf("msg_rtime = %s",
//         attr.msg_rtime ? ctime(&attr.msg_rtime) : "Not Set\n");
//  printf("msg_ctime = %s",
//         attr.msg_ctime ? ctime(&attr.msg_ctime) : "Not Set\n");
//
//  printf("msg_cbytes = %lu\n", attr.msg_cbytes);
//  printf("msg_qnum = %lu\n", attr.msg_qnum);
//  printf("msg_qbytes = %lu\n", attr.msg_qbytes);
//  printf("msg_lspid = %d\n", attr.msg_lspid);
//  printf("msg_lrpid = %d\n", attr.msg_lrpid);
//
//  struct msginfo info;
//  msgctl(m_msgid, IPC_INFO, (msqid_ds*) &info);
//  printf("\n==IPC_INFO================"
//         "\nmsgpool:%d\n",
//         info.msgpool);
//  printf("msgmap:%d\n", info.msgmap);
//  printf("msgmax:%d\n", info.msgmax);
//  printf("msgmnb:%d\n", info.msgmnb);
//  printf("msgmni:%d\n", info.msgmni);
//  printf("msgssz:%d\n", info.msgssz);
//  printf("msgtql:%d\n", info.msgtql);
//  printf("msgseg:%d\n", info.msgseg);

  char* buff = NULL;
  buff = new char[attr.msg_qbytes + sizeof(long)];
  assert(buff);
  ssize_t n;

  while ((n = msgrcv(m_msgid, buff, attr.msg_qbytes, 0, IPC_NOWAIT)) > 0) {
    //printf("erase exist message\n");
  }
  if (buff)
    delete[] buff;
  return 0;
}

int CMessageClientImpl::Close() {
  return 0;
}

CMessageClientImpl::CMessageClientImpl(const char* szMQName) {
  m_mqname = NULL;
  m_msgid = -1;
  int size = strlen(szMQName) + 2;
  m_mqname = new char[size];
  memset(m_mqname, 0, size);

  if ('/' != szMQName[0])
    m_mqname[0] = '/';
  strcat(m_mqname, szMQName);
  Open();
}

CMessageClientImpl::~CMessageClientImpl() {
  Close();
  delete[] m_mqname;
}

int CMessageClientImpl::PostMessage(const char* pMessage, size_t nSize) {
  char* buff = new char[nSize + sizeof(long)];
  long mtype = 0x4D53470D;/*MSG\n*/
  memcpy(buff, &mtype, sizeof(long));
  memcpy(buff, pMessage, nSize);

  int i = msgsnd(m_msgid, buff, nSize, 10);
  if (i == -1) {
    printf("msgsnd error:");
    switch (errno) {
      case EACCES:
        printf(
            "The calling process does not have write permission on the message queue, and does not have the CAP_IPC_OWNER capability. \n");
        break;
      case EAGAIN:
        printf(
            "The message can't be sent due to the msg_qbytes limit for the queue and IPC_NOWAIT was specified in msgflg. \n");
        break;
      case EFAULT:
        printf("The address pointed to by msgp isn't accessible. \n");
        break;
      case EIDRM:
        printf("The message queue was removed. \n");
        break;
      case EINTR:
        printf(
            "Sleeping on a full message queue condition, the process caught a signal. \n");
        break;
      case EINVAL:
        printf(
            "Invalid msqid value, or nonpositive mtype value, or invalid msgsz value. \n");
        break;
      case ENOMEM:
        printf(
            "The system does not have enough memory to make a copy of the message pointed to by msgp. \n");
        break;
    }
    return errno;
  }
  return 0;
}

CMessageClient* CreateMessageClient(const char* szMQName) {
  if (NULL == szMQName)
    return NULL;
  int size = strlen(szMQName) + 2;
  if (size < 5) {
    printf("mq name too short\n");
    return NULL;
  } else if (size > 250) {
    printf("mq name too long\n");
    return NULL;
  }
  return new CMessageClientImpl(szMQName);
}

CMessageServer::~CMessageServer() {
}

class CMessageServerImpl : public CMessageServer {

  int m_msgid;
  char * m_mqname;
  PCallBackMessageRecv m_lpfnMessageRecv;
  bool m_bExitWaiting;
  bool m_bIsWaiting;
  char* m_msgbuff;
  size_t m_msgbuffsize;

  int Open();
  int Close();
 public:
  CMessageServerImpl(const char* szMQName);
  ~CMessageServerImpl();

  int WaitingForMessage(PCallBackMessageRecv lpfnMessageRecv);
  void CancelWaiting();
};

int CMessageServerImpl::Open() {
  int fd;
  char filename[PATH_MAX];
  strcpy(filename, "/dev/shm");
  strcat(filename, m_mqname);
  fd = open(filename, O_RDWR | O_CREAT,
            (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
  int key;
  key = ftok(filename, 'v');
  close(fd);
  if (key < 0) {
    perror("ftok key error");
    return -1;
  }
  m_msgid = msgget(key, IPC_CREAT | 0644);
  if (-1 == m_msgid) {
    perror("msgget error:");
    return errno;
  }

  struct msqid_ds attr;
  msgctl(m_msgid, IPC_STAT, &attr);
//  printf("==IPC_STAT================\n"
//         "msg_perm.__key = %d\n",
//         attr.msg_perm.__key);
//  printf("msg_perm.uid = %d\n", attr.msg_perm.uid);
//  printf("msg_perm.gid = %d\n", attr.msg_perm.gid);
//  printf("msg_perm.cuid = %d\n", attr.msg_perm.cuid);
//  printf("msg_perm.cgid = %d\n", attr.msg_perm.cgid);
//  printf("msg_perm.mode = %#o, ", attr.msg_perm.mode);
//  printf("access permissions = %#o\n", attr.msg_perm.mode & 0777);
//  printf("msg_perm.__seq = %d\n", attr.msg_perm.__seq);
//  printf("msg_stime:%s", attr.msg_stime ? ctime(&attr.msg_stime) : "Not Set\n");
//  printf("msg_rtime = %s",
//         attr.msg_rtime ? ctime(&attr.msg_rtime) : "Not Set\n");
//  printf("msg_ctime = %s",
//         attr.msg_ctime ? ctime(&attr.msg_ctime) : "Not Set\n");
//
//  printf("msg_cbytes = %lu\n", attr.msg_cbytes);
//  printf("msg_qnum = %lu\n", attr.msg_qnum);
//  printf("msg_qbytes = %lu\n", attr.msg_qbytes);
//  printf("msg_lspid = %d\n", attr.msg_lspid);
//  printf("msg_lrpid = %d\n", attr.msg_lrpid);

//  struct msginfo info;
//  msgctl(m_msgid, IPC_INFO, (msqid_ds*) &info);
//  printf("\n==IPC_INFO================"
//         "\nmsgpool:%d\n",
//         info.msgpool);
//  printf("msgmap:%d\n", info.msgmap);
//  printf("msgmax:%d\n", info.msgmax);
//  printf("msgmnb:%d\n", info.msgmnb);
//  printf("msgmni:%d\n", info.msgmni);
//  printf("msgssz:%d\n", info.msgssz);
//  printf("msgtql:%d\n", info.msgtql);
//  printf("msgseg:%d\n", info.msgseg);

  m_msgbuffsize = attr.msg_qbytes;
  if (m_msgbuff) {
    delete[] m_msgbuff;
    m_msgbuff = NULL;
  }
  m_msgbuff = new char[m_msgbuffsize + sizeof(long)];
  ssize_t n;

  while ((n = msgrcv(m_msgid, m_msgbuff, attr.msg_qbytes, 0, IPC_NOWAIT)) > 0) {
    //printf("erase exist message\n");
  }
  return 0;
}

int CMessageServerImpl::Close() {
  if (m_msgbuff) {
    delete[] m_msgbuff;
    m_msgbuff = NULL;
  }
  return 0;
}

CMessageServerImpl::CMessageServerImpl(const char* szMQName) {
  m_mqname = NULL;
  m_msgbuff = NULL;
  m_msgid = -1;
  m_bExitWaiting = false;
  m_lpfnMessageRecv = NULL;
  int size = strlen(szMQName) + 2;
  m_mqname = new char[size];
  memset(m_mqname, 0, size);

  if ('/' != szMQName[0])
    m_mqname[0] = '/';
  strcat(m_mqname, szMQName);
  Open();
  m_bIsWaiting = false;
}

CMessageServerImpl::~CMessageServerImpl() {
  Close();
  if (m_mqname)
    delete[] m_mqname;
}

int CMessageServerImpl::WaitingForMessage(
    PCallBackMessageRecv lpfnMessageRecv) {
  if (NULL == lpfnMessageRecv || m_bIsWaiting)
    return -1;
  m_lpfnMessageRecv = lpfnMessageRecv;

  ssize_t n;

  m_bIsWaiting = true;
  assert(m_msgbuff);

  while (1) {
    n = msgrcv(m_msgid, m_msgbuff, m_msgbuffsize, 0, MSG_NOERROR);
    if (-1 == n) {
      printf("msgrcv error:");
      switch (errno) {
        case E2BIG:
          printf(
              "The message text length is greater than msgsz and MSG_NOERROR isn't specified in msgflg.");
          break;
        case EACCES:
          printf(
              "The calling process does not have read permission on the message queue, and does not have the CAP_IPC_OWNER capability.");
          break;
        case EAGAIN:
          printf(
              "No message was available in the queue and IPC_NOWAIT was specified in msgflg. ");
          break;
        case EFAULT:
          printf("The address pointed to by msgp isn't accessible.");
          break;
        case EIDRM:
          printf(
              "While the process was sleeping to receive a message, the message queue was removed.");
          break;
        case EINTR:
          //printf("While the process was sleeping to receive a message.");
          break;
        case EINVAL:
          printf("msgqid was invalid, or msgsz was less than 0.");
          break;
        case ENOMSG:
          printf(
              "IPC_NOWAIT was specified in msgflg and no message of the requested type existed on the message queue.");
          break;
      }
      printf("\n");
    } else {
      if (lpfnMessageRecv) {
        lpfnMessageRecv(m_msgbuff, n);
      }
    }
    if (m_bExitWaiting)
      break;
  }
  m_bExitWaiting = false;
  m_bIsWaiting = false;
  return 0;
}

void CMessageServerImpl::CancelWaiting() {
  m_bExitWaiting = true;
}

CMessageServer* CreateMessageServer(const char* szMQName) {
  if (NULL == szMQName)
    return NULL;
  int size = strlen(szMQName) + 2;
  if (size < 5) {
    printf("mq name too short\n");
    return NULL;
  } else if (size > 250) {
    printf("mq name too long\n");
    return NULL;
  }
  return new CMessageServerImpl(szMQName);
}

int DeleteMessageQueue(const char* szMQName) {
  char mqname[NAME_MAX];
  memset(mqname, 0, NAME_MAX);
  if ('/' != szMQName[0])
    mqname[0] = '/';
  strcat(mqname, szMQName);

  int fd;
  char filename[PATH_MAX];
  strcpy(filename, "/dev/shm");
  strcat(filename, mqname);
  fd = open(filename, O_RDWR | O_CREAT,
            (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
  int key;
  key = ftok(filename, 'v');
  close(fd);
  if (key < 0) {
    perror("ftok key error:");
    return -1;
  }
  int msgid = msgget(key, IPC_CREAT | 0644);
  if (-1 == msgid) {
    perror("msgget error:");
    return errno;
  }
  int ret = msgctl(msgid, IPC_RMID, NULL);
  if (-1 == ret) {
    perror("msgctl error:");
    return -1;
  }
  return 0;
}
#endif

}
/* namespace VA */
