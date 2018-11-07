/*
 * VAThread.cpp
 *
 */
#include <stdio.h>
#include "VAThread.h"

namespace VA {

/*
 * VAThreadCreate - 创建线程
 * 
 * @thread: [out]指向线程标识符的指针，对于VA_VC，为线程句柄，对于VA_GNUC，为线程ID
 * @func: [in]指向线程运行函数的指针
 * @args: [in]传递给线程运行函数的参数
 * @thread_name: [in]线程名称（目前只对VA_GNUC生效，对VA_VC不生效）
 *
 * 创建线程，并可指定线程名称
 *
 * @return: 成功返回0，否则返回错误码
 */
int VAThreadCreate(VAThread_handle_t *thread, VAThread_func_t func, void *args, const char *thread_name)
{
#if VA_VC
	*thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, args, 0, NULL);
	if (*thread  == NULL) {
		return GetLastError();
	} else {
#if 0
		if (set_priority) {
			SetThreadPriority(*thread, priority);
		}
		ResumeThread(*thread);
#endif
		return 0;
	}
#elif VA_GNUC
/* 验证未能生效，继承属性未能修改为PTHREAD_EXPLICIT_SCHED */
#if 0
    if (set_priority) {
        pthread_attr_t thread_attr;
        struct sched_param schedule_param;

        pthread_attr_init(&thread_attr);
        schedule_param.sched_priority = priority;
        pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED); /* 设置继承属性 */
        pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);                /* 设置调度策略 */
        pthread_attr_setschedparam(&thread_attr, &schedule_param); 
        int ret = pthread_create(thread, &thread_attr, func, args);
		pthread_attr_destroy(&thread_attr);
		return ret;
    }
#endif
	int ret = pthread_create(thread, NULL, func, args);
	if ((ret == 0) && (thread_name != NULL)) {
#ifndef VA_ANDROID
		pthread_setname_np(*thread, thread_name);
#endif
	}
	return ret;
#endif
}

int VAThreadCreate(VAThread_handle_t *thread, VAThread_func_t func, void *args, const char *thread_name, size_t stack_size)
{
#if VA_VC
	*thread = CreateThread(NULL, stack_size, (LPTHREAD_START_ROUTINE)func, args, 0, NULL);
	if (*thread  == NULL) {
		return GetLastError();
	} else {
#if 0
		if (set_priority) {
			SetThreadPriority(*thread, priority);
		}
		ResumeThread(*thread);
#endif
		return 0;
	}
#elif VA_GNUC
	/* 验证未能生效，继承属性未能修改为PTHREAD_EXPLICIT_SCHED */
#if 0
	if (set_priority) {
		pthread_attr_t thread_attr;
		struct sched_param schedule_param;

		pthread_attr_init(&thread_attr);
		schedule_param.sched_priority = priority;
		pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED); /* 设置继承属性 */
		pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);                /* 设置调度策略 */
		pthread_attr_setschedparam(&thread_attr, &schedule_param); 
		int ret = pthread_create(thread, &thread_attr, func, args);
		pthread_attr_destroy(&thread_attr);
		return ret;
	}
#endif
	size_t new_size = (size_t)stack_size;
	size_t stacksize = 0;
	pthread_attr_t thread_attr;
	pthread_attr_init(&thread_attr);
	pthread_attr_setstacksize(&thread_attr,new_size);
	pthread_attr_getstacksize(&thread_attr,&stacksize);
	printf("stacksize: %d.\n", stacksize);
	int ret = pthread_create(thread, &thread_attr, func, args);
	if ((ret == 0) && (thread_name != NULL)) {
#ifndef VA_ANDROID
		pthread_setname_np(*thread, thread_name);
#endif
	}
	return ret;
#endif
}

/*
 * VAThreadExit - 退出线程
 * 
 * @thread: [in]线程描述符，对于VA_VC，为线程句柄，对于VA_GNUC，为线程ID
 *
 * 阻塞等待线程退出，并回收线程资源
 *
 * @return: 成功返回0，入参检查失败返回-1，其他返回错误码
 */
int VAThreadExit(VAThread_handle_t thread)
{
#if VA_VC
	if (thread != NULL && thread != INVALID_HANDLE_VALUE) {
		DWORD dw = WaitForSingleObject(thread, INFINITE);
		if(dw == WAIT_OBJECT_0) {
			CloseHandle(thread);
			thread = NULL;
			return 0;
		}
		return GetLastError();
	}
#elif VA_GNUC
	if (thread) {
		int ret = pthread_join(thread, NULL);    /* 未关心退出线程的返回值 */
		return ret;
	}
#endif

	return -1;
}

