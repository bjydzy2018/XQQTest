#include "TestSRApiRandom.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

using std::string;
int sr_real2(){return rand() % 2;}
int sr_real3(){return rand() % 3;}
int sr_real17(){return rand() % 17;}
int sr_real6(){return rand() % 6;}
int sr_real10(){return rand() % 10;}
int sr_real11(){return rand() % 11;}
int sr_real30(){return rand() % 30;}

TestSRApiRandom::TestSRApiRandom() :err(-1), phISSSR(NULL) {}

void TestSRApiRandom::test() {
	/*boost::random::mt19937 gen;
	boost::uniform_int<> sr_real3(0, 3);
	boost::uniform_int<> sr_real17(0, 17);
	boost::uniform_int<> sr_real6(0, 6);
	boost::uniform_int<> sr_real10(0, 10);
	boost::uniform_int<> sr_real11(0, 11);
	boost::uniform_int<> sr_real30(0, 30);*/

	srand((unsigned)time(0));

	err = ISSSetMachineCode(def.machineCode.c_str());
	DEBUG_TRACE("ISSSetMachineCode return %d",err);
	if (err != ISS_SUCCESS)
		return;
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	DEBUG_TRACE("ISSSRCreate return %d",err);
	if (err != ISS_SUCCESS)
		return;

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	if (!def.msgInitStatus)
		return;

	VAThread_handle_t thread_handle_RoAppAuDa = 0;
	int ret1 = VAThreadCreate(&thread_handle_RoAppAuDa, (VAThread_func_t)&static_threadRollAppendAudioData, this, "RoAppAuDa");

	VAThread_handle_t thread_handle_RoUpDi = 0;
	int ret2 = VAThreadCreate(&thread_handle_RoUpDi, (VAThread_func_t)&static_threadRollUploadDict, this, "RoUpDi");

	VAThread_handle_t thread_handle_RoStart = 0;
	int ret3 = VAThreadCreate(&thread_handle_RoStart, (VAThread_func_t)&static_threadRollStart, this, "RoStart");

	while (true) {
		/*switch (sr_real3()) {
		case 0: {
			string scene = getStartScene(sr_real17());
			int mode = getStartMode(sr_real6());
			string szCmd = getSzCmd(sr_real3());

			DEBUG_TRACE(("call ISSSRStart, scene: " + scene + ", mode: " + LongToString(mode) + ", szCmd: " + szCmd).c_str());
			err = ISSSRStart(phISSSR, scene.c_str(), mode, (const char *)VA::GBKStrToU8Str(szCmd.c_str()));
			break;
		}
		case 1: {

			if (sr_real30() < 10) {
				DEBUG_TRACE("call ISSSRStop");
				err = ISSSRStop(phISSSR);
			}
			break;
		}
		case 2: {
#ifdef ISS_SR_3
			if (sr_real30() < 10) {
				DEBUG_TRACE("call ISSSREndAudioData");
				err = ISSSREndAudioData(phISSSR);
			}
#endif
			break;
		}
		case 3: {
			string szParam = getSzParam(sr_real10());
			string szParamValue = getSzParamValue(sr_real11());

			DEBUG_TRACE("call ISSSRSetParam");
			err = ISSSRSetParam(phISSSR, szParam.c_str(), (const char *)VA::GBKStrToU8Str(szParamValue.c_str()));
			break;
		}
		default:
			break;
		}*/
		SRJoggle();
	}
}

static VAThread_result_t static_threadRollAppendAudioData(void *args)
{
	TestSRApiRandom* pThis = (TestSRApiRandom*)args;
	pThis->threadRollAppendAudioData();
	return VAThread_result_t(0);
}

void TestSRApiRandom::threadRollAppendAudioData() {
	while (true) {
		DEBUG_TRACE("call ISSSRAppendAudioData");
		appendAudio(phISSSR, "sr", def.srPcm_CallBaiYaWei, def);

		sleep(3000);
	}
}

static VAThread_result_t static_threadRollUploadDict(void *args)
{
	TestSRApiRandom* pThis = (TestSRApiRandom*)args;
	pThis->threadRollUploadDict();
	return VAThread_result_t(0);
}

static VAThread_result_t static_threadRollStart(void *args)
{
	TestSRApiRandom* pThis = (TestSRApiRandom*)args;
	pThis->threadRollStart();
	return VAThread_result_t(0);
}

void TestSRApiRandom::threadRollStart() {
	while (true) {
		DEBUG_TRACE("threadRollStart");
		err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, NULL);
	}
}

