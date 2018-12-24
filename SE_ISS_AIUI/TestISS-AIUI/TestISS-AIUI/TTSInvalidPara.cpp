

#include "TestApiTTS.h"
#include "CommonFun.h"

#include <boost/timer.hpp>

using boost::timer;
using namespace VA;
#ifdef __ANDROID__
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
	DEBUG_TRACE("Case TTSInvalidPara-InitResLoadTypeN1 Start");
	def.setResLoadType("all:-1");

	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_ERROR_TTS_RESOURCE_COUNT, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);

	def.setResLoadType("all:2");
	DEBUG_TRACE("Case TTSInvalidPara-InitResLoadTypeN1 End");
}

// TTS_API_INVALID_PARA_1001
// isstts.cfg中loadtype设置为3
TEST_F(TTSInvalidPara, InitResLoadType3IResMode1) {
	DEBUG_TRACE("Case TTSInvalidPara-InitResLoadType3IResMode1 Start");

	def.setResLoadType("all:3");

	timer t;
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	double init_time = t.elapsed() * 1000;

	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_TRUE(init_time >= 100);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);

	def.setResLoadType("all:2");
	DEBUG_TRACE("Case TTSInvalidPara-InitResLoadType3IResMode1 End");
}

// TTS_API_INVALID_PARA_1002
// 句柄传空
TEST_F(TTSInvalidPara, InitResHandleNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-InitResHandleNULL Start");
	err = ISSTTSAdapter::ISSTTSInitRes(NULL, def.resDir, 1);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-InitResHandleNULL End");
}

// TTS_API_INVALID_PARA_1003
// 资源路径传空
TEST_F(TTSInvalidPara, InitResResDirNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-InitResResDirNULL Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, NULL, 1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-InitResResDirNULL End");
}

// TTS_API_INVALID_PARA_1004
// 资源路径下没有资源
TEST_F(TTSInvalidPara, InitResResDirNoRes) {
	DEBUG_TRACE("Case TTSInvalidPara-InitResResDirNoRes Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, ".", 1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-InitResResDirNoRes End");
}

