#include "TestSRBatch.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

#include "rapidjson/document.h"

#include <iostream>
#include <fstream>

using std::string;
using std::vector;
using std::pair;
using namespace rapidjson;
using namespace VA;

typedef struct tagParams {
	string audio_path;
	string userdata;
} TParams;


TestSRBatchAIUIEffect::TestSRBatchAIUIEffect() :err(-1), phISSSR(NULL) {
	
}

TestSRBatchAIUIEffect::~TestSRBatchAIUIEffect() {
	err = ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	DEBUG_TRACE(("ISSSRDestroy return " + LongToString(err)).c_str());

	def.initMsg();
}

int TestSRBatchAIUIEffect::GetAudioNumber(std::string audio_path) {
	std::size_t found_1 = audio_path.find_last_of("/\\");
	//printf("found_1:%d\n",found_1);
	std::size_t found_2 = audio_path.find_last_of("-");
	//printf("found_2:%d\n",found_2);

	std::string SubStr = audio_path.substr(found_1+1, 4);
	//printf("SubStr:%s\n",SubStr.c_str());
	
	int RetNum = atoi(SubStr.c_str());
	//printf("%d\n",RetNum);
	return RetNum;
	
}
void TestSRBatchAIUIEffect::test(int lang, int mode, string audio_list_path, string test_type,
	string dict_path, int UploadDictMode) {
	if (machineCode_ != "") {
		DEBUG_TRACE("MachineCode:%s", machineCode_.c_str());
		err = ISSSetMachineCode(machineCode_.c_str());
		DEBUG_TRACE(("ISSSetMachineCode return " + LongToString(err)).c_str());
	}
	else {
		DEBUG_TRACE("MachineCode:%s", def.machineCode.c_str());
		err = ISSSetMachineCode(def.machineCode.c_str());
		DEBUG_TRACE(("ISSSetMachineCode return " + LongToString(err)).c_str());
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

	TimerForTest Create;

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
#endif

	DEBUG_TRACE(("ISSSRCreate return " + LongToString(err)).c_str());

	if (err != ISS_SUCCESS){
		DEBUG_TRACE("ISSSRCreate return error");
		return;
	}
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	if (!def.msgInitStatus){
		DEBUG_TRACE("ISSSRCreate msgInitStatus fail");
		//return;
	}

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


	vector< TParams > vec_pair;
	// Ê¶±ð
	vector<string> audio_paths = readFileByLine(testRes_path + audio_list_path, true);
	//userdata
	vector<string> userdatas = readFileByLine(testRes_path + user_data_path_, false);
	TParams TempPair;
	int num;
	if ((num = audio_paths.size()) == userdatas.size()) {
		DEBUG_TRACE("audio_paths.size() = userdatas.size()");
	}
	else {
		DEBUG_TRACE("audio_paths.size() != userdatas.size()");
	}
	for (int i = 0;i < num;i++) {
		TempPair.audio_path = audio_paths[i];
		TempPair.userdata   = userdatas[i];
		vec_pair.push_back(TempPair);
	}

	DEBUG_TRACE("SRConsistent batch loop num: %d", loop_num_);
	if (loop_num_ == -1)
		loop_num_ = 10000000;

	DEBUG_TRACE("ISSSRStart mode: %d", mode);

	for (int i = 0; i < loop_num_; i++) {
		int keep_num = 0;
		int audioNum = 0;
		//BOOST_FOREACH(TParams audioanddata, vec_pair)		
		for (int j = 0;j < num;j++) 
		{

			TParams audioanddata = vec_pair[j];
			string audio_path = audioanddata.audio_path;
			if(use_path_pre_ != ""){
				DEBUG_TRACE_INFOR("use_path_pre_:%s",use_path_pre_.c_str());
				audio_path = use_path_pre_ + audio_path;
			}

			audioanddata.userdata = string("{\"UserData\": ") + audioanddata.userdata + string("}");
			//DEBUG_TRACE("userdata: %s", PrtU8((audioanddata.userdata).c_str()));
			err = ISSSRUpLoadData(phISSSR, (audioanddata.userdata).c_str(), UploadDictMode);

			DEBUG_TRACE("ISSSRUpLoadData return %d", err);
			count = 0;
			if (UploadDictMode==0||UploadDictMode==1){
				while (!def.msgUpLoadDataToCloudStatus&&count++<1000)
					sleep(10);
			}
				
			if (UploadDictMode==0||UploadDictMode==1){
				if (def.msgUpLoadDataToCloudStatus) {
					DEBUG_TRACE("ISSSRUpLoadData success");
				}
				else {
					DEBUG_TRACE("ISSSRUpLoadData failed");
				}		
			}
			
			int current_num = GetAudioNumber((audioanddata.audio_path).c_str());
			if (keep_num != current_num) {
				if (current_num>1) {
					err = ISSSRStop(phISSSR);
					DEBUG_TRACE("ISSSRStop return %d", err);
				}
				err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, mode, "");
				DEBUG_TRACE("ISSSRStart return %d", err);
				keep_num = current_num;
			}
			DEBUG_TRACE_INFOR("current test num: %d, audio_path: %s", current_num, audio_path.c_str());
			
			err = ISSSRSetParam(phISSSR, "audioTag", VA::IntToString(++audioNum).c_str());
			DEBUG_TRACE("ISSSRSetParam audioTag audioNum %d ret %d", audioNum, err);

			appendAudioToAIUISR(phISSSR, audio_path , def, 800);
			DEBUG_TRACE("waiting for sr result");
			count = 0;
			while (!def.msgResult &&!def.msgError && 
				!def.msgSpeechTimeOut && !def.msgResponseTimeout && count++ < 1000)
				sleep(10);

			def.msgResult = 0;
			//sleep(1000);
			def.initMsg();

		}
	}

	
}


