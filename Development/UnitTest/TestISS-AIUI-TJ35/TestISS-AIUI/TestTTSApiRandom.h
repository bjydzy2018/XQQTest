#ifndef TEST_TTS_APIRANDOM_H_
#define TEST_TTS_APIRANDOM_H_

#include "DefTTS.h"

#include <string>

class TestTTSApiRandom {
public:
	ISSErrID err;
	HISSTTSRES phISSTTSRES;
	HISSTTS	 phISSTTS;

	DefTTS def;

	int getParam(int);

	TestTTSApiRandom();
	void test(); // ≤‚ ‘»Îø⁄
};

#endif
