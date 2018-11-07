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

#ifndef ISS_SR_3

TestSRBatch::TestSRBatch() :err(-1), phISSSR(NULL) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	DEBUG_TRACE("ISSSetMachineCode return %d",err);
}

TestSRBatch::~TestSRBatch() {
	err = ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	DEBUG_TRACE("ISSSRDestroy return %d",err);

	def.initMsg();
}

void TestSRBatch::test(int lang, int mode, string audio_list_path, string test_type,
	string dict_path, int ifOnlyUploadToCloud) {

	if (test_type == "time") {
		DEBUG_TRACE_ERROR("AIUI SR time test doesn't support");
		return;
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

	// ISSSRCreate
	err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
	DEBUG_TRACE("ISSSRCreate return %d",err);
	if (err != ISS_SUCCESS)
		return;

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	if (!def.msgInitStatus)
		return;

	DEBUG_TRACE("after creating, sleep 10s");
	sleep(10000);

	if (dict_path != "") {
		// ISSSRUpLoadDict
		err = ISSSRUpLoadDict(phISSSR, readFile(testRes_path + dict_path, false).c_str(), ifOnlyUploadToCloud);
		DEBUG_TRACE("ISSSRUpLoadDict return %d",err);

		DEBUG_TRACE("waiting for uploading dict");
		if (!ifOnlyUploadToCloud)
			while (!def.msgUpLoadDictToLocalStatus || !def.msgUpLoadDictToCloudStatus)
				sleep(10);
		else
			while (!def.msgUpLoadDictToLocalStatus && !def.msgUpLoadDictToCloudStatus)
				sleep(10);

		DEBUG_TRACE("after uploading dict, sleep 10s");
		sleep(10000);
	}

	//  ∂±
	vector<string> audio_paths = readFileByLine(testRes_path + audio_list_path, true);

	DEBUG_TRACE("sr batch loop num: %d", loop_num_);
	if (loop_num_ == -1)
		loop_num_ = 10000000;

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, mode, "");
	DEBUG_TRACE("ISSSRStart return %d", err);

	int test_num = 1; // ≤‚ ‘–Ú∫≈
	for (int i = 0; i < loop_num_; i++) {
		for(size_t n = 0; n < audio_paths.size(); ++n){
			string audio_path = audio_paths[n];
			DEBUG_TRACE_INFOR("current test num: %d, audio_path: %s", test_num++, audio_path.c_str());

			//timer start_appendAudioData_timer;
			appendAudioToAIUISR(phISSSR, audio_path, def, 500);

			def.initMsg();
		}
	}
}

#endif
