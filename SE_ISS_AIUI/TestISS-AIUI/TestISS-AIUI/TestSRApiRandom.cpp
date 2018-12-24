#include "TestSRApiRandom.h"
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

TestSRApiRandom::TestSRApiRandom() :err(-1), phISSSR(NULL) {}

void TestSRApiRandom::test() {
	boost::random::mt19937 gen;
	boost::uniform_int<> real3(0, 3);
	boost::uniform_int<> real17(0, 17);
	boost::uniform_int<> real6(0, 6);
	boost::uniform_int<> real10(0, 10);
	boost::uniform_int<> real11(0, 11);
	boost::uniform_int<> real30(0, 30);

	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	DEBUG_TRACE(("ISSSetMachineCode return " + lexical_cast<string>(err)).c_str());
	if (err != ISS_SUCCESS)
		return;
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	DEBUG_TRACE(("ISSSRCreate return " + lexical_cast<string>(err)).c_str());
	if (err != ISS_SUCCESS)
		return;
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	if (!def.msgInitStatus)
		return;

	thread threadRollAppendAudioData(bind(&TestSRApiRandom::threadRollAppendAudioData, this));
	thread threadRollUploadDict(bind(&TestSRApiRandom::threadRollUploadDict, this));

	timer t;
	double sum_time = 0;
	double expect_time = run_time_ * 60 * 60 * 1000;
	while (sum_time < expect_time) {
		t.restart();
		switch (real3(gen)) {
		case 0: {
			string scene = getStartScene(real17(gen));
			int mode = getStartMode(real6(gen));
			string szCmd = getSzCmd(real3(gen));

			DEBUG_TRACE(("call ISSSRStart, scene: " + scene + ", mode: " + lexical_cast<string>(mode) + ", szCmd: " + szCmd).c_str());
			err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_CLOUD_REC, (const char *)VA::GBKStrToU8Str(szCmd.c_str()));
			break;
		}
		case 1: {
			boost::random::mt19937 gen;
			boost::uniform_int<> real(0, 30);

			if (real(gen) < 10) {
				DEBUG_TRACE("call ISSSRStop");
				err = ISSSRAdapter::ISSSRStop(phISSSR);
			}
			break;
		}
		case 2: {
#ifdef ISS_SR_3
			if (real30(gen) < 10) {
				DEBUG_TRACE("call ISSSREndAudioData");
				err = ISSSREndAudioData(phISSSR);
			}
#endif
			break;
		}
		case 3: {
			string szParam = getSzParam(real10(gen));
			string szParamValue = getSzParamValue(real11(gen));

			DEBUG_TRACE("call ISSSRSetParam");
			err = ISSSRAdapter::ISSSRSetParam(phISSSR, szParam.c_str(), (const char *)VA::GBKStrToU8Str(szParamValue.c_str()));
			break;
		}
		default:
			break;
		}
		double each_time = t.elapsed() * 1000;
		sum_time += each_time;
	}
	ISSSRAdapter::ISSSRDestroy(phISSSR);
	DEBUG_TRACE(("ISSSRDestroy return " + lexical_cast<string>(err)).c_str());
}

void TestSRApiRandom::threadRollAppendAudioData() {
	while (true) {
		DEBUG_TRACE("call ISSSRAppendAudioData");
		appendAudio(phISSSR, "sr", def.srPcm_CallBaiYaWei, def);

		sleep(3000);
	}
}

void TestSRApiRandom::threadRollUploadDict() {
	boost::random::mt19937 gen;
	boost::uniform_int<> real(0, 2);

	while (true) {
		string dict("");
		int ifOnlyUploadToCloud = real(gen);

		switch (real(gen)) {
		case 0:
			dict = def.dict_500;
			break;
		case 1:
			dict = def.dict_1000;
			break;
		default:
			dict = def.dict_5000;
		}

		DEBUG_TRACE("call ISSSRUpLoadDict");
		err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, dict.c_str(), ifOnlyUploadToCloud);

		sleep(5000);
	}
}

