#include "TestSRConsistent.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

#include "rapidjson/document.h"

#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/timer.hpp>
#include <boost/filesystem.hpp>

using std::string;
using std::vector;
using std::pair;
using boost::lexical_cast;
using boost::timer;
using namespace rapidjson;
using namespace boost::algorithm;
using namespace VA;

typedef struct tagParams {
	string audio_path;
	string userdata;
} TParams;

#ifndef ISS_SR_3

TestSRConsistent::TestSRConsistent() :err(-1), phISSSR(NULL) {
	
}

TestSRConsistent::~TestSRConsistent() {
	err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	DEBUG_TRACE(("ISSSRDestroy return " + lexical_cast<string>(err)).c_str());

	def.initMsg();
}

int TestSRConsistent::GetAudioNumber(std::string audio_path) {
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
void TestSRConsistent::test(int lang, int mode, string audio_list_path, string test_type,
	string dict_path, int ifOnlyUploadToCloud) {
	if (machineCode_ != "") {
		DEBUG_TRACE("MachineCode:%s", machineCode_.c_str());
		err = ISSSRAdapter::ISSSetMachineCode(machineCode_.c_str());
		DEBUG_TRACE(("ISSSetMachineCode return " + lexical_cast<string>(err)).c_str());
	}
	else {
		DEBUG_TRACE("MachineCode:%s", def.machineCode.c_str());
		err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
		DEBUG_TRACE(("ISSSetMachineCode return " + lexical_cast<string>(err)).c_str());
	}

	if (test_type == "time") {
		DEBUG_TRACE_ERROR("AIUI SR time test doesn't support");
		return;
	}
	string sr_ret_path = "TestRes/sr/consistent/ret/sr_consistent.txt" ;
	ofstream fout(sr_ret_path.c_str(), ios::out);

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
	timer InitTime;
	// ISSSRCreate
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
	DEBUG_TRACE(("ISSSRCreate return " + lexical_cast<string>(err)).c_str());
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

	DEBUG_TRACE("Init time:%f ", InitTime.elapsed() * 1000);
	DEBUG_TRACE("after creating, sleep 10s");
	sleep(10000);

	if (dict_path != "") {
		// ISSSRUpLoadDict
		err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, readFile(testRes_path + dict_path, false).c_str(), ifOnlyUploadToCloud);
		DEBUG_TRACE(("ISSSRUpLoadDict return " + lexical_cast<string>(err)).c_str());

		DEBUG_TRACE("waiting for uploading dict");
		if (!ifOnlyUploadToCloud)
			while (!def.msgUpLoadDictToLocalStatus)
				sleep(10);
		else
			while (!def.msgUpLoadDictToCloudStatus)
				sleep(10);
		//目前只支持本地传个性化，云端个性化不支持
		/*while (!def.msgUpLoadDictToLocalStatus)
			sleep(10);*/
		DEBUG_TRACE("after uploading dict, sleep 10s");
		sleep(10000);
	}

	vector< TParams > vec_pair;
	// 识别
	vector<string> audio_paths = readFileByLine(testRes_path + audio_list_path, true);
	//userdata
	vector<string> userdatas = readFileByLine(testRes_path + user_data_path_, false);
	TParams TempPair;
	int Count;
	if ((Count = audio_paths.size()) == userdatas.size()) {
		DEBUG_TRACE("audio_paths.size() = userdatas.size()");
	}
	else {
		DEBUG_TRACE("audio_paths.size() != userdatas.size()");
	}
	for (int i = 0;i < Count;i++) {
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
		BOOST_FOREACH(TParams audioanddata, vec_pair)
		{
			audioanddata.userdata = string("{\"UserData\": ") + audioanddata.userdata + string("}");
			//DEBUG_TRACE("userdata: %s", PrtU8((audioanddata.userdata).c_str()));
			err = ISSSRAdapter::ISSSRUpLoadData(phISSSR, (audioanddata.userdata).c_str(), ifOnlyUploadToCloud);
			DEBUG_TRACE("ISSSRUpLoadData return %d", err);
			int i = 0;
			while (!def.msgUpLoadDataToCloudStatus && i++ < 1000) {
				sleep(10);
			}
			if (def.msgUpLoadDataToCloudStatus) {
				DEBUG_TRACE("ISSSRUpLoadData success");
			}
			else {
				DEBUG_TRACE("ISSSRUpLoadData failed");
			}
			
			int current_num = GetAudioNumber((audioanddata.audio_path).c_str());
			if (keep_num != current_num) {
				if (current_num>1) {
					err = ISSSRAdapter::ISSSRStop(phISSSR);
					DEBUG_TRACE("ISSSRStop return %d", err);
				}
				err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, mode, "");
				DEBUG_TRACE("ISSSRStart return %d", err);
				keep_num = current_num;
			}
			DEBUG_TRACE_INFOR("current test num: %d, audio_path: %s", current_num, (audioanddata.audio_path).c_str());
			fout << (audioanddata.audio_path).c_str();
			fout << "\t";
			err = ISSSRAdapter::ISSSRSetParam(phISSSR, "audioTag", VA::IntToString(++audioNum).c_str());
			DEBUG_TRACE("ISSSRSetParam audioTag audioNum %d ret %d", audioNum, err);

			appendAudioToAIUISR(phISSSR, (audioanddata.audio_path), def,320, 800);
			DEBUG_TRACE("waiting for sr result");
			i = 0;
			while (!def.msgResult && i++ < 1000)
				sleep(10);

			if (def.msgResult) {
				fout << PrtU8(def.sr_result.c_str());
				//DEBUG_TRACE_INFOR("sr result: %s", PrtU8(def.sr_result.c_str()));
			}
			def.msgResult = 0;
			sleep(1000);
			if (def.msgResult) {
				fout << PrtU8(def.sr_result.c_str());
			}else {
				fout << endl;
			}
			def.initMsg();

		}
	}

	fout.close();
}

#endif
