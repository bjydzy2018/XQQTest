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

#ifdef ISS_SR_3

TestSRBatch3::TestSRBatch3():err(-1), phISSSR(NULL) {
	
}

TestSRBatch3::~TestSRBatch3() {
	err = ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	DEBUG_TRACE("ISSSRDestroy return %d",err);

	def.initMsg();
}

void TestSRBatch3::test(int lang, int mode, string audio_list_path, string test_type,
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

	TimerForTest Create;
	// ISSSRCreate
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
	DEBUG_TRACE("ISSSRCreate return %d",err);
	if (err != ISS_SUCCESS)
		return;
	
	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	if (!def.msgInitStatus)
		return;

	DEBUG_TRACE("SR Create Time: %f",Create.elapsed());

	ofstream fout((testRes_path + "sr/batch/ret/sr_" + test_type +"_" + lang_str + "_" + mode_str + ".csv").c_str());
	if (!fout.is_open()) {
		DEBUG_TRACE_ERROR("create result file failed");
		return;
	}
	fout << "pcm,,,,SpeechEnd_time,SR_result_type,SR_response_time,,error_code,ISR_Result,,SR_full_result,vadSpeechStart,vadSpeechEnd,stop_appendAudioData_type\n";

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

	
	if (loop_num_ == -1)
		loop_num_ = 10000000;
	DEBUG_TRACE("sr batch loop num: %d", loop_num_);
	
	int test_num = 1; // 测试序号
	for (int m = 0; m < loop_num_; m++) {
		for(size_t n = 0; n < audio_paths.size(); ++n){
			string audio_path = audio_paths[n];
			if(use_path_pre_ != ""){
				DEBUG_TRACE_INFOR("use_path_pre_:%s",use_path_pre_.c_str());
				audio_path = use_path_pre_ + audio_path;
			}
			DEBUG_TRACE_INFOR("CURRENT TEST NUM: %d, audio_path: %s", test_num++, audio_path.c_str());
			double SpeechEnd_time = 0; // 音频结束到收到SpeechEnd消息的时间
			double SR_response_time = 0;
			double audio_vad_end = 0;
			int vad_SpeechEnd = 0;
			string SR_result_type("");
			int error_code = 0;
			string SR_result("");
			string SR_full_result("");
			string stop_appendAudioData_type("get MSG_SpeechEnd");

			if (test_type == "time") {
				long audio_size = GetFileSize(audio_path);
				audio_vad_end = audio_size / 32 - getVadLabel(audio_path)[1]; // 音频后端点
				DEBUG_TRACE("get audio vad start point: %f", getVadLabel(audio_path)[0]);
				DEBUG_TRACE("get audio end silence: %f", getVadLabel(audio_path)[1]);
				DEBUG_TRACE("get audio vad end point: %f", audio_vad_end);
			}

			DEBUG_TRACE("before ISSSRStart");
			err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, mode, "");
			DEBUG_TRACE("ISSSRStart return %d", err);

			DEBUG_TRACE("before appendAudio");

			TimerForTest t;
			appendAudio(phISSSR, "sr", audio_path, def);

			DEBUG_TRACE("after appendAudio");
			
			if (test_type == "time"){
				while(!def.msgResult && !def.msgSpeechEnd && 
					!def.msgSpeechTimeOut && !def.msgResponseTimeout && !def.msgError)
				{
					sleep(10);
				}
				DEBUG_TRACE("time test");
			}
			
			bool call_endAudioData = false;
			if (def.msgSpeechEnd){
				def.speechEnd_time = t.elapsed();
				vad_SpeechEnd = def.vad_SpeechEnd;
			}else{
				call_endAudioData = true;
				stop_appendAudioData_type = "call EndAudioData()";

				t.restart(); // 如果没有检测到后端点，计时从调用ISSSREndAudioData重新开始

				DEBUG_TRACE("before ISSSREndAudioData");
				err = ISSSREndAudioData(phISSSR);
				DEBUG_TRACE("ISSSREndAudioData return %d", err);
			}

			int i = 0;
			while (!def.msgResult && !def.msgError && 
				!def.msgSpeechTimeOut && !def.msgResponseTimeout && i++ < 1000 ){
					sleep(10);
			}

			DEBUG_TRACE("after waiting for result");

			if (def.msgResult) {
				//def.sr_response_time = t.elapsed() * 1000;
				def.sr_response_time = t.elapsed();
			}

			if (test_type == "time")
				SpeechEnd_time = def.speechEnd_time - audio_vad_end;
			else if (test_type == "effect")
				SpeechEnd_time = 0;

			if (def.msgResult) {
				if (call_endAudioData){
					SR_response_time = t.elapsed();
				}
				else {
					if (test_type == "time")
						SR_response_time = def.sr_response_time - audio_vad_end;
					else if (test_type == "effect")
						SR_response_time = def.sr_response_time - vad_SpeechEnd;
				}

				DEBUG_TRACE_INFOR("sr result: %s", PrtU8(def.sr_result.c_str()));
				SR_full_result = string(VA::U8StrToGBKStr(def.sr_result.c_str()));

				Document doc;
				doc.Parse(SR_full_result.c_str());
				if (doc.HasMember("intent")) {
					Value & intent = doc["intent"];

					if (intent.HasMember("text")) {
						SR_result= intent["text"].GetString();
					}
				}

				replace_all(SR_full_result, "\n", "");
				replace_all(SR_full_result, "\"", "\"\"");

				replace_all(SR_result, "\n", "");
				replace_all(SR_result, "\"", "\"\"");

				SR_result_type = "MSG_Result";
			}
			else if (def.msgError) {
				SR_result_type = "MSG_Error";
				error_code = def.error_code;
			}
			else if (def.msgResponseTimeout) {
				SR_result_type = "MSG_ResponseTimeout";
				error_code = -1;
			}
			else if (def.msgSpeechTimeOut) {
				SR_result_type = "MSG_SpeechTimeOut";
				error_code = -1;
			}

			fout << audio_path << ",,,,";
			fout << SpeechEnd_time << ",";
			fout << SR_result_type << ",";
			fout << SR_response_time << ",,";
			fout << error_code << ",";
			fout << "\"" << SR_result << "\",,";
			fout << "\"" << SR_full_result << "\",";
			fout << def.vad_SpeechStart << ",";
			/*fout << vad_SpeechEnd << ",";*/
			fout << def.vad_SpeechEnd << ",";
			fout << stop_appendAudioData_type << "\n";
			fout.flush();

			def.initMsg();
		}
	}

	fout.close();
}

// 提取音频名中标注的前段点和尾部静音长度
vector<double> TestSRBatch3::getVadLabel(string audio_name) {
	double vad_start = 0;
	double tail_silence = 0;

	replace_all(audio_name, "\\", "/");
	vector<string> path_parts = split( audio_name, '/');
	audio_name = path_parts[path_parts.size() - 1];

	vector<string> temp = split( audio_name, '_');

	for(size_t i = 0; i < temp.size(); ++i){
		string t = temp[i];
		if (endswith(t, "s")) {
			vector<string> time = split(t, 's');

			if (vad_start == 0)
				vad_start = atof(time[0].c_str()) * 1000;
			else if (tail_silence == 0) {
				tail_silence = atof(time[0].c_str()) * 1000;
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
