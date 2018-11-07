/*
 * VAThread.h
 *
 */

#ifndef __VATHREAD_H__
#define __VATHREAD_H__

#include "VAPlatform.h"
#if VA_VC
#include <Windows.h>
#elif VA_GNUC
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <semaphore.h>
#endif
#include <stddef.h>
namespace VA {

enum RetType
{
	RET_SUCCESSFUL = 0,
	RET_FAIL       =-1
};
#if VA_VC

#define VA_THREAD_CALL WINAPI

/* 线程相关 */
typedef HANDLE VAThread_handle_t;
typedef DWORD  VAThread_result_t;
typedef VAThread_result_t (VA_THREAD_CALL *VAThread_func_t)(void *args);

/* 临界资源相关 */
typedef CRITICAL_SECTION VAThread_mutex_t;
typedef HANDLE           VAThread_cond_t;
typedef HANDLE           VAThread_sem_t;

#define VATHREAD_MUTEX_INIT(mutex)      InitializeCriticalSection(mutex)
#define VATHREAD_MUTEX_DESTROY(mutex)   DeleteCriticalSection(mutex)
#define VATHREAD_MUTEX_LOCK(mutex)      EnterCriticalSection(mutex)
#define VATHREAD_MUTEX_UNLOCK(mutex)    LeaveCriticalSection(mutex)

//static inline int VATHREAD_MUTEX_INIT( VAThread_mutex_t * mutex)
//{
//	InitializeCriticalSection(mutex);
//	return NULL == * mutex ? GetLastError() : 0;
//}
//
//static inline int VATHREAD_MUTEX_DESTROY( VAThread_mutex_t * mutex )
//{
//	DeleteCriticalSection(mutex);
//	return 0 == ret ? GetLastError() : 0;
//}
//
//static inline int VATHREAD_MUTEX_LOCK( VAThread_mutex_t * mutex )
//{
//	EnterCriticalSection(mutex);
//	return WAIT_OBJECT_0 == ret ? 0 : GetLastError();
//}
//
//static inline int VATHREAD_MUTEX_UNLOCK( VAThread_mutex_t * mutex )
//{
//	LeaveCriticalSection(mutex);
//	return 0 != ret ? 0 : GetLastError();
//}


#elif VA_GNUC

/* 线程相关 */

#define VA_THREAD_CALL
	
typedef pthread_t VAThread_handle_t;
typedef void*     VAThread_result_t;
typedef VAThread_result_t (VA_THREAD_CALL *VAThread_func_t)(void *args);

/* 临界资源相关 */
typedef pthread_mutex_t VAThread_mutex_t;
typedef pthread_cond_t  VAThread_cond_t;
typedef sem_t           VAThread_sem_t;

#define VATHREAD_MUTEX_INIT(mutex)      pthread_mutex_init(mutex, NULL)   /* 使用默认的互斥锁属性 */
#define VATHREAD_MUTEX_DESTROY(mutex)   pthread_mutex_destroy(mutex)
#define VATHREAD_MUTEX_LOCK(mutex)      pthread_mutex_lock(mutex)
#define VATHREAD_MUTEX_UNLOCK(mutex)    pthread_mutex_unlock(mutex)

#endif

#define VA_INFINITE (0)

int VAThreadCreate(VAThread_handle_t *thread, VAThread_func_t func, void *args, const char *thread_name);
int VAThreadCreate(VAThread_handle_t *thread, VAThread_func_t func, void *args, const char *thread_name, size_t stack_size);
int VAThreadExit(VAThread_handle_t thread);
int VAThreadCheckPriorityValid(int priority);
int VAThreadSetPriority(int priority);
int VAThreadSetPriority(VAThread_handle_t thread, int priority);
int VAThreadGetPriority(void);
int VAThreadGetThreadId(void);
int VAThreadCondInit(VAThread_cond_t *cond);
int VAThreadCondDestroy(VAThread_cond_t *cond);
int VAThreadCondWait(VAThread_cond_t *cond, VAThread_mutex_t *mutex);
int VAThreadCondTimedWait(VAThread_cond_t *cond, VAThread_mutex_t *mutex, unsigned long millisecond);
int VAThreadCondTimedWaitLock(VAThread_cond_t *cond, VAThread_mutex_t *mutex, unsigned long millisecond);
int VAThreadSetSignal(VAThread_cond_t *cond);

#include "VAThread.inl"


int VAThreadCondWait(VAThread_cond_t *cond, lock_i *mutex);
int VAThreadCondTimedWait(VAThread_cond_t *cond, lock_i *mutex, unsigned long millisecond);
int VAThreadCondTimedWaitLock(VAThread_cond_t *cond, lock_i *mutex, unsigned long millisecond);

int VAThreadSemInit(VAThread_sem_t *sem, unsigned int uMaxCnt);
int VAThreadSemDestroy(VAThread_sem_t *sem);
int VAThreadSemWait(VAThread_sem_t *sem);
int VAThreadSemPost(VAThread_sem_t *sem);

} /* namespace VA */

#endif /* __VATHREAD_H__ */
