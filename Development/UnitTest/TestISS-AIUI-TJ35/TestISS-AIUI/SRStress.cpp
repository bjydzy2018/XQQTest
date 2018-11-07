#include "TestApiSR.h"
#include "CommonFun.h"
#include "TimerForTest.h"

#include "rapidjson/document.h"

#include <string>

using std::string;
using namespace rapidjson;

// 压力用例
class SRStressTestGenLang : public ::testing::TestWithParam<int> {
protected:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;

	virtual void SetUp() {
		err = -1;
		phISSSR = NULL;
	}

	virtual void TearDown() {
		phISSSR = NULL;
		DefSR::initMsg();
	}
};

// 压力用例
class SRStressTest : public ::testing::Test {
protected:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;

	virtual void SetUp() {
		err = -1;
		phISSSR = NULL;
	}

	virtual void TearDown() {
		phISSSR = NULL;
		DefSR::initMsg();
	}
};

#ifdef ISS_SR_EN
INSTANTIATE_TEST_CASE_P(GenLang, SRStressTestGenLang, ::testing::Values(0, 1));
#else
INSTANTIATE_TEST_CASE_P(GenLang, SRStressTestGenLang, ::testing::Values(0));
#endif

// SR_API_STRESS_0001
// (create、destroy) * 100
TEST_P(SRStressTestGenLang, CreateDestroy100Times) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	for (int i = 0; i < 100; i++)
	{
#ifdef ISS_SR_3
		err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#else
		err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#endif
		EXPECT_EQ(ISS_SUCCESS, err);
		
		int count = 0;
		while (!def.msgInitStatus&&++count < 3000) {
			sleep(10);
		}

		EXPECT_EQ(1, def.msgInitStatus);

		err = ISSSRDestroy(phISSSR);
		EXPECT_EQ(ISS_SUCCESS, err);

		DefSR::initMsg();
	}
}

// SR_API_STRESS_0002
// (start、stop) * 1000
TEST_P(SRStressTestGenLang, StartStop1000Times) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	for (int i = 0; i < 1000; i++)
	{
		err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSSRStop(phISSSR);
		EXPECT_EQ(ISS_SUCCESS, err);
	}

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SR_API_STRESS_0003
// (start) * 1000
TEST_P(SRStressTestGenLang, Start1000Times) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	for (int i = 0; i < 1000; i++)
	{
		err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
		EXPECT_EQ(ISS_SUCCESS, err);
	}

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SR_API_STRESS_0004
// (stop) * 1000
TEST_P(SRStressTestGenLang, Stop1000Times) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSRStop(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	for (int i = 0; i < 1000; i++)
	{
		err = ISSSRStop(phISSSR);
		EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);
	}

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SR_API_STRESS_0005
// ISSSRUpLoadDict * 1000
TEST_P(SRStressTestGenLang, UploadDict100Times) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	for (int i = 0; i < 100; i++)
	{
		err = ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
		EXPECT_EQ(ISS_SUCCESS, err);
	}

	sleep(20000);

	EXPECT_EQ(100, def.msgUpLoadDictToLocalStatus);
	EXPECT_EQ(100, def.msgUpLoadDictToCloudStatus);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SR_API_STRESS_0006
// (EndAudioData) * 1000
TEST_P(SRStressTestGenLang, EndAudioData1000Times) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	char buffer[320];
	err = ISSSRAppendAudioData(phISSSR, (short *)buffer, 160, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	for (int i = 0; i < 1000; i++)
	{
		err = ISSSREndAudioData(phISSSR);
		//EXPECT_EQ(ISS_SUCCESS, err);
	}

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

//create未成功时接口乱调
TEST_P(SRStressTestGenLang, JoggleBeforeCreateSuccess) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	TestSRApiRandom t;

	//create未成功时调用各种接口
	while( !def.msgInitStatus) {
		t.SRJoggle();
	}

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

//create成功后接口乱调
TEST_P(SRStressTestGenLang, JoggleAfterCreateSuccess) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	TestSRApiRandom t;

	//create成功后调用各种接口
	for( int i = 0; i < 300; i++) {
		t.SRJoggle();
	}

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}