/*
 * VAThreadCheckPriorityValid - 检查线程优先级值是否合法
 * 
 * @priority: [in]指定线程优先级值。
 *            对于VA_VC，可为THREAD_PRIORITY_IDLE, THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_BELOW_NORMAL, 
 *            THREAD_PRIORITY_NORMAL, THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_HIGHEST, THREAD_PRIORITY_TIME_CRITICAL
 *            对于VA_QNX，可为1 ~ 63，数值越低代表有较高的优先次序，执行会较频繁
 *            对于VA_GNUC，可为-20 ~ 19，数值越低代表有较高的优先次序，执行会较频繁
 *
 * 检查线程优先级值是否合法
 *
 * @return: 成功返回0，否则返回-1
 */
int VAThreadCheckPriorityValid(int priority)
{
#if VA_VC
	switch (priority) {
	case THREAD_PRIORITY_IDLE:
	case THREAD_PRIORITY_LOWEST:
	case THREAD_PRIORITY_BELOW_NORMAL:
	case THREAD_PRIORITY_NORMAL:
	case THREAD_PRIORITY_ABOVE_NORMAL:
	case THREAD_PRIORITY_HIGHEST:
	case THREAD_PRIORITY_TIME_CRITICAL:
		return 0;
	default:
		return -1;
	}
#elif VA_QNX
	if (priority > 63 || priority < 1) {
		return -1;
	} else {
		return 0;
	}
#elif VA_GNUC
	if (priority > 19 || priority <-20) {
		return -1;
	} else {
		return 0;
	}
#endif
}

/*
 * VAThreadSetPriority - 设置线程优先级
 * 
 * @priority: [in]指定线程优先级值。
 *            对于VA_VC，可为THREAD_PRIORITY_IDLE, THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_BELOW_NORMAL, 
 *            THREAD_PRIORITY_NORMAL, THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_HIGHEST, THREAD_PRIORITY_TIME_CRITICAL
 *            对于VA_GNUC，可为-20 ~ 19，数值越低代表有较高的优先次序，执行会较频繁
 *
 * 设置线程优先级，需在线程函数内部调用
 *
 * @return: 成功返回0，否则返回错误码
 */
int VAThreadSetPriority(int priority)
{
	int ret = -1;

#if VA_VC
	ret = SetThreadPriority(GetCurrentThread(), priority);
	ret = (ret == FALSE) ? GetLastError() : 0;
	ResumeThread(GetCurrentThread());
#elif VA_QNX
	ret = setprio(0, priority);
#elif VA_GNUC
	ret = setpriority(PRIO_PROCESS, 0, priority);
#endif

	return ret;
}

int VAThreadSetPriority(VAThread_handle_t thread, int priority)
{
	int ret = -1;
#if VA_VC
	ret = SetThreadPriority(thread, priority);
	ret = (ret == FALSE) ? GetLastError() : 0;
	ResumeThread(thread);
#elif VA_QNX
	ret = pthread_setschedprio(thread, priority);
#endif
	return ret;
}
/*
 * VAThreadGetPriority - 获取线程优先级
 * 
 * 获取线程优先级，需在线程函数内部调用
 *
 * @return: 返回线程优先级
 */
int VAThreadGetPriority(void)
{
#if VA_VC
	return GetThreadPriority(GetCurrentThread());
#elif VA_QNX
	return getprio(0);
#elif VA_GNUC
	return getpriority(PRIO_PROCESS, 0);
#endif
}

/*
 * VAThreadGetThreadId - 获取线程ID
 * 
 * 获取当前线程标识符，需在线程函数内部调用
 *
 * @return: 成功返回线程ID
 */
int VAThreadGetThreadId(void)
{
#if VA_VC
	return GetCurrentThreadId();
#elif VA_GNUC
	return pthread_self();
#endif
}

/***************************************************************
 * function name:  VAThreadCondInit
 * function desc:  初始化条件变量。这里把条件变量设置成为
                   CLOCK_MONOTONIC，去除与系统时间关联。               
 * function   in:  cond,线程句柄
 * function  out: 
 * return       : 0,successful,其他，错误
 **************************************************************/
