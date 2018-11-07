#ifndef TEST_SR_APIRANDOM_H_
#define TEST_SR_APIRANDOM_H_

#include "DefSR.h"
#include "VAThread.h"
#include <string>

using namespace VA;

class TestSRApiRandom {
public:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;

	std::string getStartScene(int);
	int getStartMode(int);
	std::string getSzCmd(int);
	std::string getSzParam(int);
	std::string getSzParamValue(int);

	void SRJoggle();

	TestSRApiRandom();
	void test(); // 测试入口
	void threadRollAppendAudioData(); // 循环送音频线程
	void threadRollUploadDict(); // 循环上传词典线程
	void threadRollStart(); // 循环start
};

static VAThread_result_t static_threadRollAppendAudioData(void *args);
static VAThread_result_t static_threadRollUploadDict(void *args);
static VAThread_result_t static_threadRollStart(void *args);

#endif // !TEST_SR_APIRANDOM_H_
