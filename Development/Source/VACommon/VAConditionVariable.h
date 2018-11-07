/*
 * VAConditionVariable.h
 *
 *  Created on: 2014年11月4日
 *      Author: pffang
 */

#ifndef VACONDITIONVARIABLE_H_
#define VACONDITIONVARIABLE_H_
#include "VACommon.h"

#if VA_GNUC

namespace VA {

class Condition_Variable {
	pthread_cond_t mCond;
	pthread_condattr_t mAttr;
public:
	Condition_Variable();
	~Condition_Variable();
	void Wait(pthread_mutex_t * mutex);
	void WaitFor(pthread_mutex_t * mutex, unsigned long int millisecond);
	void Notify();
};

} /* namespace VA */
#endif

#endif /* VACONDITIONVARIABLE_H_ */
