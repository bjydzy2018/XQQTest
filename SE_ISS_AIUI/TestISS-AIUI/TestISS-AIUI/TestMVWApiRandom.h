#ifndef TEST_MVW_APIRANDOM_H_
#define TEST_MVW_APIRANDOM_H_

#include "DefMVW.h"

#include <string>

class TestMVWApiRandom {
public:
	ISSErrID err;
	HISSMVW	 phISSMVW;

	DefMVW def;
	double run_time_;

	std::string getParam(int);
	std::string getParamValue(int);
	std::string getMvwKeyWord(int);

	void setRunTime(double run_time) { run_time_ = run_time; };
	TestMVWApiRandom();
	void test(); // �������
	void threadRollAppendAudioData(); // ѭ������Ƶ�߳�
};

#endif  // !TEST_MVW_APIRANDOM_H_
