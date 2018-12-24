#include "TestApiSR.h"
#include "CommonFun.h"

#include "rapidjson/document.h"

#include <string>

#include <boost/timer.hpp>
using namespace VA;
using boost::timer;
using std::string;
using namespace rapidjson;

// 冒烟用例
class SRSmokeTest : public ::testing::Test {
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

// SR_API_SMOKE_1000
// ISSSRCreate前不调用ISSSetMachineCode
TEST_F(SRSmokeTest, CreateDestroyNoMachineCode) {
	DEBUG_TRACE("Case SRSmokeTest-CreateDestroyNoMachineCode Start");
	DEBUG_TRACE("Before ISSSRCreate");
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	DEBUG_TRACE("After ISSSRCreate");
	EXPECT_EQ(ISS_ERROR_MACHINE_CODE_NOT_SET, err);
	DEBUG_TRACE("err:%d", err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRSmokeTest-CreateDestroyNoMachineCode End");
}

// SR_API_SMOKE_1001
// 中文语种正常create、destroy
TEST_F(SRSmokeTest, CreateExCHDestroy) {
	DEBUG_TRACE("Case SRSmokeTest-CreateExCHDestroy Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
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
	DEBUG_TRACE("Case SRSmokeTest-CreateExCHDestroy End");
}

#ifdef ISS_SR_EN
// SR_API_SMOKE_1002
// 英文语种正常create、destroy
TEST_F(SRSmokeTest, CreateExENDestroy) {
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	
	EXPECT_EQ(1, def.msgInitStatus);

	DEBUG_TRACE("before ISSSRDestroy");
	err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("after ISSSRDestroy");
}
#endif

// SR_API_SMOKE_4001
// stop后送音频
TEST_F(SRSmokeTest, AppendAudioDataAfterStop) {
	DEBUG_TRACE("Case SRSmokeTest-AppendAudioDataAfterStop Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	//if (!def.msgInitStatus)
	//	return;
	isslog_error_return_no(def.msgInitStatus, ("def.msgInitStatus is false"));

	EXPECT_EQ(1, def.msgInitStatus);

	if (err == ISS_SUCCESS) {
		err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
		EXPECT_EQ(ISS_SUCCESS, err);

		if (err == ISS_SUCCESS) {
			err = ISSSRAdapter::ISSSRStop(phISSSR);
			EXPECT_EQ(ISS_SUCCESS, err);

			if (err == ISS_SUCCESS) {
				unsigned int pNumberOfWritten = 0;
				char buffer[320];
				err = ISSSRAdapter::ISSSRAppendAudioData(phISSSR, (short *)buffer, 160, &pNumberOfWritten);
				EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);
			}
		}
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRSmokeTest-AppendAudioDataAfterStop End");
}

// SR_API_SMOKE_6001
// 识别一条中文音频
TEST_F(SRSmokeTest, AppendAudioDataCHAudio) {
	DEBUG_TRACE("Case SRSmokeTest-AppendAudioDataCHAudio Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	
	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_NavigateIflytek, def);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		string SR_result;
		Document doc;
		doc.Parse(def.sr_result.c_str());
		if (doc.HasMember("intent")) {
			Value & intent = doc["intent"];

			if (intent.HasMember("text")) {
				Value & text = intent["text"];
				SR_result = string(text.GetString());
			}
		}
		EXPECT_STREQ("导航到科大讯飞", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRSmokeTest-AppendAudioDataCHAudio End");
}

#ifdef ISS_SR_EN
// SR_API_SMOKE_6002
// 识别一条英文音频
TEST_F(SRSmokeTest, AppendAudioDataENAudio) {
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	
	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_AnyHotel, def);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		Document doc;
		doc.Parse(def.sr_result.c_str());
		string SR_result = doc["text"].GetString();
		EXPECT_STREQ("any hotel along ", SR_result.c_str());
	}

	DEBUG_TRACE("before ISSSRDestroy");
	err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("after ISSSRDestroy");
}
#endif

// SR_API_SMOKE_6003
// 持续送大音频
TEST_F(SRSmokeTest, AppendSuperBigAudioData) {
	DEBUG_TRACE("Case SRSmokeTest-AppendSuperBigAudioData Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	
	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	unsigned int pNumberOfWritten = 0;
	char buffer[140000];
	for (int i = 0; i < 1000; i++)
	{
		err = ISSSRAdapter::ISSSRAppendAudioData(phISSSR, (short *)buffer, 70000, &pNumberOfWritten);
		EXPECT_EQ(ISS_ERROR_NO_ENOUGH_BUFFER, err);
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRSmokeTest-AppendSuperBigAudioData End");
}

#ifdef ISS_SR_EN
// SR_API_SMOKE_6004
// 中英文切换两次
TEST_F(SRSmokeTest, AppendAudioDataCHSwitchEN) {
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	
	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_NavigateIflytek, def);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		Document doc;
		doc.Parse(def.sr_result.c_str());
		string SR_result = doc["text"].GetString();
		EXPECT_STREQ("导航到科大讯飞", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	DEBUG_TRACE("before ISSSRDestroy");
	err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	DEBUG_TRACE("after ISSSRDestroy");
	phISSSR = NULL;
	def.initMsg();
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	
	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_AnyHotel, def);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		Document doc;
		doc.Parse(def.sr_result.c_str());
		string SR_result = doc["text"].GetString();
		EXPECT_STREQ("any hotel along ", SR_result.c_str());
	}

	DEBUG_TRACE("before ISSSRDestroy");
	err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	DEBUG_TRACE("after ISSSRDestroy");
	phISSSR = NULL;
	def.initMsg();
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	
	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_NavigateIflytek, def);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		Document doc;
		doc.Parse(def.sr_result.c_str());
		string SR_result = doc["text"].GetString();
		EXPECT_STREQ("导航到科大讯飞", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	DEBUG_TRACE("before ISSSRDestroy");
	err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	DEBUG_TRACE("after ISSSRDestroy");
	phISSSR = NULL;
	def.initMsg();
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	
	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_AnyHotel, def);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		Document doc;
		doc.Parse(def.sr_result.c_str());
		string SR_result = doc["text"].GetString();
		EXPECT_STREQ("any hotel along ", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	DEBUG_TRACE("before ISSSRDestroy");
	err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	DEBUG_TRACE("after ISSSRDestroy");
	phISSSR = NULL;
	def.initMsg();
	EXPECT_EQ(ISS_SUCCESS, err);
}
#endif

// SR_API_SMOKE_7001
// 上传联系人词典，并进行识别
TEST_F(SRSmokeTest, UploadCHContactDict) {
	DEBUG_TRACE("Case SRSmokeTest-UploadCHContactDict Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}
	
	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
	EXPECT_EQ(ISS_SUCCESS, err);

	while (!def.msgUpLoadDictToLocalStatus )
		sleep(10);

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_CallBaiYaWei, def);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		string SR_result;
		Document doc;
		doc.Parse(def.sr_result.c_str());
		if (doc.HasMember("intent")) {
			Value & intent = doc["intent"];

			if (intent.HasMember("text")) {
				Value & text = intent["text"];
				SR_result = string(text.GetString());
			}
		}
		EXPECT_STREQ("打电话给白亚纬", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRSmokeTest-UploadCHContactDict End");
}


// SR_API_SMOKE_8001
// 窄波束识别
TEST_F(SRSmokeTest, SeoptSR) {
	DEBUG_TRACE("Case SRSmokeTest-SeoptSR Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, 2,DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudioToAIUISR(phISSSR, def.srPcm_SeoptSR, def, 1024, 0);

	EXPECT_TRUE(def.msgResult);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRSmokeTest-SeoptSR End");

}


// SR_API_SMOKE_9001
// 后处理结果
TEST_F(SRSmokeTest, SRHouChuLi) {
	DEBUG_TRACE("Case SRSmokeTest-SRHouChuLi Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_CallBaiYaWei, def);
	sleep(3000);
	EXPECT_TRUE(def.msgTPPResult);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRSmokeTest-SRHouChuLi End");
}


// SR_API_SMOKE_10001
// PGS实时转写
TEST_F(SRSmokeTest, SRPGS) {
	DEBUG_TRACE("Case SRSmokeTest-SRPGS Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_CallBaiYaWei, def);

	sleep(3000);

	EXPECT_TRUE(def.msgSRResult);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRSmokeTest-SRPGS End");
}


// SR_API_SMOKE
// 抛弃当前尚未抛出的识别结果
TEST_F(SRSmokeTest, ClearResult) {
	DEBUG_TRACE("Case SRSmokeTest-ClearResult Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("call ISSSRClearResult before");
	ISSSRAdapter::ISSSRClearResult(phISSSR);
	DEBUG_TRACE("call ISSSRClearResult after");
	
	appendAudioToAIUISR(phISSSR, def.srPcm_CallBaiYaWei, def, 320, 0);

	sleep(3000);

	EXPECT_FALSE(def.msgResult);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRSmokeTest-ClearResult End");
}

