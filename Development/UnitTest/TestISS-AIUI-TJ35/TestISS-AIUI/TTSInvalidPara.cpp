#include "TestApiTTS.h"
#include "CommonFun.h"
#include "TimerForTest.h"

class TTSInvalidPara : public ::testing::Test {
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

class TTSInvalidParaGenResMode : public ::testing::TestWithParam<int> {
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
INSTANTIATE_TEST_CASE_P(LoadResMode, TTSInvalidParaGenResMode, ::testing::Values(1));

class TTSInvalidParaGenInvalidParamValue : public ::testing::TestWithParam<int> {
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
INSTANTIATE_TEST_CASE_P(InvalidParamValue, TTSInvalidParaGenInvalidParamValue, ::testing::Values(-32769, 32768));

// TTS_API_INVALID_PARA_1000
// isstts.cfg中loadtype设置为-1
TEST_F(TTSInvalidPara, InitResLoadTypeN1) {
	def.setResLoadType("all:-1");

	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_ERROR_TTS_RESOURCE_COUNT, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);

	def.setResLoadType("all:2");
}

// TTS_API_INVALID_PARA_1001
// isstts.cfg中loadtype设置为3
TEST_F(TTSInvalidPara, InitResLoadType3IResMode1) {
	def.setResLoadType("all:3");

	//timer t;
	TimerForTest t;
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	//double init_time = t.elapsed() * 1000;
	double init_time = t.elapsed();

	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_TRUE(init_time >= 100);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);