int VAThreadCondInit(VAThread_cond_t *cond)
{
#if VA_VC
	*cond = CreateEvent(NULL, FALSE, FALSE, NULL);
	return (*cond == INVALID_HANDLE_VALUE) ? GetLastError() : 0;
#elif VA_GNUC
	
    #if VA_ANDROID
	    //安卓平台
	    return pthread_cond_init(cond,NULL);
	#else
	   //linux平台
	   pthread_condattr_t cattr;
	   int ret = pthread_condattr_init(&(cattr));  
	   if (ret != RET_SUCCESSFUL)  
	   {       
	        return (RET_FAIL);  
	   }

	   ret = pthread_condattr_setclock(&(cattr), CLOCK_MONOTONIC);
	   if(ret != RET_SUCCESSFUL)
	   {
	        return (RET_FAIL);
	   }
	   return pthread_cond_init(cond,  &(cattr));
    #endif
	
#endif
}

int VAThreadCondDestroy(VAThread_cond_t *cond)
{
#if VA_VC
	int ret = CloseHandle(*cond);
	return (ret == FALSE) ? GetLastError() : 0;
#elif VA_GNUC
	return pthread_cond_destroy(cond);
#endif
}

/* 条件等待 */
int VAThreadCondWait(VAThread_cond_t *cond, VAThread_mutex_t *mutex)
{
	int ret;

#if VA_VC
	ResetEvent(*cond);
	ret = WaitForSingleObject(*cond, INFINITE);
	ret = (ret != WAIT_OBJECT_0) ? GetLastError() : 0;
#elif VA_GNUC
	VATHREAD_MUTEX_LOCK(mutex);
	ret = pthread_cond_wait(cond, mutex);
	VATHREAD_MUTEX_UNLOCK(mutex);
#endif

	return ret;
}

/***************************************************************
 * function name: VAThreadCondTimedWait
 * function desc: 计时等待，线程中计时条件等待线程唤醒
                  在线程等待并且在等待时间内没设置信号量的时候，
				  线程唤醒。调用的时候没有加解锁操作。
 * function   in:  cond,条件变量
                   mutex，锁
                   millisecond，等待时间单位为ms
 * function  out: 
 * return       : 0,successful,其他，错误
 **************************************************************/
int VAThreadCondTimedWait(VAThread_cond_t *cond, VAThread_mutex_t *mutex, unsigned long millisecond)
{
#if VA_GNUC
	struct timespec timeToWait;
	struct timespec now;
    struct timeval cur_time;
	unsigned long s, us;
#endif
	int ret;


#if VA_VC
	ResetEvent(*cond);
	ret = WaitForSingleObject(*cond, millisecond);
	ret = (ret != WAIT_OBJECT_0) ? GetLastError() : 0;
#elif VA_GNUC
	#if VA_ANDROID
        //安卓平台
	    gettimeofday(&cur_time, NULL);
	    us = cur_time.tv_usec + 1000UL * millisecond;
	    s = us / 1000000UL;
	    us = us - s * 1000000UL;
	    timeToWait.tv_sec = cur_time.tv_sec + s;
	    timeToWait.tv_nsec = us * 1000UL;
	    VATHREAD_MUTEX_LOCK(mutex);
	      ret = pthread_cond_timedwait(cond, mutex, &timeToWait);
	    VATHREAD_MUTEX_UNLOCK(mutex); 

    #else
	    //linux平台
		unsigned long int ulquotient  = 0;  //商
		unsigned long int ulremainder  = 0;     //余数
	    clock_gettime(CLOCK_MONOTONIC, &now);
		if (now.tv_nsec > 0)
		{
			ulquotient = now.tv_nsec/1000;  //us
            ulremainder = now.tv_nsec%1000; //ns
		}
		else
		{
            ulquotient = 0;
			ulremainder = 0;
		}

		us = ulquotient + 1000UL * millisecond; //us
        s = us/1000000UL;                       //s
		us = us - s*1000000UL;                  //remainder us

	    timeToWait.tv_sec = now.tv_sec + s;
	    timeToWait.tv_nsec = us * 1000UL + ulremainder;//ns
	    VATHREAD_MUTEX_LOCK(mutex);
	       ret = pthread_cond_timedwait(cond, mutex, &timeToWait);
	    VATHREAD_MUTEX_UNLOCK(mutex);

    #endif
	    
#endif

	return ret;
}

/***************************************************************
 * function name: VAThreadCondTimedWaitLock
 * function desc: 计时等待，线程中计时条件等待线程唤醒
                  在线程等待并且在等待时间内没设置信号
				  量的时候，线程唤醒。调用的时候有加解锁操作。
 * function   in:  cond,条件变量
                   mutex，锁
                   millisecond，等待时间单位为ms
 * function  out: 
 * return       : 0,successful,其他，错误
 **************************************************************/
