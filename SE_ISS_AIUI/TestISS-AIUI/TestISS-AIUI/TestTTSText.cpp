#ifdef __ANDROID__

#include "TestTTSText.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

#include <vector>
#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

using namespace VA;
using std::string;
using std::vector;
using boost::lexical_cast;
using namespace boost::algorithm;
using namespace boost::filesystem;

TestTTSText::TestTTSText() :err(-1), phISSTTSRES(NULL), phISSTTS(NULL), text_(""),
							tts_audio_path_("") {
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	DEBUG_TRACE(("ISSTTSInitRes return " + lexical_cast<string>(err)).c_str());

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	DEBUG_TRACE(("ISSTTSCreate return " + lexical_cast<string>(err)).c_str());

	err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_SPEAKER, 9);
	DEBUG_TRACE(("speaker is set to 9 and ISSTTSSetParam return " + lexical_cast<string>(err)).c_str());
}

TestTTSText::~TestTTSText() {
	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	DEBUG_TRACE(("ISSTTSDestroy return " + lexical_cast<string>(err)).c_str());
	phISSTTS = NULL;

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	DEBUG_TRACE(("ISSTTSUnInitRes return " + lexical_cast<string>(err)).c_str());
	phISSTTSRES = NULL;
}

// 设置发音人
void TestTTSText::setSpeaker(int speaker) {
	err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_SPEAKER, speaker);
	DEBUG_TRACE(("speaker is set to " + lexical_cast<string>(speaker) + " and ISSTTSSetParam return " + 
		lexical_cast<string>(err)).c_str());
}

// 按文本进行合成
void TestTTSText::test() {
	vector<string> texts;
	split(texts, text_, is_any_of("&"));
	BOOST_FOREACH(string text, texts) {
		tts(text);
	}
}

// 读取文本列表文件进行合成
void TestTTSText::test(string & list_path) {
	vector<string> texts = readFileByLine(testRes_path + list_path, false);
	BOOST_FOREACH(string text, texts) {
		tts(text);
	}
}

void TestTTSText::tts(string & text) {
	DEBUG_TRACE_INFOR("tts text: %s", text.c_str());

	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text.c_str(), strlen(text.c_str()), ISS_TTS_CODEPAGE_UTF8);
	DEBUG_TRACE(("ISSTTSStart return " + lexical_cast<string>(err)).c_str());

	while (!def.msgOnDataReady)
		sleep(10);

	DEBUG_TRACE_INFOR("get Proc_OnTTSDataReady, start calling getAudioData");
	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;

	if (tts_audio_path_ != "") {
		// 创建合成音频目录
		bool ret = create_directories(tts_audio_path_);
		if (ret)
			DEBUG_TRACE_INFOR("create dir %s successed", PrtU8(tts_audio_path_.c_str()));

		// 设置合成出的音频文件名，替换掉一些不能作为文件名的字符
		string text_temp = replace_all_copy(text, "?", "#");
		replace_all(text_temp, "*", "#");
		replace_all(text_temp, "/", "#");
		replace_all(text_temp, "\\", "#");
		replace_all(text_temp, "<", "#");
		replace_all(text_temp, ">", "#");
		replace_all(text_temp, ":", "#");
		replace_all(text_temp, "\"", "#");
		replace_all(text_temp, "|", "#");

		// 创建音频文件
		string output_audio_name = PrtU8((tts_audio_path_ + text_temp + ".pcm").c_str());
		DEBUG_TRACE_INFOR("audio file name: %s", output_audio_name.c_str());
		std::ofstream fout(output_audio_name.c_str(), ios::out | ios::binary);
		if (!fout.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed, stop tts");
			fout.close();
			def.initMsg();

			return;
		}

		while (err != ISS_ERROR_TTS_COMPLETED) {
			err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);

			if (piBufferFramesGot != 0)
				fout.write(buffer, piBufferFramesGot * 2);
			else
				sleep(5);
		}

		fout.close();
	}
	else {
		while (err != ISS_ERROR_TTS_COMPLETED) {
			err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);

			if (piBufferFramesGot == 0) {
				sleep(5);
			}
		}
	}
	
	DEBUG_TRACE_INFOR("getAudioData done");
	def.initMsg();
}
#endif