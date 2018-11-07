#ifndef SEOPT_SR_BATCH_H_
#define SEOPT_SR_BATCH_H_

#include "DefSeopt.h"
#include "DefSR.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class TestSeoptSRBatch {
private:
	ISSErrID err;
	ISSSeoptHandle phISSSeopt;
	HISSSR	 phISSSR;

	DefSeopt seoptDef;
	DefSR srDef;


	int loop_num_;

	std::string use_path_pre_;
	std::string machineCode_ ;

	std::string GetAudioOutName(std::string audio_in_path,std::string strLabel);

public:
	TestSeoptSRBatch();
	~TestSeoptSRBatch();
	// SR 参数： int lang, int mode, string audio_list_path, string test_type, string dict_path, int UploadDictMode
	// Seopt 参数：std::string audio_list_path, std::string audio_out_path, bool IfNeedSleep
	//void test(int lang, int mode, string audio_list_path, string test_type,
	//	string dict_path, int ifOnlyUploadToCloud);
	void test(string audio_list_path ,int lang, int nSrMode, int nSeoptMode);
	void test(int, int, std::string, std::string, std::string dict_path = "", int UploadDictMode = 0);
	void setLoopNum(int loop_num) { loop_num_ = loop_num; };
	void setUsePathPre(std::string use_path_pre ) { use_path_pre_ = use_path_pre; };
	void setMachineCode(std::string machineCode) { machineCode_ = machineCode; };
};

#endif