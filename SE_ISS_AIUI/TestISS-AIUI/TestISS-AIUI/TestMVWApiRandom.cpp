#include "TestMVWApiRandom.h"
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
using boost::bind;
using boost::timer;

TestMVWApiRandom::TestMVWApiRandom() :err(-1), phISSMVW(NULL) {}

void TestMVWApiRandom::test() {
	boost::random::mt19937 gen;
	boost::uniform_int<> real6(0, 6);
	boost::uniform_int<> real3(0, 3);
	boost::uniform_int<> real4(0, 4);
	boost::uniform_int<> real11(0, 11);
	boost::uniform_int<> real20(0, 20);

	err = ISSMVWAdapter::ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL);
	DEBUG_TRACE(("ISSMVWCreate return " + lexical_cast<string>(err)).c_str());
	if (err != ISS_SUCCESS)
		return;

	thread threadRollAppendAudioData(bind(&TestMVWApiRandom::threadRollAppendAudioData, this));

	timer t;
	double sum_time = 0;
	double expect_time = run_time_ * 60 * 60 * 1000;
	while (sum_time < expect_time) {
		t.restart();
		switch (real6(gen))
		{
		case 0: {
			int scene = real20(gen);
			DEBUG_TRACE(("call ISSMVWStart, scene: " + lexical_cast<string>(scene)).c_str());
			err = ISSMVWAdapter::ISSMVWStart(phISSMVW, scene);
			break;
		}
		case 1: {
			int scene = real20(gen);
			DEBUG_TRACE(("call ISSMVWAddStartScene, scene: " + lexical_cast<string>(scene)).c_str());
			err = ISSMVWAdapter::ISSMVWAddStartScene(phISSMVW, scene);
			break;
		}
		case 2:
			if (real20(gen) < 7) {
				DEBUG_TRACE("call ISSMVWStop");
				err = ISSMVWAdapter::ISSMVWStop(phISSMVW);
				break;
			}
		case 3: {
			if (real20(gen) < 7) {
				int scene = real20(gen);
				DEBUG_TRACE(("call ISSMVWStopScene, scene: " + lexical_cast<string>(scene)).c_str());
				err = ISSMVWAdapter::ISSMVWStopScene(phISSMVW, scene);
				break;
			}
		}
		case 4: {
			int scene = real20(gen);
			string mvwKeyWord = getMvwKeyWord(real3(gen));
			DEBUG_TRACE("call ISSMVWSetKeyWords");
			err = ISSMVWAdapter::ISSMVWSetKeyWords(phISSMVW, scene, mvwKeyWord.c_str());
			break;
		}
		case 5: {
			string param = getParam(real4(gen));
			string paramValue = getParamValue(real11(gen));
			DEBUG_TRACE(("call ISSMVWSetParam, param: " + param + ", paramValue: " + paramValue).c_str());
			err = ISSMVWAdapter::ISSMVWSetParam(phISSMVW, param.c_str(), paramValue.c_str());
			break;
		}
		case 6: {
			int scene = real20(gen);
			DEBUG_TRACE("call ISSMVWSetDefaultKeyWords");
			err = ISSMVWAdapter::ISSMVWSetDefaultKeyWords(phISSMVW, scene);
		}
		}
		double each_time = t.elapsed() * 1000;
		sum_time += each_time;
	}
	ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	DEBUG_TRACE(("ISSMVWDestroy return " + lexical_cast<string>(err)).c_str());
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
