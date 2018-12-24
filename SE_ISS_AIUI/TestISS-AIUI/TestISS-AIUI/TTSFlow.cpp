
#include "TestApiTTS.h"
#include "CommonFun.h"
using namespace VA;
#ifdef __ANDROID__
class TTSFlow : public ::testing::Test {
protected:
	ISSErrID err;
	HISSTTSRES phISSTTSRES;
	HISSTTS	 phISSTTS;

	DefTTS def;

	virtual void SetUp() {
		err = -1;
		phISSTTSRES = NULL;
		phISSTTS = NULL;
	}

	virtual void TearDown() {
		phISSTTS = NULL;
		phISSTTSRES = NULL;

		DefTTS::initMsg();
	}
};

// TTS_API_FLOW_6001
// start前stop
TEST_F(TTSFlow, StopBeforeStart) {

	DEBUG_TRACE("Case TTSFlow-StopBeforeStart Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSStop(phISSTTS);
	EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSFlow-StopBeforeStart End");
}

// TTS_API_FLOW_6002
// start后收到Proc_OnTTSDataReady前stop
TEST_F(TTSFlow, StopBeforeDataReady) {
	DEBUG_TRACE("Case TTSFlow-StopBeforeDataReady Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_EQ(0, def.msgOnDataReady);

	err = ISSTTSAdapter::ISSTTSStop(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	sleep(2000);
	EXPECT_EQ(0, def.msgOnDataReady);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSFlow-StopBeforeDataReady End");
}

// TTS_API_FLOW_6003
// start后收到Proc_OnTTSDataReady前destroy
TEST_F(TTSFlow, DestroyBeforeDataReady) {
	DEBUG_TRACE("Case TTSFlow-DestroyBeforeDataReady Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_EQ(0, def.msgOnDataReady);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSFlow-DestroyBeforeDataReady End");
}

// TTS_API_FLOW_6004
// start后收到Proc_OnTTSDataReady前调用getAudioData
TEST_F(TTSFlow, GetAudioDataBeforeDataReady) {
	DEBUG_TRACE("Case TTSFlow-GetAudioDataBeforeDataReady Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_EQ(0, def.msgOnDataReady);

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
	EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);

	int j = 0;
	while (!def.msgOnDataReady && j++ < 200)
		sleep(10);
	EXPECT_TRUE(j <= 200);

	err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSFlow-GetAudioDataBeforeDataReady End");
}

// TTS_API_FLOW_7001
// 直接调用getAudioData
TEST_F(TTSFlow, GetAudioDataBeforeStart) {
	DEBUG_TRACE("Case TTSFlow-GetAudioDataBeforeStart Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
	EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSFlow-GetAudioDataBeforeStart End");
}

// TTS_API_FLOW_8001
// stop后调用getAudioData
TEST_F(TTSFlow, GetAudioDataAfterStop) {
	DEBUG_TRACE("Case TTSFlow-GetAudioDataAfterStop Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_EQ(0, def.msgOnDataReady);

	int j = 0;
	while (!def.msgOnDataReady && j++ < 200)
		sleep(10);
	EXPECT_TRUE(j <= 200);

	err = ISSTTSAdapter::ISSTTSStop(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
	EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSFlow-GetAudioDataAfterStop End");
}
#endif