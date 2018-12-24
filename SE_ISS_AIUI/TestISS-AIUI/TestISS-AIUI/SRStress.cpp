#include "TestApiSR.h"
#include "CommonFun.h"

#include "rapidjson/document.h"

#include <string>

#include <boost/timer.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
using namespace VA;
using boost::timer;
using std::string;
using namespace rapidjson;

// Ñ¹Á¦ÓÃÀý
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
#ifdef ISS_SR_EN
INSTANTIATE_TEST_CASE_P(GenLang, SRStressTestGenLang, ::testing::Values(0, 1));
#else
INSTANTIATE_TEST_CASE_P(GenLang, SRStressTestGenLang, ::testing::Values(0));
#endif

// SR_API_STRESS_0001
// (create¡¢destroy) * 100
TEST_P(SRStressTestGenLang, CreateDestroy100Times) {
	DEBUG_TRACE("Case SRStressTestGenLang-CreateDestroy100Times Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	for (int i = 0; i < 100; i++)
	{
#ifdef ISS_SR_3
		err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#else
		err = ISSSRAdapter::ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#endif
		EXPECT_EQ(ISS_SUCCESS, err);
		int count = 0;
		while (!def.msgInitStatus&&++count<3000) {
			sleep(10);
		}
		
		EXPECT_EQ(1, def.msgInitStatus);

		{
			isslog_tracer_perf("ISSSRDestroy");
			err = ISSSRAdapter::ISSSRDestroy(phISSSR);
		}
		EXPECT_EQ(ISS_SUCCESS, err);
		def.initMsg();
	}
	DEBUG_TRACE("Case SRStressTestGenLang-CreateDestroy100Times End");
}

// SR_API_STRESS_0002
// (start¡¢stop) * 1000
TEST_P(SRStressTestGenLang, StartStop1000Times) {
	DEBUG_TRACE("Case SRStressTestGenLang-StartStop1000Times Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	
	EXPECT_EQ(1, def.msgInitStatus);

	for (int i = 0; i < 1000; i++)
	{
		err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSSRAdapter::ISSSRStop(phISSSR);
		EXPECT_EQ(ISS_SUCCESS, err);
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRStressTestGenLang-StartStop1000Times End");
}

// SR_API_STRESS_0003
// (start) * 1000
TEST_P(SRStressTestGenLang, Start1000Times) {
	DEBUG_TRACE("Case SRStressTestGenLang-Start1000Times Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	
	EXPECT_EQ(1, def.msgInitStatus);

	for (int i = 0; i < 1000; i++)
	{
		err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
		EXPECT_EQ(ISS_SUCCESS, err);
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRStressTestGenLang-Start1000Times End");
}

// SR_API_STRESS_0004
// (stop) * 1000
TEST_P(SRStressTestGenLang, Stop1000Times) {
	DEBUG_TRACE("Case SRStressTestGenLang-Stop1000Times Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	
	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSRAdapter::ISSSRStop(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	for (int i = 0; i < 1000; i++)
	{
		err = ISSSRAdapter::ISSSRStop(phISSSR);
		EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRStressTestGenLang-Stop1000Times End");
}

// SR_API_STRESS_0005
// ISSSRUpLoadDict * 1000
TEST_P(SRStressTestGenLang, UploadDict100Times) {
	DEBUG_TRACE("Case SRStressTestGenLang-UploadDict100Times Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(GetParam()), def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	
	EXPECT_EQ(1, def.msgInitStatus);

	for (int i = 0; i < 100; i++)
	{
		err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
		EXPECT_EQ(ISS_SUCCESS, err);
	}

	sleep(20000);

	EXPECT_EQ(100, def.msgUpLoadDictToLocalStatus);
	//EXPECT_EQ(100, def.msgUpLoadDictToCloudStatus);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRStressTestGenLang-UploadDict100Times End");
}
