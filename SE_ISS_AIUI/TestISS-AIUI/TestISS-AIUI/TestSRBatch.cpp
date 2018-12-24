#include "TestSRBatch.h"
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
using boost::lexical_cast;
using boost::timer;
using namespace rapidjson;
using namespace boost::algorithm;
using namespace VA;

#ifndef ISS_SR_3

TestSRBatch::TestSRBatch() :err(-1), phISSSR(NULL) {
	
}

TestSRBatch::~TestSRBatch() {
	err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	DEBUG_TRACE(("ISSSRDestroy return " + lexical_cast<string>(err)).c_str());

	def.initMsg();
}

void TestSRBatch::test(int lang, int mode, string audio_list_path, string test_type,
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

	//AIUI批量测试：只start一次，然后持续送音频进行识别
	/*if (test_type == "time") {
		DEBUG_TRACE_ERROR("AIUI SR time test doesn't support");
		return;
	}*/
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
	timer InitTime;
	// ISSSRCreate
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
	DEBUG_TRACE(("ISSSRCreate return " + lexical_cast<string>(err)).c_str());
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	if (!def.msgInitStatus)
		//return;

	DEBUG_TRACE("Init time:%f ", InitTime.elapsed() * 1000 );

	ofstream fout((testRes_path + "sr/batch/ret/sr_" + test_type + "_" + lang_str + "_" + mode_str + ".csv").c_str());
	if (!fout.is_open()) {
		DEBUG_TRACE_ERROR("create result file failed");
		return;
	}
	fout << "pcm,,,,SpeechEnd_time,SR_result_type,SR_response_time,,error_code,ISR_Result,,SR_full_result,vadSpeechStart,vadSpeechEnd,stop_appendAudioData_type\n";

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
		/*while (!def.msgUpLoadDictToLocalStatus )
			sleep(10);*/
		DEBUG_TRACE("after uploading dict, sleep 10s");
		sleep(10000);
	}

	// 识别
	vector<string> audio_paths = readFileByLine(testRes_path + audio_list_path, true);

	DEBUG_TRACE("sr batch loop num: %d", loop_num_);
	if (loop_num_ == -1)
		loop_num_ = 10000000;

	DEBUG_TRACE("ISSSRStart mode: %d", mode);
	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, mode, "");
	DEBUG_TRACE("ISSSRStart return %d", err);

	int test_num = 1; // 测试序号
	timer AppendAudioTime;
	double TotalTime = 0;
	double EachTime = 0;
	for (int i = 0; i < loop_num_; i++) {
		int audioNum = 0;
		BOOST_FOREACH(string audio_path, audio_paths) {
			DEBUG_TRACE_INFOR("current test num: %d, audio_path: %s", test_num++, audio_path.c_str());
			//double SpeechEnd_time = 0; // 音频结束到收到SpeechEnd消息的时间
			//double SR_response_time = 0;
			//int vad_SpeechEnd = 0;
			//string SR_result_type("");
			//int error_code = 0;
			//string SR_result("");
			//string SR_full_result("");
			//string stop_appendAudioData_type("get MSG_SpeechEnd");

			if (test_type == "time") {
				int audio_size = boost::filesystem::file_size(audio_path);
				double audio_vad_end = audio_size / 32 - getVadLabel(audio_path)[1]; // 音频后端点
				DEBUG_TRACE("get audio vad start point: %f", getVadLabel(audio_path)[0]);
				DEBUG_TRACE("get audio end silence: %f", getVadLabel(audio_path)[1]);
				DEBUG_TRACE("get audio vad end point: %f", audio_vad_end);
			}

			err = ISSSRAdapter::ISSSRSetParam(phISSSR, "audioTag", VA::IntToString(++audioNum).c_str());
			DEBUG_TRACE("ISSSRSetParam audioTag audioNum %d ret %d", audioNum, err);

			AppendAudioTime.restart();
			//appendAudio(phISSSR, "sr", audio_path, def);
			if (test_type == "time") {
				appendAudioToAIUISR(phISSSR, audio_path, def,320, 3000);
			}
			else {
				appendAudioToAIUISR(phISSSR, audio_path, def,1024, 0);
			}
			EachTime = AppendAudioTime.elapsed() * 1000;
			TotalTime += EachTime;

			/*if (!def.msgSpeechEnd) {
				vad_SpeechEnd = 0;
				stop_appendAudioData_type = "no MSG_SpeechEnd";
			}
			else {
				vad_SpeechEnd = def.vad_SpeechEnd;
			}*/
			if (test_type == "time") {
				int i = 0;
				while (!def.msgResult && i++ < 1000)
					sleep(10);
			}
			
			/*if (def.msgSpeechEnd&&audio_vad_end)
				SpeechEnd_time = def.speechEnd_time - audio_vad_end;
			else
				SpeechEnd_time = 0;*/

			//if (def.msgResult) {
			//	if (test_type == "time")
			//		SR_response_time = def.sr_response_time - audio_vad_end;
			//	else if (test_type == "effect"&&def.msgSpeechEnd)
			//		SR_response_time = def.sr_response_time - vad_SpeechEnd;
			//	else
			//		SR_response_time = 0;
			//	//DEBUG_TRACE_INFOR("sr result: %s", PrtU8(def.sr_result.c_str()));
			//	SR_full_result = string(PrtU8(def.sr_result.c_str()));

			//	Document doc;
			//	doc.Parse(SR_full_result.c_str());
			//	if (doc.HasMember("intent")) {
			//		Value & intent = doc["intent"];

			//		if (intent.HasMember("text")) {
			//			Value & text = intent["text"];
			//			SR_result =  string(text.GetString());
			//		}
			//	}

			//	replace_all(SR_full_result, "\n", "");
			//	replace_all(SR_full_result, "\"", "\"\"");

			//	replace_all(SR_result, "\n", "");
			//	replace_all(SR_result, "\"", "\"\"");

			//	SR_result_type = "MSG_Result";
			//}
			//else if (def.msgError) {
			//	SR_result_type = "MSG_Error";
			//	error_code = def.error_code;
			//}
			//else if (def.msgResponseTimeout) {
			//	SR_result_type = "MSG_ResponseTimeout";
			//	error_code = -1;
			//}
			//else if (def.msgSpeechTimeOut) {
			//	SR_result_type = "MSG_SpeechTimeOut";
			//	error_code = -1;
			//}

			//fout << audio_path << ",,,,";
			//fout << SpeechEnd_time << ",";
			//fout << SR_result_type << ",";
			//fout << SR_response_time << ",,";
			//fout << error_code << ",";
			//fout << "\"" << SR_result << "\",,";
			//fout << "\"" << SR_full_result << "\",";
			//fout << def.vad_SpeechStart << ",";
			//fout << vad_SpeechEnd << ",";
			//fout << stop_appendAudioData_type << "\n";
			//fout.flush();
			def.initMsg();
			//sleep(5);
		}
	}
	DEBUG_TRACE("AppendAudio resume total time:%f",TotalTime);
	fout.close();
}

// 提取音频名中标注的前段点和尾部静音长度
vector<double> TestSRBatch::getVadLabel(string audio_name) {
	double vad_start = 0;
	double tail_silence = 0;

	replace_all(audio_name, "\\", "/");
	vector<string> path_parts;
	split(path_parts, audio_name, is_any_of("/"));
	audio_name = path_parts[path_parts.size() - 1];

	vector<string> temp;
	split(temp, audio_name, is_any_of("_"));

	BOOST_FOREACH(string t, temp) {
		if (ends_with(t, "s")) {
			vector<string> time;
			split(time, t, is_any_of("s"));

			if (vad_start == 0)
				vad_start = lexical_cast<double>(time[0]) * 1000;
			else if (tail_silence == 0) {
				tail_silence = lexical_cast<double>(time[0]) * 1000;
				break;
			}
		}
	}

	vector<double> ret;
	ret.push_back(vad_start);
	ret.push_back(tail_silence);

	return ret;
}

#endif
