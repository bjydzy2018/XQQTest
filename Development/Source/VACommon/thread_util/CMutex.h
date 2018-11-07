/*
 * CMutex.h
 *
 *  Created on: 2014-8-13
 *      Author: yjren
 */
#ifndef CMUTEX_H_
#define CMUTEX_H_

#include"../VAInc.h"
class CMutex{

#ifdef VA_GNUC
	pthread_mutex_t mutex;
#elif VA_VC
	HANDLE handle;
#endif

public:
	CMutex();

	int init();
	int destroy();
	int lock();
	int trylock();
	int unlock();
};



#endif /* CMUTEX_H_ */