// TTS_API_INVALID_PARA_1005
// 资源路径为无效字符串（"///、/"）
TEST_F(TTSInvalidPara, InitResResDirInvalid) {
	DEBUG_TRACE("Case TTSInvalidPara-InitResResDirInvalid Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, "/*&^%\\", 1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-InitResResDirInvalid End");
}

// TTS_API_INVALID_PARA_1006
// 资源路径为无效字符串（""）
TEST_F(TTSInvalidPara, InitResResDirNULLStr) {
	DEBUG_TRACE("Case TTSInvalidPara-InitResResDirNULLStr Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, "", 1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-InitResResDirNULLStr End");
}

// TTS_API_INVALID_PARA_1007
// 加载方式错误
TEST_F(TTSInvalidPara, InitResResMode) {
	DEBUG_TRACE("Case TTSInvalidPara-InitResResMode Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, -1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-InitResResMode End");
}

// TTS_API_INVALID_PARA_3000
// 合成实例句柄传NULL
TEST_F(TTSInvalidPara, CreateHandleNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-CreateHandleNULL Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(NULL, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-CreateHandleNULL End");
}

// TTS_API_INVALID_PARA_3001
// 合成资源实例句柄传NULL
TEST_F(TTSInvalidPara, CreateResHandleNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-CreateResHandleNULL Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, NULL, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-CreateResHandleNULL End");
}

// TTS_API_INVALID_PARA_3002
// 回调句柄传NULL
TEST_F(TTSInvalidPara, CreateProcNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-CreateProcNULL Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, NULL, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-CreateProcNULL Start");
}

// TTS_API_INVALID_PARA_4000
// SetParam句柄传空
TEST_F(TTSInvalidPara, SetParamHandleNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-SetParamHandleNULL Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParam(NULL, ISS_TTS_PARAM_SPEAKER, ivTTS_ROLE_XIAOYAN);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-SetParamHandleNULL End");
}

// TTS_API_INVALID_PARA_4001
// 参数id不存在
TEST_F(TTSInvalidPara, SetParamIDNotExist) {
	DEBUG_TRACE("Case TTSInvalidPara-SetParamIDNotExist Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, 10000, ivTTS_ROLE_XIAOYAN);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-SetParamIDNotExist End");
}

// TTS_API_INVALID_PARA_4002
// 异常的发音人参数值
TEST_F(TTSInvalidPara, SetParamValueNotExist) {
	DEBUG_TRACE("Case TTSInvalidPara-SetParamValueNotExist Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_SPEAKER, 1000000);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-SetParamValueNotExist End");
}

// TTS_API_INVALID_PARA_4003-4004
// 发音语速参数值取最小值-1、最大值+1
TEST_P(TTSInvalidParaGenInvalidParamValue, SetParamSpeedInvalid) {
	DEBUG_TRACE("Case TTSInvalidParaGenInvalidParamValue-SetParamSpeedInvalid Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_SPEAKER, GetParam());
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidParaGenInvalidParamValue-SetParamSpeedInvalid End");
}

// TTS_API_INVALID_PARA_4005-4006
// 音量参数值取最小值-1、最大值+1
TEST_P(TTSInvalidParaGenInvalidParamValue, SetParamVolumeInvalid) {
	DEBUG_TRACE("Case TTSInvalidParaGenInvalidParamValue-SetParamVolumeInvalid Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOLUME, GetParam());
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidParaGenInvalidParamValue-SetParamVolumeInvalid End");
}

// TTS_API_INVALID_PARA_4007-4008
// 音调参数值取最小值-1、最大值+1
TEST_P(TTSInvalidParaGenInvalidParamValue, SetParamPitchInvalid) {
	DEBUG_TRACE("Case TTSInvalidParaGenInvalidParamValue-SetParamPitchInvalid Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOICE_PITCH, GetParam());
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidParaGenInvalidParamValue-SetParamPitchInvalid End");
}

// TTS_API_INVALID_PARA_4009
// 使用模式传不存在的正值
TEST_F(TTSInvalidPara, SetParamModePositiveInvalid) {
	DEBUG_TRACE("Case TTSInvalidPara-SetParamModePositiveInvalid Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_USERMODE, 5);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-SetParamModePositiveInvalid End");
}

// TTS_API_INVALID_PARA_4010
// 使用模式传不存在的负值
TEST_F(TTSInvalidPara, SetParamModeNegativeInvalid) {
	DEBUG_TRACE("Case TTSInvalidPara-SetParamModeNegativeInvalid Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_USERMODE, -1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-SetParamModeNegativeInvalid End");
}

// TTS_API_INVALID_PARA_5000
// SetParamEx句柄传NULL
TEST_F(TTSInvalidPara, SetParamExHandleNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-SetParamExHandleNULL Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParamEx(NULL, ISS_TTS_PARAM_SPEAKER, "jiajia");
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-SetParamExHandleNULL End");
}

// TTS_API_INVALID_PARA_5001
// 参数id不存在
TEST_F(TTSInvalidPara, SetParamExIDNotExist) {
	DEBUG_TRACE("Case TTSInvalidPara-SetParamExIDNotExist Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParamEx(phISSTTS, 1000, "jiajia");
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-SetParamExIDNotExist End");
}

// TTS_API_INVALID_PARA_5002
// 发音人参数值不存在
TEST_F(TTSInvalidPara, SetParamExValueNotExist) {
	DEBUG_TRACE("Case TTSInvalidPara-SetParamExValueNotExist Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParamEx(phISSTTS, ISS_TTS_PARAM_SPEAKER, "xiaola");
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-SetParamExValueNotExist End");
}

// TTS_API_INVALID_PARA_5003
// 发音人参数值为大写
TEST_F(TTSInvalidPara, SetParamExSpeakerUppercase) {
	DEBUG_TRACE("Case TTSInvalidPara-SetParamExSpeakerUppercase Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParamEx(phISSTTS, ISS_TTS_PARAM_SPEAKER, "JIAJIA");
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-SetParamExSpeakerUppercase End");
}

// TTS_API_INVALID_PARA_5004
// 日志路径不存在
TEST_F(TTSInvalidPara, SetParamExLogDirNotExist) {
	DEBUG_TRACE("Case TTSInvalidPara-SetParamExLogDirNotExist Start");
#if VA_ANDROID
	const char * log_dir = "/sdcard/tmp_tts_log";
#else
	const char * log_dir = "tmp_tts_log";
#endif
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParamEx(phISSTTS, ISS_TTS_PARAM_TMP_LOG_DIR, log_dir);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-SetParamExLogDirNotExist End");
}

// TTS_API_INVALID_PARA_6000
// start句柄传NULL
TEST_F(TTSInvalidPara, StartHandleNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-StartHandleNULL Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSAdapter::ISSTTSStart(NULL, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-StartHandleNULL End");
}

// TTS_API_INVALID_PARA_6001
// start文本传NULL
TEST_F(TTSInvalidPara, StartTextNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-StartTextNULL Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, NULL, 0, ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-StartTextNULL End");
}

// TTS_API_INVALID_PARA_6002
// start长度传负值
TEST_F(TTSInvalidPara, StartSizeNegative) {
	DEBUG_TRACE("Case TTSInvalidPara-StartSizeNegative Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, -1, ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-StartSizeNegative End");
}

// TTS_API_INVALID_PARA_6003
// 异常的编码格式
TEST_F(TTSInvalidPara, StartCodepageInvalid) {
	DEBUG_TRACE("Case TTSInvalidPara-StartCodepageInvalid Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE(500));
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-StartCodepageInvalid End");
}

// TTS_API_INVALID_PARA_7000
// GetAudioData句柄传NULL
TEST_F(TTSInvalidPara, GetAudioDataHandleNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-GetAudioDataHandleNULL Start");
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

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	err = ISSTTSAdapter::ISSTTSGetAudioData(NULL, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-GetAudioDataHandleNULL End");
}

// TTS_API_INVALID_PARA_7001
// GetAudioData buffer传NULL
TEST_F(TTSInvalidPara, GetAudioDataBufferNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-GetAudioDataBufferNULL Start");
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

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, NULL, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-GetAudioDataBufferNULL End");
}

// TTS_API_INVALID_PARA_7002
// GetAudioData piBufferFramesGot传NULL
TEST_F(TTSInvalidPara, GetAudioDataBufferFramesGotNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-GetAudioDataBufferFramesGotNULL Start");
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

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, buffer, 800, NULL, &piProgBegin, &piProgLen);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-GetAudioDataBufferFramesGotNULL End");

}

// TTS_API_INVALID_PARA_7003
// GetAudioData piProgBegin传NULL
TEST_F(TTSInvalidPara, GetAudioDataProgBeginNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-GetAudioDataProgBeginNULL Start");
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

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, NULL, &piProgLen);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-GetAudioDataProgBeginNULL End");
}

// TTS_API_INVALID_PARA_7004
// GetAudioData piProgLen传NULL
TEST_F(TTSInvalidPara, GetAudioDataProgLenNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-GetAudioDataProgLenNULL Start");
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

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-GetAudioDataProgLenNULL End");
}

// TTS_API_INVALID_PARA_8000
// stop句柄传NULL
TEST_F(TTSInvalidPara, StopHandleNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-StopHandleNULL Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSStop(NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-StopHandleNULL End");
}

// TTS_API_INVALID_PARA_9000
// destroy句柄传NULL
TEST_F(TTSInvalidPara, DestroyHandleNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-DestroyHandleNULL Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSDestroy(NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-DestroyHandleNULL End");
}

// TTS_API_INVALID_PARA_1000
// uninit 句柄传NULL
TEST_F(TTSInvalidPara, UninitHandleNULL) {
	DEBUG_TRACE("Case TTSInvalidPara-UninitHandleNULL Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSInvalidPara-UninitHandleNULL End");
}
#endif