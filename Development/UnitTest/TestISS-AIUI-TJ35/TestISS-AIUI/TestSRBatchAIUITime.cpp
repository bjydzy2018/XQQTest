#include "TestSRBatch.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"
#include "TimerForTest.h"
#include "rapidjson/document.h"

#include <iostream>
#include <fstream>

using std::string;
using std::vector;
using namespace rapidjson;
using namespace VA;

TestSRBatchAIUITime::TestSRBatchAIUITime() :err(-1), phISSSR(NULL) {
	
}

TestSRBatchAIUITime::~TestSRBatchAIUITime() {
	err = ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	DEBUG_TRACE("ISSSRDestroy return %d",err);

	def.initMsg();
}

void TestSRBatchAIUITime::test(int lang, int mode, string audio_list_path, string test_type,
	string dict_path, int UploadDictMode) {


	if (machineCode_ != "") {
		DEBUG_TRACE("MachineCode:%s", machineCode_.c_str());
		err = ISSSetMachineCode(machineCode_.c_str());
		DEBUG_TRACE("ISSSetMachineCode return %d",err);
	}
	else {
		DEBUG_TRACE("MachineCode:%s", def.machineCode.c_str());
		err = ISSSetMachineCode(def.machineCode.c_str());
		DEBUG_TRACE("ISSSetMachineCode return %d",err);
	}


	//测试响应时间的时候，不会上传词典，不管配置文件里面有没有配置
	string lang_str("");
	string mode_str("");

	if (lang == 0)
		lang_str = "chinese";
	else if (lang == 1)
		lang_str = "english";
	else if (lang == 2)
		lang_str = "cantonese";

	if (mode == 0)
		mode_str = "cloud";
	else if (mode == 1)
		mode_str = "local";
	else if (mode == 2)
		mode_str = "mix";

	DEBUG_TRACE("before ISSSRCreate");

	TimerForTest Create;

	// ISSSRCreate
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
#endif
	DEBUG_TRACE("ISSSRCreate return %d",err);

	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	if (!def.msgInitStatus)
		//return;

	DEBUG_TRACE("SR Create Time: %f",Create.elapsed());

	DEBUG_TRACE("after creating, sleep 10s");
	sleep(10000);

	if (dict_path != "") {
		// ISSSRUpLoadDict
		err = ISSSRUpLoadDict(phISSSR, readFile(testRes_path + dict_path, false).c_str(), UploadDictMode);
		DEBUG_TRACE("ISSSRUpLoadDict return %d",err);

		DEBUG_TRACE("waiting for uploading dict");
		int count = 0;
		if (UploadDictMode==0)
			while ((!def.msgUpLoadDictToLocalStatus || !def.msgUpLoadDictToCloudStatus)&&count++<3000)
				sleep(10);
		if (UploadDictMode==1)
			while (!def.msgUpLoadDictToCloudStatus &&count++<3000)
				sleep(10);
		if (UploadDictMode==2)
			while (!def.msgUpLoadDictToLocalStatus &&count++<3000)
				sleep(10);

		DEBUG_TRACE("after uploading dict, sleep 10s");
		sleep(10000);
	}


	// 识别
	vector<string> audio_paths = readFileByLine(testRes_path + audio_list_path, true);

	DEBUG_TRACE("sr batch loop num: %d", loop_num_);
	if (loop_num_ == -1)
		loop_num_ = 10000000;

	//只start一次
	DEBUG_TRACE("ISSSRStart mode: %d", mode);
	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, mode, "");
	DEBUG_TRACE("ISSSRStart return %d", err);

	int test_num = 1; // 测试序号
	TimerForTest AppendAudioTime;
	double TotalTime = 0;
	double EachTime = 0;
	for (int i = 0; i < loop_num_; i++) {
		int audioNum = 0;
		for(size_t n = 0; n < audio_paths.size(); ++n){
			string audio_path = audio_paths[n];
			if(use_path_pre_ != ""){
				DEBUG_TRACE_INFOR("use_path_pre_:%s",use_path_pre_.c_str());
				audio_path = use_path_pre_ + audio_path;
			}
			DEBUG_TRACE_INFOR("current test num: %d, audio_path: %s", test_num++, audio_path.c_str());

			err = ISSSRSetParam(phISSSR, "audioTag", VA::IntToString(++audioNum).c_str());
			DEBUG_TRACE("ISSSRSetParam audioTag audioNum %d ret %d", audioNum, err);

			AppendAudioTime.restart();
			//appendAudio(phISSSR, "sr", audio_path, def);
			if (test_type == "time") {
				appendAudioToAIUISR(phISSSR, audio_path, def, 3000);
			}
			else {
				appendAudioToAIUISR(phISSSR, audio_path, def, 0);
			}
			EachTime = AppendAudioTime.elapsed();
			TotalTime += EachTime;

			if (test_type == "time") {
				int i = 0;
				while (!def.msgResult && i++ < 1000)
					sleep(10);
			}
			
			def.initMsg();
			//sleep(5);
		}
	}
	DEBUG_TRACE("AppendAudio resume total time:%f",TotalTime);
	
}

