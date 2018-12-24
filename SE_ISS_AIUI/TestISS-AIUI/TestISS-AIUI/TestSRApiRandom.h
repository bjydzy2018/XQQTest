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
	void test(); // �������
	void threadRollAppendAudioData(); // ѭ������Ƶ�߳�
	void threadRollUploadDict(); // ѭ���ϴ��ʵ��߳�
};

#endif // !TEST_SR_APIRANDOM_H_
