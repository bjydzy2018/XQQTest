/*
 * CSystem.cpp
 *
 *  Created on: 2014-8-19
 *      Author: yjren
 */

#include"CSystem.h"
using namespace VA;

long CSystem::getCurrentTime()
{
#ifdef VA_GNUC
	long msec = 0;
	timeval time;
	gettimeofday(&time,NULL);
	msec = time.tv_sec*1000+time.tv_usec/1000;
	return msec;
#elif VA_VC
	DWORD time = GetTickCount();
	return time;
#endif
}


