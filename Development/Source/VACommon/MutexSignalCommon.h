#ifndef __MUTEX_SIGNAL_COMMON_H__
#define __MUTEX_SIGNAL_COMMON_H__

#include "VAInc.h"
#if VA_GNUC
#include <sys/time.h>
#include <pthread.h>
#elif VA_VC
#include <Windows.h>
#endif

namespace VA {

#if VA_GNUC
#define INFINITE 0xffffffff
void EnterMutex(pthread_mutex_t* pMutex);
void LeaveMutex(pthread_mutex_t *pMutex);
void SetSignal(pthread_cond_t* pCond);
void PthreadSleep(pthread_cond_t * cond, pthread_mutex_t * mutex,
                  unsigned long int millisecond);
#elif VA_VC
void EnterMutex(LPCRITICAL_SECTION lpCriticalSection);
void LeaveMutex(LPCRITICAL_SECTION lpCriticalSection);
void SetSignal(HANDLE* pEventHandle);
#endif

}

#endif