string TestSRApiRandom::getStartScene(int item) {
	switch (item)
	{
	case 0:
		return ISS_SR_SCENE_ALL;
	case 1:
		return ISS_SR_SCENE_POI;
	case 2:
		return ISS_SR_SCENE_CONTACTS;
	case 3:
		return ISS_SR_SCENE_SELECT;
	case 4:
		return ISS_SR_SCENE_CONFIRM;
	case 5:
		return ISS_SR_SCENE_ANSWER_CALL;
#ifdef ISS_SR_3
	case 6:
		return ISS_SR_SCENE_ONESHOT;
	case 7:
		return ISS_SR_SCENE_STKS;
	case 8:
		return ISS_SR_SCENE_CMDLIST_WITHALL;
	case 9:
		return ISS_SR_SCENE_SELECTLIST_POI;
	case 10:
		return ISS_SR_SCENE_SELECTLIST_CONTACTS;
	case 11:
		return ISS_SR_SCENE_HIMALAYAFM;
	case 12:
		return ISS_SR_SCENE_SELECT_MVW;
	case 13:
		return ISS_SR_SCENE_CONFIRM_MVW;
	case 14:
		return ISS_SR_SCENE_ANSWER_CALL_MVW;
	case 15:
		return ISS_SR_SCENE_BUILD_GRM_MVW;
#endif
	case 16:
		return ISS_SR_SCENE_MUSIC;
	default:
		return ISS_SR_SCENE_ALL;
	}
}

int TestSRApiRandom::getStartMode(int item) {
	switch (item)
	{
	case 0:
		return ISS_SR_MODE_CLOUD_REC;
	case 1:
		return ISS_SR_MODE_LOCAL_REC;
	case 2:
		return ISS_SR_MODE_MIX_REC;
#ifdef ISS_SR_3
	case 3:
		return ISS_SR_MODE_LOCAL_CMDLIST;
	case 4:
		return ISS_SR_MODE_LOCAL_NLP;
	case 5:
		return ISS_SR_MODE_LOCAL_MVW;
#endif
	default:
		return ISS_SR_MODE_MIX_REC;
	}
}

string TestSRApiRandom::getSzCmd(int item) {
	switch (item) {
	case 0:
		return "{\"dictname\":\"cmd\",\"dictcontant\": [{ \"name\": \"司小四要拿四十四斤四两西红柿换史小世十四斤四两细蚕丝\", \"id\": 0 }]}";
	case 1:
		return "{\"dictname\":\"cmd\",\"dictcontant\": [\"name\": \"司小四要拿四十四斤四两西红柿换史小世十四斤四两细蚕丝\", \"id\": 0 }]}";
	case 2:
		return "{\"dictname\": \"a\"}";
	default:
		return "";
	}
}

string TestSRApiRandom::getSzParam(int item) {
	switch (item) {
	case 0:
		return ISS_SR_PARAM_LONGTITUDE;
	case 1:
		return ISS_SR_PARAM_LATITUDE;
#ifdef ISS_SR_3
	case 2:
		return ISS_SR_PARAM_SPEECH_TIMEOUT;
	case 3:
		return ISS_SR_PARAM_RESPONSE_TIMEOUT;
	case 4:
		return ISS_SR_MVW_PARAM_AEC;
	case 5:
		return ISS_SR_MVW_PARAM_LSA;
	case 6:
		return ISS_SR_PARAM_ACOUS_LANG;
#endif
	case 7:
		return ISS_SR_PARAM_IAT_EXTEND_PARAMS;
	case 8:
		return ISS_SR_PARAM_SPEECH_TAIL;
	case 9:
		return ISS_SR_PARAM_CITY;
	default:
		return ISS_SR_PARAM_TRACE_LEVEL;
	}
}

string TestSRApiRandom::getSzParamValue(int item) {
	switch (item) {
	case 0:
		return "102.4";
	case 1:
		return "青岛";
	case 2:
		return "陶喆";
	case 3:
		return "△╲Ⅸ❽‰にオУ┿夊Óū";
	case 4:
		return "";
	case 5:
		return "on";
	case 6:
		return "off";
#ifdef ISS_SR_3
	case 7:
		return ISS_SR_PARAM_ACOUS_LANG_VALUE_MANDARIN;
	case 8:
		return ISS_SR_PARAM_ACOUS_LANG_VALUE_ENGLISH;
	case 9:
		return ISS_SR_PARAM_ACOUS_LANG_VALUE_CANTONESE;
	case 10:
		return ISS_SR_PARAM_ACOUS_LANG_VALUE_HENANESE;
#endif
	default:
		return "=.=";
	}
}
