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

TestSRBatchVad::TestSRBatchVad() :err(-1), phISSSR(NULL) {
	
}

TestSRBatchVad::~TestSRBatchVad() {
	err = ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	DEBUG_TRACE("ISSSRDestroy return %d",err);

	def.initMsg();
}

void TestSRBatchVad::test(int lang, int mode, string audio_list_path,int IsAppendAudioSleep) {

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
	TimerForTest InitTime;
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

	DEBUG_TRACE("Init time:%f ", InitTime.elapsed() );

	DEBUG_TRACE("after creating, sleep 10s");
	sleep(10000);

	// 识别
	vector<string> audio_paths = readFileByLine(testRes_path + audio_list_path, true);

	DEBUG_TRACE("sr batch loop num: %d", loop_num_);
	if (loop_num_ == -1)
		loop_num_ = 10000000;

	////只start一次
	//DEBUG_TRACE("ISSSRStart mode: %d", mode);
	//err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, mode, "");
	//DEBUG_TRACE("ISSSRStart return %d", err);

	int test_num = 1; // 测试序号
	for (int i = 0; i < loop_num_; i++) {
		int audioNum = 0;
		for(size_t n = 0; n < audio_paths.size(); ++n){
			string audio_path = audio_paths[n];
			if(use_path_pre_ != ""){
				DEBUG_TRACE_INFOR("use_path_pre_:%s",use_path_pre_.c_str());
				audio_path = use_path_pre_ + audio_path;
			}
			DEBUG_TRACE_INFOR("current test num: %d, audio_path: %s", test_num++, audio_path.c_str());

			
			DEBUG_TRACE("ISSSRSetParam audioTag audioNum %d ret %d", audioNum, err);
			err = ISSSRSetParam(phISSSR, "audioTag", VA::IntToString(++audioNum).c_str());

			DEBUG_TRACE("ISSSRStart mode: %d", mode);
			err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, mode, "");
			DEBUG_TRACE("ISSSRStart return %d", err);

			

			if(IsAppendAudioSleep==0){
				appendAudioToAIUISRNoSleep(phISSSR, audio_path, def, 3000);
			}
			if(IsAppendAudioSleep==1){
				appendAudioToAIUISR(phISSSR, audio_path, def, 3000);
			}
		
			

			//err = ISSSRFlushAudioData(phISSSR ,1000);

			int i = 0;
			while (!def.msgSpeechEnd && i++ < 100)
				sleep(10);
			
			def.initMsg();
		}
	}
	
}

