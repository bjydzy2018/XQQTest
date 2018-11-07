/*
 * CMutex.cpp
 *
 *  Created on: 2014-8-13
 *      Author: yjren
 */
#include"CMutex.h"

CMutex::CMutex(){
}
int CMutex::init(){
#ifdef VA_GNUC
	return pthread_mutex_init(&mutex,NULL);
#elif VA_VC
	handle = CreateMutex(NULL,FALSE,NULL);
#endif
	return 0;
}

int CMutex::destroy(){
#ifdef VA_GNUC
	return pthread_mutex_destroy(&mutex);
#elif VA_VC
	CloseHandle(handle);
#endif
	return 0;
}

int CMutex::lock(){
#ifdef VA_GNUC
	return pthread_mutex_lock(&mutex);
#elif VA_VC
	WaitForSingleObject(handle,INFINITE);
#endif
	return 0;
}

int CMutex::trylock(){
#ifdef VA_GNUC
	return pthread_mutex_trylock(&mutex);
#elif VA_VC
	WaitForSingleObject(handle,0);
#endif
	return 0;
}

int CMutex::unlock(){
#ifdef VA_GNUC
	return pthread_mutex_unlock(&mutex);
#elif VA_VC
	ReleaseMutex(handle);
#endif
	return 0;
}





