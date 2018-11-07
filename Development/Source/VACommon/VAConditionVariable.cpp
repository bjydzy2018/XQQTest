/*
 * VAConditionVariable.cpp
 *
 *  Created on: 2014年11月4日
 *      Author: pffang
 */

#include "VAConditionVariable.h"
#if VA_GNUC

namespace VA {

Condition_Variable::Condition_Variable() {
	pthread_condattr_init(&mAttr);
	pthread_condattr_setclock(&mAttr, CLOCK_MONOTONIC);
	pthread_cond_init(&mCond, &mAttr);

}

Condition_Variable::~Condition_Variable() {
	pthread_cond_destroy(&mCond);
	pthread_condattr_destroy(&mAttr);
}

void Condition_Variable::Wait(pthread_mutex_t * mutex) {
	pthread_mutex_lock(mutex);
	pthread_cond_wait(&mCond, mutex);
	pthread_mutex_unlock(mutex);
}

void Condition_Variable::WaitFor(pthread_mutex_t * mutex,
		unsigned long int millisecond) {
	struct timespec timeToWait;
	struct timespec now;

	unsigned long long ns;
	unsigned long long s;
	pthread_mutex_lock(mutex);
	clock_gettime(CLOCK_MONOTONIC, &now);
//	fprintf(stderr, "now:S:%lu, ns:%lu\n", now.tv_sec, now.tv_nsec);
	ns = (unsigned long long)now.tv_nsec + 1000000ULL * (unsigned long long)millisecond;
//	fprintf(stderr, "TTT ns1:%llu\n", ns);
	s = ns / 1000000000ULL;
//	fprintf(stderr, "TTT s:%llu\n", s);
	ns = ns - s * 1000000000ULL;
//	fprintf(stderr, "TTT ns2:%llu\n", ns);
	timeToWait.tv_sec = now.tv_sec + s;
	timeToWait.tv_nsec = ns;
	pthread_cond_timedwait(&mCond, mutex, &timeToWait);
	pthread_mutex_unlock(mutex);
}

void Condition_Variable::Notify() {
	pthread_cond_signal(&mCond);
}

} /* namespace VA */
#endif
