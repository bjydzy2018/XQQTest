#include "TestTTSApiRandom.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

using namespace VA;
using std::string;

int tts_real3(){
	return rand() % 3;
}
int tts_real5(){
	return rand() % 5;
}
int tts_real32768(){
	return (rand() % 32768)*2 - 32768;
}

TestTTSApiRandom::TestTTSApiRandom() :err(-1), phISSTTSRES(NULL), phISSTTS(NULL) {}

void TestTTSApiRandom::test() {
	/*boost::random::mt19937 gen;
	boost::uniform_int<> tts_real3(0, 3);
	boost::uniform_int<> tts_real5(0, 5);
	boost::uniform_int<> tts_tts_real32768(-32768, 32768);*/

	srand((unsigned)time(0));

	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 0);
	DEBUG_TRACE("ISSTTSInitRes return %d",err);
	if (err != ISS_SUCCESS)
		return;

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	DEBUG_TRACE("ISSTTSCreate return %d",err);
	if (err != ISS_SUCCESS)
		return;

	while (true) {
		switch (tts_real3())
		{
		case 0: {
			const char * text = "I am a man of fortune, and i must seek my fortune";
			DEBUG_TRACE("call ISSTTSStart");
#if VA_VC
			err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
#elif VA_GNUC
			err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_UTF8);
#endif
			break;
		}
		case 1: {
			DEBUG_TRACE("call ISSTTSStop");
			err = ISSTTSStop(phISSTTS);
			break;
		}
		case 2: {
			char buffer[320] = { 0 };
			unsigned int piBufferFramesGot = 0;
			unsigned int piProgBegin = 0;
			unsigned int piProgLen = 0;
			DEBUG_TRACE("call ISSTTSGetAudioData");
			err = ISSTTSGetAudioData(phISSTTS, buffer, 160, &piBufferFramesGot, &piProgBegin, &piProgLen);
			break;
		}
		case 3: {
			int param = getParam(tts_real5());
			int paramValue = tts_real32768();
			DEBUG_TRACE(("call ISSTTSSetParam, param: " + LongToString(param) + ", paramValue: " + LongToString(paramValue)).c_str());
			err = ISSTTSSetParam(phISSTTS, param, paramValue);
			break;
		}
		default:
			break;
		}
	}
}

int TestTTSApiRandom::getParam(int item) {
	switch (item) {
	case 0:
		return ISS_TTS_PARAM_VOICE_SPEED;
	case 1:
		return ISS_TTS_PARAM_VOICE_PITCH;
	case 2:
		return ISS_TTS_PARAM_VOLUME;
	case 3:
		return ISS_TTS_PARAM_USERMODE;
	case 4:
		return ISS_TTS_PARAM_VOLUME_INCREASE;
	default:
		return 0;
	}
}