int VAThreadCondTimedWaitLock(VAThread_cond_t *cond, VAThread_mutex_t *mutex, unsigned long millisecond)
{
#if VA_GNUC
	struct timespec timeToWait;
	struct timespec now;
	struct timeval cur_time;
	unsigned long s, us,ns;
#endif
	int ret;

#if VA_VC
	ResetEvent(*cond);
	VATHREAD_MUTEX_UNLOCK(mutex);
	//ResetEvent(*cond);
	ret = WaitForSingleObject(*cond, (millisecond == VA_INFINITE)? INFINITE:millisecond);
	ret = (ret != WAIT_OBJECT_0) ? GetLastError() : 0;
	VATHREAD_MUTEX_LOCK(mutex);
#elif VA_GNUC
	if(millisecond == VA_INFINITE)
	{
		ret = pthread_cond_wait(cond, mutex);
	}
	else
	{   
         #if VA_ANDROID
		   //安卓平台
		     gettimeofday(&cur_time, NULL);
		     us = cur_time.tv_usec + 1000UL * millisecond;
		     s = us / 1000000UL;
		     us = us - s * 1000000UL;
		     timeToWait.tv_sec = cur_time.tv_sec + s;
		     timeToWait.tv_nsec = us * 1000UL;
		     ret = pthread_cond_timedwait(cond, mutex, &timeToWait);

         #else
		   //linux平台

			 unsigned long int ulquotient  = 0;  //商
			 unsigned long int ulremainder  = 0;     //余数
		     clock_gettime(CLOCK_MONOTONIC, &now);
			 if (now.tv_nsec > 0)
			 {
				 ulquotient = now.tv_nsec/1000;  //us
				 ulremainder = now.tv_nsec%1000; //ns
			 }
			 else
			 {
				 ulquotient = 0;
				 ulremainder = 0;
			 }

			 us = ulquotient + 1000UL * millisecond; //us
			 s = us/1000000UL;                       //s
			 us = us - s*1000000UL;                  //remainder us

		     timeToWait.tv_sec = now.tv_sec + s;
			 timeToWait.tv_nsec = us * 1000UL + ulremainder;//ns
		     ret = pthread_cond_timedwait(cond, mutex, &timeToWait);

         #endif
       
	}
#endif

	return ret;
}

int VAThreadSetSignal(VAThread_cond_t *cond)
{
#if VA_VC
	int ret = SetEvent(*cond);
	return (ret != TRUE) ? GetLastError() : 0;
#elif VA_GNUC
	return pthread_cond_signal(cond);
#endif
}


int VAThreadCondWait(VAThread_cond_t *cond, lock_i *mutex)
{
	VAThread_mutex_t* pTmpMutex = mutex->get_mutex();
	return VAThreadCondWait(cond, pTmpMutex);
}
int VAThreadCondTimedWait(VAThread_cond_t *cond, lock_i *mutex, unsigned long millisecond)
{
	VAThread_mutex_t* pTmpMutex = mutex->get_mutex();
	return VAThreadCondTimedWait(cond, pTmpMutex, millisecond);
}
int VAThreadCondTimedWaitLock(VAThread_cond_t *cond, lock_i *mutex, unsigned long millisecond)
{
	VAThread_mutex_t* pTmpMutex = mutex->get_mutex();
	return VAThreadCondTimedWaitLock(cond, pTmpMutex, millisecond);
}

int VAThreadSemInit(VAThread_sem_t *sem, unsigned int uMaxCnt)
{
#if VA_VC
	*sem = CreateSemaphore(NULL, 0, uMaxCnt, NULL);
	return (*sem == INVALID_HANDLE_VALUE) ? GetLastError() : 0;
#elif VA_GNUC
	return sem_init(sem, 0, uMaxCnt);
#endif
}

int VAThreadSemDestroy(VAThread_sem_t *sem)
{
#if VA_VC
	int ret = CloseHandle(*sem);
	return (ret == FALSE) ? GetLastError() : 0;
#elif VA_GNUC
	return sem_destroy(sem);
#endif
}

/* 阻塞等待信号量 */
int VAThreadSemWait(VAThread_sem_t *sem)
{
#if VA_VC
	int ret = WaitForSingleObject(*sem, INFINITE);
	ret = (ret != WAIT_OBJECT_0) ? GetLastError() : 0;
	return ret;
#elif VA_GNUC
	return sem_wait(sem);
#endif
}

/* 释放信号量 */
int VAThreadSemPost(VAThread_sem_t *sem)
{
#if VA_VC
	int ret = ReleaseSemaphore(*sem, 1, NULL);
	return (ret != TRUE) ? GetLastError() : 0;
#elif VA_GNUC
	return sem_post(sem);
#endif
}

} /* namespace VA */
