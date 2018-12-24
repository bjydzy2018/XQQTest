#ifdef __ANDROID__
#include "TestTTSApiRandom.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>
#include <boost/timer.hpp>
#include <boost/bind.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

using namespace VA;
using std::string;
using boost::lexical_cast;
using boost::thread;
using boost::timer;
using boost::bind;

TestTTSApiRandom::TestTTSApiRandom() :err(-1), phISSTTSRES(NULL), phISSTTS(NULL) {}

void TestTTSApiRandom::test() {
	boost::random::mt19937 gen;
	boost::uniform_int<> real3(0, 3);
	boost::uniform_int<> real5(0, 5);
	boost::uniform_int<> real32768(-32768, 32768);

	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	DEBUG_TRACE(("ISSTTSInitRes return " + lexical_cast<string>(err)).c_str());
	if (err != ISS_SUCCESS)
		return;

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	DEBUG_TRACE(("ISSTTSCreate return " + lexical_cast<string>(err)).c_str());
	if (err != ISS_SUCCESS)
		return;

	timer t;
	double sum_time = 0;
	double expect_time = run_time_ * 60 * 60 * 1000;
	while (sum_time < expect_time) {
		t.restart();
		switch (real3(gen))
		{
		case 0: {
			const char * text = "I am a man of fortune, and i must seek my fortune";
			DEBUG_TRACE("call ISSTTSStart");
#if VA_VC
			err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
#elif VA_GNUC
			err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_UTF8);
#endif
			break;
		}
		case 1: {
			DEBUG_TRACE("call ISSTTSStop");
			err = ISSTTSAdapter::ISSTTSStop(phISSTTS);
			break;
		}
		case 2: {
			char buffer[320] = { 0 };
			unsigned int piBufferFramesGot = 0;
			unsigned int piProgBegin = 0;
			unsigned int piProgLen = 0;
			DEBUG_TRACE("call ISSTTSGetAudioData");
			err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, buffer, 160, &piBufferFramesGot, &piProgBegin, &piProgLen);
			break;
		}
		case 3: {
			int param = getParam(real5(gen));
			int paramValue = real32768(gen);
			DEBUG_TRACE(("call ISSTTSSetParam, param: " + lexical_cast<string>(param) + ", paramValue: " + lexical_cast<string>(paramValue)).c_str());
			err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, param, paramValue);
			break;
		}
		default:
			break;
		}
		double each_time = t.elapsed() * 1000;
		sum_time += each_time;
	}
	ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
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
#endif
