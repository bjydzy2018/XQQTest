/*
 * CThread.h
 *
 *  Created on: 2014-8-17
 *      Author: yjren
 */

#ifndef CTHREAD_H_
#define CTHREAD_H_

#include"..\VAInc.h"

#ifdef VA_GNUC
#include"unistd.h"
#include"pthread.h"
#endif

const int INVALID_PARAM_ERR_CThread = 101;

namespace VA{

class CThread{

#ifdef VA_GNUC
	pthread_t handle;
#elif VA_VC
	HANDLE handle;
#endif

public:
	CThread();
	~CThread();

#ifdef VA_GNUC
	int start(void* (*f)(void*),void *);
#elif VA_VC
	int start(DWORD (*f)(LPVOID ),LPVOID arg);
#endif
	int detach();
	static int sleep(unsigned long msec);
};

}
#endif /* CTHREAD_H_ */