	def.setResLoadType("all:2");
}

// TTS_API_INVALID_PARA_1002
// 句柄传空
TEST_F(TTSInvalidPara, InitResHandleNULL) {
	err = ISSTTSInitRes(NULL, def.resDir, 1);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_1003
// 资源路径传空
TEST_F(TTSInvalidPara, InitResResDirNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, NULL, 1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_1004
// 资源路径下没有资源
TEST_F(TTSInvalidPara, InitResResDirNoRes) {
	err = ISSTTSInitRes(&phISSTTSRES, ".", 1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_1005
// 资源路径为无效字符串（"///、/"）
TEST_F(TTSInvalidPara, InitResResDirInvalid) {
	err = ISSTTSInitRes(&phISSTTSRES, "/*&^%\\", 1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_1006
// 资源路径为无效字符串（""）
TEST_F(TTSInvalidPara, InitResResDirNULLStr) {
	err = ISSTTSInitRes(&phISSTTSRES, "", 1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_1007
// 加载方式错误
TEST_F(TTSInvalidPara, InitResResMode) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, -1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_3000
// 合成实例句柄传NULL
TEST_F(TTSInvalidPara, CreateHandleNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(NULL, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_3001
// 合成资源实例句柄传NULL
TEST_F(TTSInvalidPara, CreateResHandleNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, NULL, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_3002
// 回调句柄传NULL
TEST_F(TTSInvalidPara, CreateProcNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, NULL, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_4000
// SetParam句柄传空
TEST_F(TTSInvalidPara, SetParamHandleNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParam(NULL, ISS_TTS_PARAM_SPEAKER, ivTTS_ROLE_XIAOYAN);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_4001
// 参数id不存在
TEST_F(TTSInvalidPara, SetParamIDNotExist) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParam(phISSTTS, 10000, ivTTS_ROLE_XIAOYAN);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_4002
// 异常的发音人参数值
TEST_F(TTSInvalidPara, SetParamValueNotExist) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_SPEAKER, 1000000);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_4003-4004
// 发音语速参数值取最小值-1、最大值+1
TEST_P(TTSInvalidParaGenInvalidParamValue, SetParamSpeedInvalid) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_SPEAKER, GetParam());
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_4005-4006
// 音量参数值取最小值-1、最大值+1
TEST_P(TTSInvalidParaGenInvalidParamValue, SetParamVolumeInvalid) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOLUME, GetParam());
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_4007-4008
// 音调参数值取最小值-1、最大值+1
TEST_P(TTSInvalidParaGenInvalidParamValue, SetParamPitchInvalid) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOICE_PITCH, GetParam());
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_4009
// 使用模式传不存在的正值
TEST_F(TTSInvalidPara, SetParamModePositiveInvalid) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_USERMODE, 5);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_4010
// 使用模式传不存在的负值
TEST_F(TTSInvalidPara, SetParamModeNegativeInvalid) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_USERMODE, -1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_5000
// SetParamEx句柄传NULL
TEST_F(TTSInvalidPara, SetParamExHandleNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParamEx(NULL, ISS_TTS_PARAM_SPEAKER, "jiajia");
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_5001
// 参数id不存在
TEST_F(TTSInvalidPara, SetParamExIDNotExist) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParamEx(phISSTTS, 1000, "jiajia");
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_5002
// 发音人参数值不存在
TEST_F(TTSInvalidPara, SetParamExValueNotExist) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParamEx(phISSTTS, ISS_TTS_PARAM_SPEAKER, "xiaola");
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_5003
// 发音人参数值为大写
TEST_F(TTSInvalidPara, SetParamExSpeakerUppercase) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParamEx(phISSTTS, ISS_TTS_PARAM_SPEAKER, "JIAJIA");
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_5004
// 日志路径不存在
TEST_F(TTSInvalidPara, SetParamExLogDirNotExist) {
#if VA_ANDROID
	const char * log_dir = "/sdcard/tmp_tts_log";
#else
	const char * log_dir = "tmp_tts_log";
#endif
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParamEx(phISSTTS, ISS_TTS_PARAM_TMP_LOG_DIR, log_dir);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_6000
// start句柄传NULL
TEST_F(TTSInvalidPara, StartHandleNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSStart(NULL, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_6001
// start文本传NULL
TEST_F(TTSInvalidPara, StartTextNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSStart(phISSTTS, NULL, 0, ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_6002
// start长度传负值
TEST_F(TTSInvalidPara, StartSizeNegative) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSStart(phISSTTS, text, -1, ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_6003
// 异常的编码格式
TEST_F(TTSInvalidPara, StartCodepageInvalid) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE(500));
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_7000
// GetAudioData句柄传NULL
TEST_F(TTSInvalidPara, GetAudioDataHandleNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	int j = 0;
	while (!def.msgOnDataReady && j++ < 200)
		sleep(10);
	EXPECT_TRUE(j <= 200);

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	err = ISSTTSGetAudioData(NULL, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_7001
// GetAudioData buffer传NULL
TEST_F(TTSInvalidPara, GetAudioDataBufferNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	int j = 0;
	while (!def.msgOnDataReady && j++ < 200)
		sleep(10);
	EXPECT_TRUE(j <= 200);

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	err = ISSTTSGetAudioData(phISSTTS, NULL, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_7002
// GetAudioData piBufferFramesGot传NULL
TEST_F(TTSInvalidPara, GetAudioDataBufferFramesGotNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	int j = 0;
	while (!def.msgOnDataReady && j++ < 200)
		sleep(10);
	EXPECT_TRUE(j <= 200);

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	err = ISSTTSGetAudioData(phISSTTS, buffer, 800, NULL, &piProgBegin, &piProgLen);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_7003
// GetAudioData piProgBegin传NULL
TEST_F(TTSInvalidPara, GetAudioDataProgBeginNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	int j = 0;
	while (!def.msgOnDataReady && j++ < 200)
		sleep(10);
	EXPECT_TRUE(j <= 200);

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, NULL, &piProgLen);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_7004
// GetAudioData piProgLen传NULL
TEST_F(TTSInvalidPara, GetAudioDataProgLenNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	int j = 0;
	while (!def.msgOnDataReady && j++ < 200)
		sleep(10);
	EXPECT_TRUE(j <= 200);

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_8000
// stop句柄传NULL
TEST_F(TTSInvalidPara, StopHandleNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSStop(NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_9000
// destroy句柄传NULL
TEST_F(TTSInvalidPara, DestroyHandleNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSDestroy(NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_1000
// uninit 句柄传NULL
TEST_F(TTSInvalidPara, UninitHandleNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}
