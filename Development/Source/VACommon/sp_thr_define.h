/**
 * @file    sp_thr_define.h
 * @brief   跨平台的一些线程定义
 *
 * 函数成功返回0, 其他情况返回错误码
 *
 * @author  jjzhuang
 * @version 1.0
 * @date    2015/12/08
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version    <th>Date        <th>Author    <th>Notes</tr>
 *  <tr> <td> 1.0       <td>2015/07/22  <td>jjzhuang  <td>create</tr>
 *  <tr> <td> 1.1       <td>2016/02/22  <td>jjzhuang  <td>使用CreateThread 代替 _beginthreadex</tr>
 * </table>
 */
#ifndef __spthread_hpp__
#define __spthread_hpp__


#ifndef WIN32

/// pthread
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdint.h>
#include <time.h>

typedef void*           sp_thread_result_t;
typedef pthread_mutex_t sp_thread_mutex_t;
typedef pthread_t       sp_thread_t;
typedef pthread_attr_t  sp_thread_attr_t;

//锁相关
#define sp_thread_mutex_init(m,a)   pthread_mutex_init(m,a)
#define sp_thread_mutex_destroy(m)  pthread_mutex_destroy(m)
#define sp_thread_mutex_lock(m)     pthread_mutex_lock(m)
#define sp_thread_mutex_unlock(m)   pthread_mutex_unlock(m)


//线程相关
#define sp_thread_attr_init(a)        pthread_attr_init(a)
#define sp_thread_attr_setdetachstate pthread_attr_setdetachstate
#define SP_THREAD_CREATE_DETACHED     PTHREAD_CREATE_DETACHED
#define sp_thread_self                pthread_self
#define sp_thread_create              pthread_create

#define SP_THREAD_CALL
typedef sp_thread_result_t (SP_THREAD_CALL * sp_thread_func_t )( void * args );

#define sp_sleep(x) sleep(x)
#define SP_THREAD_INLINE   static  __inline


//使用信号量代替条件，线程池中使用
//如果使用条件变量，多次singal无法累积
#if 0
typedef pthread_cond_t  sp_thread_cond_t;
#   define sp_thread_cond_init(c,a)    pthread_cond_init(c,a)
#   define sp_thread_cond_destroy(c)   pthread_cond_destroy(c)
#   define sp_thread_cond_wait(c,m)    pthread_cond_wait(c,m)
#   define sp_thread_cond_signal(c)    pthread_cond_signal(c)

#else

typedef sem_t sp_thread_cond_t;

SP_THREAD_INLINE int sp_thread_cond_init(sp_thread_cond_t* cond,void* attr)
{
    sem_init(cond,0,0);
    return 0;
}

SP_THREAD_INLINE int sp_thread_cond_destroy(sp_thread_cond_t* cond)
{
    sem_destroy(cond);
    return 0;
}

SP_THREAD_INLINE int sp_thread_cond_wait(sp_thread_cond_t* cond,sp_thread_mutex_t *lock)
{
    int ret = 0;
    sp_thread_mutex_unlock(lock);
    ret = sem_wait(cond);
    sp_thread_mutex_lock(lock);
    return ret;
}

SP_THREAD_INLINE int sp_thread_cond_signal(sp_thread_cond_t* cond)
{
    sem_post(cond);
    return 0;
}

#endif


#else ///////////////////////////////////////////////////////////////////////

// win32 thread
#include "windows.h"
//#include <winsock2.h>
#include <process.h>

#define SP_THREAD_INLINE            static  __inline

//线程相关
#define SP_THREAD_CALL              __stdcall
#define SP_THREAD_CREATE_DETACHED   1
#define sp_sleep(x)                 Sleep(1000*x)

#define  sp_priority_lowest         THREAD_PRIORITY_LOWEST
#define  sp_priority_below_normal   THREAD_PRIORITY_BELOW_NORMAL
#define  sp_priority_normal         THREAD_PRIORITY_NORMAL
#define  sp_priority_above_normal   THREAD_PRIORITY_ABOVE_NORMAL
#define  sp_priority_highest        THREAD_PRIORITY_HIGHEST



typedef HANDLE      sp_thread_mutex_t;
typedef HANDLE      sp_thread_cond_t;
typedef DWORD       sp_thread_attr_t;
typedef DWORD       sp_thread_id;

//_beginthreadex
//typedef unsigned    sp_thread_result_t;
//typedef unsigned    sp_thread_t;
//typedef sp_thread_result_t ( SP_THREAD_CALL * sp_thread_func_t )( void * args );


//CreateThread
typedef DWORD     sp_thread_result_t;
typedef HANDLE    sp_thread_t;
typedef sp_thread_result_t ( SP_THREAD_CALL * sp_thread_func_t )( void * args );

SP_THREAD_INLINE int sp_set_threadpriority(sp_thread_t thr,int pri)
{
    return SetThreadPriority(thr,pri);
}

