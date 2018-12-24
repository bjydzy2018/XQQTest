#ifndef TEST_SR_CONSISTENT_H_
#define TEST_SR_CONSISTENT_H_

#include "DefSR.h"

#include <string>
#include <vector>

class TestSRConsistent {
private:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;

	int loop_num_;
	std::string user_data_path_;
	std::string machineCode_;

#ifdef ISS_SR_3
	std::vector<double> getVadLabel(std::string);
#endif

public:
	TestSRConsistent();
	~TestSRConsistent();
	void test(int, int, std::string, std::string, std::string dict_path = "", int ifOnlyUploadToCloud = 0);
	int GetAudioNumber(std::string audio_path);
	void setLoopNum(int loop_num) { loop_num_ = loop_num; };
	void setUserDataPath(std::string user_data_path) { user_data_path_ = user_data_path; };
	void setMachineCode(std::string machineCode) { machineCode_ = machineCode; };
};

#endif // !TEST_SR_CONSISTENT_H_
