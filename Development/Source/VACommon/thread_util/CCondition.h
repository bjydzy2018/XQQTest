/*
 * CCondition.h
 *
 *  Created on: 2014-8-13
 *      Author: yjren
 */

#ifndef CCONDITION_H_
#define CCONDITION_H_
#include"../VAInc.h"

class CCondition{
#ifdef VA_GNUC
	/*
	 *This mutex is useless ONLY for condwait which need non_null mutex.
	 *In reality,we use solitary mutex instead of conbining those together
	 *for high flexible with losing some performance .
	 */
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	pthread_condattr_t mAttr;
#elif VA_VC
	HANDLE handle;
#endif
public:
	CCondition();

	int init();
	int destroy();
	int wait(unsigned long int millisecond = 0);
	int signal();

};



#endif /* CCONDITION_H_ */
