#ifndef TEST_MVW_APIRANDOM_H_
#define TEST_MVW_APIRANDOM_H_

#include "DefMVW.h"
#include "VAThread.h"
#include <string>

using namespace VA;

class TestMVWApiRandom {
public:
	ISSErrID err;
	HISSMVW	 phISSMVW;

	DefMVW def;

	std::string getParam(int);
	std::string getParamValue(int);
	std::string getMvwKeyWord(int);

	TestMVWApiRandom();
	void test(); // 测试入口
	void threadRollAppendAudioData(); // 循环送音频线程
};

static VA::VAThread_result_t static_threadRollAppendAudioData(void *args);

#endif  // !TEST_MVW_APIRANDOM_H_
