#ifndef _TIMERFORTEST_H_
#define _TIMERFORTEST_H_

#include "VAInc.h"

long CurrentTime();

class TimerForTest {
public :
	//µ¥Î»:ms
	TimerForTest(){ startTime = CurrentTime(); }

	void   restart() { startTime = CurrentTime(); } 
	double elapsed() const
	{ return  double(CurrentTime() - startTime); }
private:
	long startTime;

};

#endif