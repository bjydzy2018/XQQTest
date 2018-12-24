#ifndef TEST_UPLOADDICT_BATCH_H_
#define TEST_UPLOADDICT_BATCH_H_

#include "DefSR.h"

#include <string>
#include <vector>

class TestUpLoadDictBatch {
private:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;

	int loop_num_;
	std::string machineCode_;

public:
	TestUpLoadDictBatch();
	~TestUpLoadDictBatch();
	void test(int lang, string dict_path, string dict_name, int ifOnlyUploadToCloud, int ifEveryTimeNeedCreate);
	void setLoopNum(int loop_num) { loop_num_ = loop_num; };
	void setMachineCode(std::string machineCode) { machineCode_ = machineCode; };
};

#endif 

