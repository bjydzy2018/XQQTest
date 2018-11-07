/*
 * CThread.cpp
 *
 *  Created on: 2014-8-17
 *      Author: yjren
 */

#include"CThread.h"

namespace VA{

CThread::CThread()
{
#ifdef VA_GNUC
	handle = 0;
#endif
}

CThread::~CThread()
{
#ifdef VA_GNUC
	detach();
#endif
}
#ifdef VA_GNUC
int CThread::start(void* (*p_func)(void *),void *p_arg)
{
	int err = 0;
	if(p_func == NULL){
		return INVALID_PARAM_ERR_CThread;
	}

	if(handle != 0){
		detach();
	}

	err = pthread_create(&handle,NULL,p_func,p_arg);
	return err;
}
#elif VA_VC
int CThread::start(DWORD (*p_func)(LPVOID),LPVOID arg)
{
	int err = 0;
	if(p_func == NULL){
		return INVALID_PARAM_ERR_CThread;
	}

	if(handle != 0){
		detach();
	}
	handle = CreateThread(NULL,0,p_func,arg,NULL,NULL);
	return 0;
}
#endif

int CThread::detach()
{
	int err = 0;
#ifdef VA_GNUC
	err = pthread_detach(handle);
	handle = 0;
#endif
	return err;
}
int CThread::sleep(unsigned long msec)
{
	int err = 0;
#ifdef VA_GNUC
	unsigned long sec = msec/1000;
	unsigned long nano = (msec - sec*1000)*1000*1000;
	timespec time;
	time.tv_sec = sec;
	time.tv_nsec = nano;
	err = nanosleep(&time,NULL);
#elif VA_VC
	Sleep(msec);
#endif
	return err;
}


}