/*
 * CSystem.h
 *
 *  Created on: 2014-8-19
 *      Author: yjren
 */

#ifndef CSYSTEM_H_
#define CSYSTEM_H_

#include"..\VAInc.h"

#ifdef VA_GNUC
#include"sys/time.h"
#elif VA_VC

#endif

namespace VA{

class CSystem {
public:
	static long getCurrentTime();
};
}



#endif /* CSYSTEM_H_ */