SP_THREAD_INLINE sp_thread_id sp_thread_self()
{
    return GetCurrentThreadId();
}

SP_THREAD_INLINE int sp_thread_attr_init( sp_thread_attr_t * attr )
{
    *attr = 0;
    return 0;
}

SP_THREAD_INLINE int sp_thread_attr_setdetachstate( sp_thread_attr_t * attr, int detachstate )
{
    *attr |= detachstate;
    return 0;
}

SP_THREAD_INLINE int sp_thread_create( sp_thread_t * thread, sp_thread_attr_t * attr,
                                      sp_thread_func_t myfunc, void * args )
{
    
    // _beginthreadex returns 0 on an error
    //HANDLE h = (HANDLE)_beginthreadex( NULL, 0, myfunc, args, 0, thread );
    DWORD thread_id_;
    HANDLE h = CreateThread(NULL, 0, myfunc, args, 0, &thread_id_);
    if(0!= h){
        //ref-1
        //thread exit when myfunc is return
        CloseHandle(h);
    }
    *thread = (sp_thread_t)h;
    return h > 0 ? 0 : GetLastError();
}


//锁相关
SP_THREAD_INLINE int sp_thread_mutex_init( sp_thread_mutex_t * mutex, void * attr )
{
    *mutex = CreateMutex( NULL, FALSE, NULL );
    return NULL == * mutex ? GetLastError() : 0;
}

SP_THREAD_INLINE int sp_thread_mutex_destroy( sp_thread_mutex_t * mutex )
{
    int ret = CloseHandle( *mutex );
    return 0 == ret ? GetLastError() : 0;
}

SP_THREAD_INLINE int sp_thread_mutex_lock( sp_thread_mutex_t * mutex )
{
    int ret = WaitForSingleObject( *mutex, INFINITE );
    return WAIT_OBJECT_0 == ret ? 0 : GetLastError();
}

SP_THREAD_INLINE int sp_thread_mutex_unlock( sp_thread_mutex_t * mutex )
{
    int ret = ReleaseMutex( *mutex );
    return 0 != ret ? 0 : GetLastError();
}


//使用信号量代替条件，线程池中使用
//如果使用条件变量，多次signal无法累积
SP_THREAD_INLINE int sp_thread_cond_init( sp_thread_cond_t * cond, void * attr )
{
    //*cond = CreateEvent( NULL, FALSE, FALSE, NULL );
    *cond = CreateSemaphore( NULL, 0 ,INT_MAX,NULL);
    return NULL == *cond ? GetLastError() : 0;
}

SP_THREAD_INLINE int sp_thread_cond_destroy( sp_thread_cond_t * cond )
{
    int ret = CloseHandle( *cond );
    return 0 == ret ? GetLastError() : 0;
}

/*
Caller MUST be holding the mutex lock; the
lock is released and the caller is blocked waiting
on 'cond'. When 'cond' is signaled, the mutex
is re-acquired before returning to the caller.
*/
SP_THREAD_INLINE int sp_thread_cond_wait( sp_thread_cond_t * cond, sp_thread_mutex_t * mutex )
{
    int ret = 0;
    sp_thread_mutex_unlock( mutex );
    ret = WaitForSingleObject( *cond, INFINITE );
    sp_thread_mutex_lock( mutex );
    return WAIT_OBJECT_0 == ret ? 0 : GetLastError();
}

SP_THREAD_INLINE int sp_thread_cond_signal( sp_thread_cond_t * cond )
{
    int ret = ReleaseSemaphore( *cond ,1,0);
    return 0 == ret ? GetLastError() : 0;
}


#if 0
//use event
SP_THREAD_INLINE int sp_thread_cond_init( sp_thread_cond_t * cond, void * attr )
{
    *cond = CreateEvent( NULL, FALSE, FALSE, NULL );
    return NULL == *cond ? GetLastError() : 0;
}

SP_THREAD_INLINE int sp_thread_cond_destroy( sp_thread_cond_t * cond )
{
    int ret = CloseHandle( *cond );
    return 0 == ret ? GetLastError() : 0;
}

/*
Caller MUST be holding the mutex lock; the
lock is released and the caller is blocked waiting
on 'cond'. When 'cond' is signaled, the mutex
is re-acquired before returning to the caller.
*/
SP_THREAD_INLINE int sp_thread_cond_wait( sp_thread_cond_t * cond, sp_thread_mutex_t * mutex )
{
    int ret = 0;
    sp_thread_mutex_unlock( mutex );
    ret = WaitForSingleObject( *cond, INFINITE );
    sp_thread_mutex_lock( mutex );
    return WAIT_OBJECT_0 == ret ? 0 : GetLastError();
}

SP_THREAD_INLINE int sp_thread_cond_signal( sp_thread_cond_t * cond )
{
    int ret = SetEvent( *cond );
    return 0 == ret ? GetLastError() : 0;
}
#endif



#endif /* platform */

#endif  /* __spthread_hpp__ */
