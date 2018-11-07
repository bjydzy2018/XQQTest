#ifndef TEST_NLI_BATCH_H_
#define TEST_NLI_BATCH_H_

#include "DefSR.h"

#include <string>
#include <vector>

class TestNLIBatch {
private:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;

	int loop_num_;
	int sleep_time_;
	//std::string user_data_path_;
	std::string machineCode_ ;

public:
	TestNLIBatch();
	~TestNLIBatch();
	void test(int lang, int mode, string text_list_path, std::string dict_path = "",int UploadDictMode = 0);

	void setLoopNum(int loop_num) { loop_num_ = loop_num; };
	void setSleepTime(int sleep_time) { sleep_time_ = sleep_time; };
	//void setUserDataPath(std::string user_data_path) { user_data_path_ = user_data_path; };
	void setMachineCode(std::string machineCode) { machineCode_ = machineCode; };
};

#endif // !TEST_NLI_BATCH_H_

