#include "MutexSignalCommon.h"

namespace VA {

#if VA_GNUC
void EnterMutex(pthread_mutex_t* pMutex) {
    pthread_mutex_lock(pMutex);
}
void LeaveMutex(pthread_mutex_t* pMutex) {
    pthread_mutex_unlock(pMutex);
}
void SetSignal(pthread_cond_t* pCond) {
    pthread_cond_signal(pCond);
}

/* Set up the condvar attributes to use CLOCK_MONOTONIC. 

    pthread_condattr_init( &attr);
    pthread_condattr_setclock( &attr, CLOCK_MONOTONIC);
    pthread_cond_init( &c, &attr);
    
    void PthreadSleep(pthread_cond_t* cond, pthread_mutex_t* mutex, unsigned long int millisecond) {
        struct timespec timeToWait;
        struct timespec now;
        
        unsigned long int s, ns;
        pthread_mutex_lock(mutex);
        clock_gettime(CLOCK_MONOTONIC, &now);
        ns = now.tv_nsec + 1000000UL * millisecond;
        s = ns / 1000000000UL;
        ns = ns - s * 1000000000UL;
        timeToWait.tv_sec = now.tv_sec + s;
        timeToWait.tv_nsec = ns * 1000UL;
        pthread_cond_timedwait(cond, mutex, &timeToWait);
        pthread_mutex_unlock(mutex);
    }

*/
void PthreadSleep(pthread_cond_t* cond, pthread_mutex_t* mutex,
                  unsigned long int millisecond) 
{
	if (millisecond != INFINITE) {
		struct timespec timeToWait;
		struct timeval now;
		unsigned long int s, us;
		pthread_mutex_lock(mutex);
		gettimeofday(&now, NULL);
		us = now.tv_usec + 1000UL * millisecond;
		s = us / 1000000UL;
		us = us - s * 1000000UL;
		timeToWait.tv_sec = now.tv_sec + s;
		timeToWait.tv_nsec = us * 1000UL;
		pthread_cond_timedwait(cond, mutex, &timeToWait);
	} else {
		pthread_mutex_lock(mutex);
		pthread_cond_wait(cond, mutex);
	}

	pthread_mutex_unlock(mutex);
}
#elif VA_VC
void EnterMutex(LPCRITICAL_SECTION lpCriticalSection) {
    EnterCriticalSection(lpCriticalSection);
}
void LeaveMutex(LPCRITICAL_SECTION lpCriticalSection) {
    LeaveCriticalSection(lpCriticalSection);
}
void SetSignal(HANDLE* pEventHandle) {
    SetEvent(*pEventHandle);
}
#endif

}