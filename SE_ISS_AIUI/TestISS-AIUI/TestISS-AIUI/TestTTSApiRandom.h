#ifdef __ANDROID__
#ifndef TEST_TTS_APIRANDOM_H_
#define TEST_TTS_APIRANDOM_H_

#include "DefTTS.h"

#include <string>

class TestTTSApiRandom {
public:
	ISSErrID err;
	HISSTTSRES phISSTTSRES;
	HISSTTS	 phISSTTS;
	double run_time_;
	DefTTS def;

	int getParam(int);
	void setRunTime(double run_time) { run_time_ = run_time; };
	TestTTSApiRandom();
	void test(); // ≤‚ ‘»Îø⁄
};

#endif
#endif