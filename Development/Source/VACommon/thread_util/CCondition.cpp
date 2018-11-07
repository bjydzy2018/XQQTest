/*
 * CCondition.cpp
 *
 *  Created on: 2014-8-13
 *      Author: yjren
 */
#include"CCondition.h"
#include"../../ISS_API/iss_errors.h"
using namespace VA;

#if VA_DEBUG
    static VA::CTrace CLinkedTrace("CCondition", VA_DEBUG_LEVEL_DEBUG);
#endif
#define DEBUG_TRACE(...) (CLinkedTrace.TraceLevel(VA_DEBUG_LEVEL_INFOR, __VA_ARGS__))


CCondition::CCondition(){
}

int CCondition::init(){
#ifdef VA_GNUC
	mutex = PTHREAD_COND_INITIALIZER;
	pthread_condattr_init(&mAttr);
	pthread_condattr_setclock(&mAttr, CLOCK_MONOTONIC);
	return pthread_cond_init(&cond,&mAttr);
#elif VA_VC
	handle = CreateEvent(NULL,FALSE,FALSE,NULL);
#endif
	return 0;
}

int CCondition::destroy(){
#ifdef VA_GNUC
	pthread_cond_destroy(&cond);
	pthread_condattr_destroy(&mAttr);
	pthread_mutex_destroy(&mutex);
	return ISS_SUCCESS;
#elif VA_VC
	CloseHandle(handle);
#endif
	return 0;
}

int CCondition::wait(unsigned long int millisecond){
	int err = 0;

	if(millisecond){
#ifdef VA_GNUC
		struct timespec timeToWait;
		struct timespec now;
		unsigned long long ns;
		unsigned long long s;
		pthread_mutex_lock(mutex);
		clock_gettime(CLOCK_MONOTONIC, &now);
		ns = (unsigned long long)now.tv_nsec + 1000000ULL * (unsigned long long)millisecond;
		s = ns / 1000000000ULL;
		ns = ns - s * 1000000000ULL;
		timeToWait.tv_sec = now.tv_sec + s;
		timeToWait.tv_nsec = ns;
		err = pthread_cond_timedwait(&cond, mutex, &timeToWait);
		pthread_mutex_lock(mutex);
#elif VA_VC
		WaitForSingleObject(handle,millisecond);
#endif
	} else {
#ifdef VA_GNUC
		pthread_mutex_lock(mutex);
		err = pthread_cond_wait(&cond,&mutex);
		pthread_mutex_unlock(mutex);
#elif VA_VC
		WaitForSingleObject(handle,INFINITE);
#endif
	}

	return err;
}

int CCondition::signal(){
#ifdef VA_GNUC
	return pthread_cond_signal(&cond);
#elif VA_VC
	SetEvent(handle);
#endif
	return 0;
}

