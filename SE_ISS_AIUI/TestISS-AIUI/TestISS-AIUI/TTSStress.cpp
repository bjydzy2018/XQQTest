
#include "TestApiTTS.h"
#include "CommonFun.h"
using namespace VA;
#ifdef __ANDROID__
class TTSStress : public ::testing::Test {
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

// TTS_API_STRESS_0001
// (InitRes, UnInitRes)*1000
TEST_F(TTSStress, InitUnInit1000) {
	DEBUG_TRACE("Case TTSStress-InitUnInit1000 Start");
	for (int i = 0; i < 1000; i++)
	{
		err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	DEBUG_TRACE("Case TTSStress-InitUnInit1000 End");
}

// TTS_API_STRESS_0002
// InitRes, (create, destroy)*1000, UnInitRes
TEST_F(TTSStress, CreateDestroy1000) {
	DEBUG_TRACE("Case TTSStress-CreateDestroy1000 Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	for (int i = 0; i < 1000; i++)
	{
		err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
		EXPECT_EQ(ISS_SUCCESS, err);
	}

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSStress-CreateDestroy1000 End");
}

// TTS_API_STRESS_0003
// InitRes, Create, (start收到DataReady回调前, stop)*1000, destroy, uninit
TEST_F(TTSStress, StartAfterBeforeDataReady1000) {
	DEBUG_TRACE("Case TTSStress-StartAfterBeforeDataReady1000 Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	for (int i = 0; i < 1000; i++)
	{
		err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
		EXPECT_EQ(ISS_SUCCESS, err);
		EXPECT_EQ(0, def.msgOnDataReady);

		err = ISSTTSAdapter::ISSTTSStop(phISSTTS);
		EXPECT_EQ(ISS_SUCCESS, err);

		def.initMsg();
	}

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSStress-StartAfterBeforeDataReady1000 End");
}

// TTS_API_STRESS_0004
// InitRes, Create, (start收到DataReady回调后, stop)*1000, destroy, uninit
TEST_F(TTSStress, StartAfterAfterDataReady1000) {
	DEBUG_TRACE("Case TTSStress-StartAfterAfterDataReady1000 Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	for (int i = 0; i < 1000; i++)
	{
		err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
		EXPECT_EQ(ISS_SUCCESS, err);
		
		int j = 0;
		while (!def.msgOnDataReady && j++ < 200)
			sleep(10);
		EXPECT_TRUE(j <= 200);

		err = ISSTTSAdapter::ISSTTSStop(phISSTTS);
		EXPECT_EQ(ISS_SUCCESS, err);

		def.initMsg();
	}

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSStress-StartAfterAfterDataReady1000 End");
}

// TTS_API_STRESS_0005
// 合成GBK编码文本
TEST_F(TTSStress, All1000) {
	DEBUG_TRACE("Case TTSStress-All1000 Start");
	for (int i = 0; i < 1000; i++)
	{
		err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
		EXPECT_EQ(ISS_SUCCESS, err);

		char * text = "合成接口测试";
		err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
		EXPECT_EQ(ISS_SUCCESS, err);

		int j = 0;
		while (!def.msgOnDataReady && j++ < 200)
			sleep(10);
		EXPECT_TRUE(j <= 200);

		j = 0;
		char buffer[1600] = { 0 };
		unsigned int piBufferFramesGot = 0;
		unsigned int piProgBegin = 0;
		unsigned int piProgLen = 0;
		while (piBufferFramesGot == 0 && j++<10) {
			err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
			EXPECT_EQ(ISS_SUCCESS, err);
		}
		EXPECT_TRUE(j <= 10);

		err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
		EXPECT_EQ(ISS_SUCCESS, err);

		def.initMsg();
	}
	DEBUG_TRACE("Case TTSStress-All1000 End");
}

// TTS_API_STRESS_0006
// start * 1000
TEST_F(TTSStress, Start1000) {
	DEBUG_TRACE("Case TTSStress-Start1000 Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	for (int i = 0; i < 1000; i++)
	{
		err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
		EXPECT_EQ(ISS_SUCCESS, err);

		def.initMsg();
	}

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSStress-Start1000 End");
}

// TTS_API_STRESS_0007
// stop * 1000
TEST_F(TTSStress, Stop1000) {
	DEBUG_TRACE("Case TTSStress-Stop1000 Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSStop(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);
	def.initMsg();
	
	for (int i = 0; i < 1000; i++)
	{
		err = ISSTTSAdapter::ISSTTSStop(phISSTTS);
		EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);
	}

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSStress-Stop1000 End");
}

// TTS_API_STRESS_0008
// setparam * 1000
TEST_F(TTSStress, SetParam1000BeforeStart) {
	DEBUG_TRACE("Case TTSStress-SetParam1000BeforeStart Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	for (int i = 0; i < 1000; i++)
	{
		err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_SPEAKER, ivTTS_ROLE_XIAOJIE);
		EXPECT_EQ(ISS_SUCCESS, err);
	}

	char * text = "合成接口测试";
	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSStop(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSStress-SetParam1000BeforeStart End");
}

// TTS_API_STRESS_0009
// setparam * 1000
TEST_F(TTSStress, SetParam1000AfterStart) {
	DEBUG_TRACE("Case TTSStress-SetParam1000AfterStart Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	for (int i = 0; i < 1000; i++)
	{
		err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_SPEAKER, ivTTS_ROLE_XIAOJIE);
		EXPECT_EQ(ISS_SUCCESS, err);
	}

	err = ISSTTSAdapter::ISSTTSStop(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSStress-SetParam1000AfterStart End");
}
#endif