void TestSRApiRandom::threadRollUploadDict() {

	while (true) {
		string dict("");
		int ifOnlyUploadToCloud = sr_real2();

		switch (sr_real2()) {
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
		err = ISSSRUpLoadDict(phISSSR, dict.c_str(), ifOnlyUploadToCloud);

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
	case 6:
		return ISS_SR_SCENE_ONESHOT;
	case 7:
		return ISS_SR_SCENE_STKS;
#ifdef ISS_SR_3
	case 8:
		return ISS_SR_SCENE_CMDLIST_WITHALL;
	case 9:
		return ISS_SR_SCENE_SELECTLIST_POI;
	case 10:
		return ISS_SR_SCENE_SELECTLIST_CONTACTS;
#endif
	case 11:
		return ISS_SR_SCENE_MUSIC;
	case 12:
		return ISS_SR_SCENE_HIMALAYAFM;
	case 13:
		return ISS_SR_SCENE_SELECT_MVW;
	case 14:
		return ISS_SR_SCENE_CONFIRM_MVW;
	case 15:
		return ISS_SR_SCENE_ANSWER_CALL_MVW;
	case 16:
		return ISS_SR_SCENE_BUILD_GRM_MVW;
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
	case 3:
		return ISS_SR_MODE_LOCAL_CMDLIST;
	case 4:
		return ISS_SR_MODE_LOCAL_NLP;
	case 5:
		return ISS_SR_MODE_LOCAL_MVW;
	default:
		return ISS_SR_MODE_MIX_REC;
	}
}

string TestSRApiRandom::getSzCmd(int item) {
	switch (item) {
	case 0:
		return "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [{ \"name\": \"司小四要拿四十四斤四两西红柿换史小世十四斤四两细蚕丝\", \"id\": 0 }]}]}";
	case 1:
		return "{\"grm\": [{\"dictname\":\"cmd\",\"dictcontant\": [\"name\": \"司小四要拿四十四斤四两西红柿换史小世十四斤四两细蚕丝\", \"id\": 0 }]}]}";
	case 2:
		return "{\"grm\": \"a\"}";
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
	case 2:
		return ISS_SR_PARAM_SPEECH_TIMEOUT;
	case 3:
		return ISS_SR_PARAM_IAT_EXTEND_PARAMS;
	case 4:
		return ISS_SR_PARAM_RESPONSE_TIMEOUT;
	case 5:
		return ISS_SR_PARAM_SPEECH_TAIL;
	case 6:
		return ISS_SR_PARAM_CITY;
	case 7:
		return ISS_SR_PARAM_TRACE_LEVEL;
	case 8:
		return ISS_SR_MVW_PARAM_AEC;
	case 9:
		return ISS_SR_MVW_PARAM_LSA;
	default:
		return ISS_SR_PARAM_ACOUS_LANG;
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
	case 7:
		return ISS_SR_PARAM_ACOUS_LANG_VALUE_MANDARIN;
	case 8:
		return ISS_SR_PARAM_ACOUS_LANG_VALUE_ENGLISH;
	case 9:
		return ISS_SR_PARAM_ACOUS_LANG_VALUE_CANTONESE;
	case 10:
		return ISS_SR_PARAM_ACOUS_LANG_VALUE_HENANESE;
	default:
		return "=.=";
	}
}

void TestSRApiRandom::SRJoggle(){
	switch (sr_real3()) {
		case 0: {
			string scene = getStartScene(sr_real17());
			int mode = getStartMode(sr_real6());
			string szCmd = getSzCmd(sr_real3());

			DEBUG_TRACE(("call ISSSRStart, scene: " + scene + ", mode: " + LongToString(mode) + ", szCmd: " + szCmd).c_str());
			err = ISSSRStart(phISSSR, scene.c_str(), mode, (const char *)VA::GBKStrToU8Str(szCmd.c_str()));
			break;
				}
		case 1: {

			if (sr_real30() < 10) {
				DEBUG_TRACE("call ISSSRStop");
				err = ISSSRStop(phISSSR);
			}
			break;
				}
		case 2: {
#ifdef ISS_SR_3
			if (sr_real30() < 10) {
				DEBUG_TRACE("call ISSSREndAudioData");
				err = ISSSREndAudioData(phISSSR);
			}
#endif
			break;
				}
		case 3: {
			string szParam = getSzParam(sr_real10());
			string szParamValue = getSzParamValue(sr_real11());

			DEBUG_TRACE("call ISSSRSetParam");
			err = ISSSRSetParam(phISSSR, szParam.c_str(), (const char *)VA::GBKStrToU8Str(szParamValue.c_str()));
			break;
				}
		default:
			break;
	}
}