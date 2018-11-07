#include "TestTTSText.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

#include <vector>
#include <iostream>
#include <fstream>


using namespace VA;
using std::string;
using std::vector;

TestTTSText::TestTTSText() :err(-1), phISSTTSRES(NULL), phISSTTS(NULL), text_(""),
							tts_audio_path_("") {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	DEBUG_TRACE("ISSTTSInitRes return %d" , err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	DEBUG_TRACE("ISSTTSCreate return %d" , err);

	err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_SPEAKER, 9);
	DEBUG_TRACE("speaker is set to 9 and ISSTTSSetParam return %d" , err);
}

TestTTSText::~TestTTSText() {
	err = ISSTTSDestroy(phISSTTS);
	DEBUG_TRACE("ISSTTSDestroy return %d" , err);
	phISSTTS = NULL;

	err = ISSTTSUnInitRes(phISSTTSRES);
	DEBUG_TRACE("ISSTTSUnInitRes return %d" , err);
	phISSTTSRES = NULL;
}

// ���÷�����
void TestTTSText::setSpeaker(int speaker) {
	err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_SPEAKER, speaker);
	DEBUG_TRACE("speaker is set to %d and ISSTTSSetParam return %d",speaker,err);
}

// ���ı����кϳ�
void TestTTSText::test() {
	vector<string> texts = split(text_, '&');
	for(size_t i = 0; i < texts.size(); ++i){
		tts(texts[i]);
	}
}

// ��ȡ�ı��б��ļ����кϳ�
void TestTTSText::test(string & list_path) {
	vector<string> texts = readFileByLine(testRes_path + list_path, false);
	for(size_t i = 0; i < texts.size(); ++i){
		tts(texts[i]);
	}
}

void TestTTSText::tts(string & text) {
	DEBUG_TRACE_INFOR("tts text: %s", text.c_str());

	err = ISSTTSStart(phISSTTS, text.c_str(), strlen(text.c_str()), ISS_TTS_CODEPAGE_UTF8);
	DEBUG_TRACE("ISSTTSStart return %d" , err);

	while (!def.msgOnDataReady)
		sleep(10);

	DEBUG_TRACE_INFOR("get Proc_OnTTSDataReady, start calling getAudioData");
	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;

	if (tts_audio_path_ != "") {

		// ���úϳɳ�����Ƶ�ļ������滻��һЩ������Ϊ�ļ������ַ�
		string text_temp = text;
		replace_all(text_temp, "?", "#");
		replace_all(text_temp, "*", "#");
		replace_all(text_temp, "/", "#");
		replace_all(text_temp, "\\", "#");
		replace_all(text_temp, "<", "#");
		replace_all(text_temp, ">", "#");
		replace_all(text_temp, ":", "#");
		replace_all(text_temp, "\"", "#");
		replace_all(text_temp, "|", "#");

		// ������Ƶ�ļ�
		string output_audio_name = PrtU8((testRes_path + tts_audio_path_ + text_temp + ".pcm").c_str());
		DEBUG_TRACE_INFOR("audio file name: %s", output_audio_name.c_str());
		std::ofstream fout(output_audio_name.c_str(), ios::out | ios::binary);
		if (!fout.is_open()) {
			DEBUG_TRACE_ERROR("audio file open failed, stop tts");
			fout.close();
			def.initMsg();

			return;
		}

		while (err != ISS_ERROR_TTS_COMPLETED) {
			err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);

			if (piBufferFramesGot != 0)
				fout.write(buffer, piBufferFramesGot * 2);
			else
				sleep(5);
		}

		fout.close();
	}
	else {
		while (err != ISS_ERROR_TTS_COMPLETED) {
			err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);

			if (piBufferFramesGot == 0) {
				sleep(5);
			}
		}
	}
	
	DEBUG_TRACE_INFOR("getAudioData done");
	def.initMsg();
}
