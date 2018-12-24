#ifndef TEST_SR_BATCH_H_
#define TEST_SR_BATCH_H_

#include "DefSR.h"

#include <string>
#include <vector>

class TestSRBatch {
private:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;

	int loop_num_;

	std::string machineCode_ ;

//#ifdef ISS_SR_3
	std::vector<double> getVadLabel(std::string);
//#endif

public:
	TestSRBatch();
	~TestSRBatch();
	void test(int, int, std::string, std::string, std::string dict_path = "", int ifOnlyUploadToCloud = 0);

	void setLoopNum(int loop_num) { loop_num_ = loop_num; };
	void setMachineCode(std::string machineCode) { machineCode_ = machineCode; };
};

#endif // !TEST_SR_BATCH_H_

