#ifndef TEST_SR_APIRANDOM_H_
#define TEST_SR_APIRANDOM_H_

#include "DefSR.h"

#include <string>

class TestSRApiRandom {
public:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;
	double run_time_;

	std::string getStartScene(int);
	int getStartMode(int);
	std::string getSzCmd(int);
	std::string getSzParam(int);
	std::string getSzParamValue(int);
	void setRunTime(double run_time) { run_time_ = run_time; };
	TestSRApiRandom();
	void test(); // 测试入口
	void threadRollAppendAudioData(); // 循环送音频线程
	void threadRollUploadDict(); // 循环上传词典线程
};

#endif // !TEST_SR_APIRANDOM_H_
