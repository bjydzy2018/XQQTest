/*
 * VAMessageQueue.h
 *
 *  Created on: 2013-8-22
 *      Author: pffang
 */

#ifndef VAMESSAGEQUEUE_H_
#define VAMESSAGEQUEUE_H_

#include "VACommon.h"

#if defined __POSIX_QUEUE__ || VA_QNX
#include <mqueue.h>
#else
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#endif

namespace VA {

#if defined __POSIX_QUEUE__ || VA_QNX
class CMessageClient {
 public:
  virtual ~CMessageClient()=0;
  virtual int PostMessage(const char* pMessage, size_t nSize,
                          unsigned int prio = 0)=0;
};
CMessageClient* CreateMessageClient(const char* szMQName);

typedef int (*PCallBackMessageRecv)(const char * pMessage, size_t nSize,
                                    unsigned int prio);
class CMessageServer {
 public:
  virtual ~CMessageServer()=0;
  virtual int WaitingForMessage(PCallBackMessageRecv lpfnMessageRecv)=0;
  virtual void CancelWaiting()=0;
};
#else
class CMessageClient {
 public:
  virtual ~CMessageClient()=0;
  virtual int PostMessage(const char* pMessage, size_t nSize)=0;
};
CMessageClient* CreateMessageClient(const char* szMQName);

typedef int (*PCallBackMessageRecv)(const char * pMessage, size_t nSize);
class CMessageServer {
 public:
  virtual ~CMessageServer()=0;
  virtual int WaitingForMessage(PCallBackMessageRecv lpfnMessageRecv)=0;
  virtual void CancelWaiting()=0;
};
#endif
CMessageServer* CreateMessageServer(const char* szMQName);
int DeleteMessageQueue(const char* szMQName);

} /* namespace VA */
#endif /* VAMESSAGEQUEUE_H_ */
