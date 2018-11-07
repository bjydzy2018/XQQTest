#include "TimerForTest.h"

using namespace VA;
using std::string;

//兼容wince平台和Linux平台GetTickCount函数
long CurrentTime(){
	long time_now = 0;
#if VA_VC
	time_now = GetTickCount();
#elif VA_GNUC
	struct timeval now;
	gettimeofday(&now, NULL);
	time_now = now.tv_sec * 1000 + now.tv_usec / 1000;
#endif
	return time_now;
}
