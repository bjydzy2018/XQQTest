#include "TestMVWApiRandom.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

using std::string;

int mvw_real6(){return rand() % 6;}
int mvw_real3(){return rand() % 3;};
int mvw_real4(){return rand() % 4;};
int mvw_real11(){return rand() % 11;};
int mvw_real20(){return rand() % 20;};

TestMVWApiRandom::TestMVWApiRandom() :err(-1), phISSMVW(NULL) {}

void TestMVWApiRandom::test() {
	/*boost::random::mt19937 gen;
	boost::uniform_int<> mvw_real6(0, 6);
	boost::uniform_int<> mvw_real3(0, 3);
	boost::uniform_int<> mvw_real4(0, 4);
	boost::uniform_int<> mvw_real11(0, 11);
	boost::uniform_int<> mvw_real20(0, 20);*/

	srand((unsigned)time(0));

	err = ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL);
	DEBUG_TRACE("ISSMVWCreate return %d",err);
	if (err != ISS_SUCCESS)
		return;

	VAThread_handle_t thread_handle_RoAppAuDa = 0;
	int ret = VAThreadCreate(&thread_handle_RoAppAuDa, (VAThread_func_t)&static_threadRollAppendAudioData, this, "RoAppAuDa");

	while (true) {
		switch (mvw_real6())
		{
		case 0: {
			int scene = mvw_real20();
			DEBUG_TRACE("call ISSMVWStart, scene: %d",err);
			err = ISSMVWStart(phISSMVW, scene);
			break;
		}
		case 1: {
			int scene = mvw_real20();
			DEBUG_TRACE("call ISSMVWAddStartScene, scene: %d",err);
			err = ISSMVWAddStartScene(phISSMVW, scene);
			break;
		}
		case 2:
			if (mvw_real20() < 7) {
				DEBUG_TRACE("call ISSMVWStop");
				err = ISSMVWStop(phISSMVW);
				break;
			}
		case 3: {
			if (mvw_real20() < 7) {
				int scene = mvw_real20();
				DEBUG_TRACE("call ISSMVWStopScene, scene: %d",err);
				err = ISSMVWStopScene(phISSMVW, scene);
				break;
			}
		}
		case 4: {
			int scene = mvw_real20();
			string mvwKeyWord = getMvwKeyWord(mvw_real3());
			DEBUG_TRACE("call ISSMVWSetKeyWords");
			err = ISSMVWSetKeyWords(phISSMVW, scene, mvwKeyWord.c_str());
			break;
		}
		case 5: {
			string param = getParam(mvw_real4());
			string paramValue = getParamValue(mvw_real11());
			DEBUG_TRACE(("call ISSMVWSetParam, param: " + param + ", paramValue: " + paramValue).c_str());
			err = ISSMVWSetParam(phISSMVW, param.c_str(), paramValue.c_str());
			break;
		}
		case 6: {
			int scene = mvw_real20();
			DEBUG_TRACE("call ISSMVWSetDefaultKeyWords");
			err = ISSMVWSetDefaultKeyWords(phISSMVW, scene);
		}
		}
	}
}

static VAThread_result_t static_threadRollAppendAudioData(void *args)
{
	TestMVWApiRandom* pThis = (TestMVWApiRandom*)args;
	pThis->threadRollAppendAudioData();
	return VAThread_result_t(0);
}
void TestMVWApiRandom::threadRollAppendAudioData() {
	while (true) {
		DEBUG_TRACE("call ISSMVWAppendAudioData");
		appendAudio(phISSMVW, "mvw", def.mvwPcm_global, def);

		sleep(3000);
	}
}

string TestMVWApiRandom::getParam(int item) {
	switch (item) {
	case 0:
		return ISS_MVW_PARAM_AEC;
	case 1:
		return ISS_MVW_PARAM_LSA;
	case 2:
		return ISS_MVW_PARAM_TMP_LOG_DIR;
	case 3:
		return ISS_MVW_PARAM_THRESHOLD_LEVEL;
	default:
		return "";
	}
}

string TestMVWApiRandom::getParamValue(int item) {
	switch (item) {
	case 0:
		return ISS_MVW_PARAM_VALUE_ON;
	case 1:
		return ISS_MVW_PARAM_VALUE_OFF;
	case 2:
		return "asdfaw";
	case 3:
		return "△╲Ⅸ❽‰にオУ┿夊Óū";
	case 4:
		return "";
	case 5:
		return "=.=";
	case 6:
		return "-3";
	case 7:
		return "0";
	case 8:
		return "3";
	case 9:
		return "4";
	case 10:
		return "-4";
	case 11:
		return "2.5";
	default:
		return "";
	}
}

string TestMVWApiRandom::getMvwKeyWord(int item) {
	switch (item) {
	case 0:
		return def.mvwKeyWord_KaiYiNiHao;
	case 1:
		return def.mvwKeyWord_KaiYi_LingXi;
	case 2:
		return def.mvwKeyWord_123;
	default:
		return "";
	}
}
