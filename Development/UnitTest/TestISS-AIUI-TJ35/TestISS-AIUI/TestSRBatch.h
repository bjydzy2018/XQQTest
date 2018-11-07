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

	//std::string use_path_pre_;
	//std::vector<double> getVadLabel(std::string);

public:
	TestSRBatch();
	~TestSRBatch();
	void test(int, int, std::string, std::string, std::string dict_path = "", int UploadDictMode = 0);

	void setLoopNum(int loop_num) { loop_num_ = loop_num; };
	//void setUsePathPre(std::string use_path_pre ) { use_path_pre_ = use_path_pre; };
};


class TestSRBatch3 {
private:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;

	int loop_num_;

	std::string use_path_pre_;
	std::string machineCode_ ;
	std::vector<double> getVadLabel(std::string);

public:
	TestSRBatch3();
	~TestSRBatch3();
	void test(int, int, std::string, std::string, std::string dict_path = "", int UploadDictMode = 0);

	void setLoopNum(int loop_num) { loop_num_ = loop_num; };
	void setUsePathPre(std::string use_path_pre ) { use_path_pre_ = use_path_pre; };
	void setMachineCode(std::string machineCode) { machineCode_ = machineCode; };
};

class TestSRBatchAIUITime {
private:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;

	int loop_num_;

	std::string use_path_pre_;

	std::string machineCode_ ;

public:
	TestSRBatchAIUITime();
	~TestSRBatchAIUITime();
	void test(int, int, std::string, std::string, std::string dict_path = "", int UploadDictMode = 0);

	void setLoopNum(int loop_num) { loop_num_ = loop_num; };
	void setUsePathPre(std::string use_path_pre ) { use_path_pre_ = use_path_pre; };
	void setMachineCode(std::string machineCode) { machineCode_ = machineCode; };
};

class TestSRBatchAIUIEffect {
private:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;

	int loop_num_;
	std::string user_data_path_;
	std::string use_path_pre_;
	std::string machineCode_;

public:
	TestSRBatchAIUIEffect();
	~TestSRBatchAIUIEffect();
	void test(int, int, std::string, std::string, std::string dict_path = "", int UploadDictMode = 0);
	int GetAudioNumber(std::string audio_path);
	void setLoopNum(int loop_num) { loop_num_ = loop_num; };
	void setUserDataPath(std::string user_data_path) { user_data_path_ = user_data_path; };
	void setUsePathPre(std::string use_path_pre ) { use_path_pre_ = use_path_pre; };
	void setMachineCode(std::string machineCode) { machineCode_ = machineCode; };
};


class TestSRBatchVad {
private:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;

	int loop_num_;
	std::string use_path_pre_;
	std::string machineCode_;

public:
	TestSRBatchVad();
	~TestSRBatchVad();
	void test(int, int, std::string, int);
	void setLoopNum(int loop_num) { loop_num_ = loop_num; };
	void setUsePathPre(std::string use_path_pre ) { use_path_pre_ = use_path_pre; };
	void setMachineCode(std::string machineCode) { machineCode_ = machineCode; };
};


#endif



