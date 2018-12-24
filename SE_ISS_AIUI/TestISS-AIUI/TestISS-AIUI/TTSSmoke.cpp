
#include "TestApiTTS.h"
#include "CommonFun.h"

#include <boost/timer.hpp>
#include <boost/filesystem.hpp>

using boost::timer;
using namespace boost::filesystem;
using namespace VA;
#ifdef __ANDROID__
class TTSSmoke : public ::testing::Test {
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

class TTSSmokeGenResMode : public ::testing::TestWithParam<int> {
protected:  
	ISSErrID err;
	HISSTTSRES phISSTTSRES;
	HISSTTS	 phISSTTS;

	DefTTS def;

	virtual void SetUp() {
		err	= -1;
		phISSTTSRES = NULL;
		phISSTTS = NULL;
	}  

	virtual void TearDown() {
		phISSTTS = NULL;
		phISSTTSRES = NULL;

		DefTTS::initMsg();
	}
};
INSTANTIATE_TEST_CASE_P(LoadResMode, TTSSmokeGenResMode, ::testing::Values(1));

class TTSSmokeGenParamValue : public ::testing::TestWithParam<int> {
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
INSTANTIATE_TEST_CASE_P(ParamValue, TTSSmokeGenParamValue, ::testing::Values(-32768, 32767));

// TTS_API_SMOKE_1000
// isstts.cfg中loadtype设置为0，iResMode设为1
TEST_F(TTSSmoke, InitResLoadType0IResMode1) {
	DEBUG_TRACE("Case TTSSmoke-InitResLoadType0IResMode1 Start");
	def.setResLoadType("all:0");

	timer t;
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	double init_time = t.elapsed() * 1000;

	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_TRUE(init_time < 100);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);

	def.setResLoadType("all:2");
	DEBUG_TRACE("Case TTSSmoke-InitResLoadType0IResMode1 End");
}

// TTS_API_SMOKE_1001
// isstts.cfg中loadtype设置为1，iResMode设为0
TEST_F(TTSSmoke, InitResLoadType1IResMode0) {
	DEBUG_TRACE("Case TTSSmoke-InitResLoadType1IResMode0 Start");
	def.setResLoadType("all:1");

	timer t;
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 0);
	double init_time = t.elapsed() * 1000;

	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_TRUE(init_time >= 100);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);

	def.setResLoadType("all:2");
	DEBUG_TRACE("Case TTSSmoke-InitResLoadType1IResMode0 End");
}

// TTS_API_SMOKE_1002
// isstts.cfg中loadtype设置为2，iResMode设为0
TEST_F(TTSSmoke, InitResLoadType2IResMode0) {
	DEBUG_TRACE("Case TTSSmoke-InitResLoadType2IResMode0 Start");
	timer t;
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 0);
	double init_time = t.elapsed() * 1000;

	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_TRUE(init_time < 100);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSSmoke-InitResLoadType2IResMode0 End");
}

// TTS_API_SMOKE_1003
// isstts.cfg中loadtype设置为2，iResMode设为1
TEST_F(TTSSmoke, InitResLoadType2IResMode1) {
	DEBUG_TRACE("Case TTSSmoke-InitResLoadType2IResMode1 Start");
	timer t;
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	double init_time = t.elapsed() * 1000;

	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_TRUE(init_time >= 100);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSSmoke-InitResLoadType2IResMode1 End");
}

#if VA_VC
// TTS_API_SMOKE_2000
// isstts.cfg中loadtype设置为2，iResMode设为0
TEST_F(TTSSmoke, InitResWLoadType2IResMode0) {
	DEBUG_TRACE("Case TTSSmoke-InitResWLoadType2IResMode0 Start");
	timer t;
	err = ISSTTSAdapter::ISSTTSInitResW(&phISSTTSRES, def.resDirW, 0);
	double init_time = t.elapsed() * 1000;

	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_TRUE(init_time < 100);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSSmoke-InitResWLoadType2IResMode0 End");
}
#endif

// TTS_API_SMOKE_3000
// 句柄及回调参数正常
TEST_P(TTSSmokeGenResMode, CreateDestroy) {
	DEBUG_TRACE("Case TTSSmokeGenResMode-CreateDestroy Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSSmokeGenResMode-CreateDestroy End");
}

// TTS_API_SMOKE_4000
// 设置参数，句柄及参数值正常，循环遍历所有参数、参数值组合
TEST_P(TTSSmokeGenResMode, SetParamBeforeStart) {
	DEBUG_TRACE("Case TTSSmokeGenResMode-SetParamBeforeStart Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	int speakers[5] = { 
		60020, // XIAOYAN
		60030, // XIAOFENG
		65040, // XIAOXUE
		65060, // XIAOJIE
		65180  // JIAJIA
	};

	for (int i = 0; i < 5; i++)
	{
		err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_SPEAKER, speakers[i]);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_USERMODE, i);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOICE_SPEED, i * 7235);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOICE_PITCH, i * 7235);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOLUME, i * 7235);
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
		while (piBufferFramesGot==0 && j++<10) {
			err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
			EXPECT_EQ(ISS_SUCCESS, err);
		}
		EXPECT_TRUE(j <= 10);

		def.initMsg();
	}

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSSmokeGenResMode-SetParamBeforeStart End");
}

// TTS_API_SMOKE_4001
// 正在合成过程中设置，循环遍历所有参数、参数值组合
TEST_P(TTSSmokeGenResMode, SetParamAfterStart) {
	DEBUG_TRACE("Case TTSSmokeGenResMode-SetParamAfterStart Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	int speakers[5] = {
		60020, // XIAOYAN
		60030, // XIAOFENG
		65040, // XIAOXUE
		65060, // XIAOJIE
		65180  // JIAJIA
	};
	char * text = "合成接口测试";

	for (int i = 0; i < 5; i++)
	{
		err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
		EXPECT_EQ(ISS_SUCCESS, err);

		int j = 0;
		while (!def.msgOnDataReady && j++ < 200)
			sleep(10);
		EXPECT_TRUE(j <= 200);

		sleep(200);

		err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_SPEAKER, speakers[i]);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_USERMODE, i);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOICE_SPEED, i * 7235);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOICE_PITCH, i * 7235);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOLUME, i * 7235);
		EXPECT_EQ(ISS_SUCCESS, err);

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

		def.initMsg();
	}

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSSmokeGenResMode-SetParamAfterStart End");
}

// TTS_API_SMOKE_4002~4003
// 发音语速参数值取最小值、最大值
TEST_P(TTSSmokeGenParamValue, SetParamVoiceSpeed) {
	DEBUG_TRACE("Case TTSSmokeGenParamValue-SetParamVoiceSpeed Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOICE_SPEED, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	int j = 0;
	while (!def.msgOnDataReady && j++<200)
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
	DEBUG_TRACE("Case TTSSmokeGenParamValue-SetParamVoiceSpeed End");
}

// TTS_API_SMOKE_4004~4005
// 发音音量参数值取最小值、最大值
TEST_P(TTSSmokeGenParamValue, SetParamVoiceVolume) {
	DEBUG_TRACE("Case TTSSmokeGenParamValue-SetParamVoiceVolume Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOLUME, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	int j = 0;
	while (!def.msgOnDataReady && j++<200)
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
	DEBUG_TRACE("Case TTSSmokeGenParamValue-SetParamVoiceVolume End");
}

// TTS_API_SMOKE_4006~4007
// 发音音调参数值取最小值、最大值
TEST_P(TTSSmokeGenParamValue, SetParamVoicePitch) {
	DEBUG_TRACE("Case TTSSmokeGenParamValue-SetParamVoicePitch Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOICE_PITCH, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	int j = 0;
	while (!def.msgOnDataReady && j++<200)
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
	DEBUG_TRACE("Case TTSSmokeGenParamValue-SetParamVoicePitch End");
}

// TTS_API_SMOKE_5000
// 正常设置发音人
TEST_P(TTSSmokeGenResMode, SetParamExSpeakerBeforeStart) {
	DEBUG_TRACE("Case TTSSmokeGenResMode-SetParamExSpeakerBeforeStart Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParamEx(phISSTTS, ISS_TTS_PARAM_SPEAKER, "xiaofeng");
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

	def.initMsg();

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSSmokeGenResMode-SetParamExSpeakerBeforeStart End");
}

// TTS_API_SMOKE_5001
// 正在合成过程中正常设置发音人
TEST_P(TTSSmokeGenResMode, SetParamExSpeakerAfterStart) {
	DEBUG_TRACE("Case TTSSmokeGenResMode-SetParamExSpeakerAfterStart Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
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

	sleep(200);

	err = ISSTTSAdapter::ISSTTSSetParamEx(phISSTTS, ISS_TTS_PARAM_SPEAKER, "xiaofeng");
	EXPECT_EQ(ISS_SUCCESS, err);

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

	def.initMsg();

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSSmokeGenResMode-SetParamExSpeakerAfterStart End");
}

// TTS_API_SMOKE_5002
// 设置日志路径、日志写文件
TEST_P(TTSSmokeGenResMode, SetParamExLogBeforeStart) {
	DEBUG_TRACE("Case TTSSmokeGenResMode-SetParamExLogBeforeStart Start");
#if VA_ANDROID
	const char * log_dir = "/sdcard/tmp_tts_log";
#else
	const char * log_dir = "tmp_tts_log";
#endif
	ASSERT_TRUE(create_directories(log_dir));

	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetLogCfgParam(ISS_TTS_PARAM_LOG_LEVEL, "-1");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetLogCfgParam(ISS_TTS_PARAM_LOG_OUTPUT, "1");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetLogCfgParam(ISS_TTS_PARAM_LOG_FILE_NAME, "tmp_tts_log_file");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetParamEx(phISSTTS, ISS_TTS_PARAM_TMP_LOG_DIR, log_dir);
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
	while (err != ISS_ERROR_TTS_COMPLETED) {
		err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);

		if (piBufferFramesGot != 0) {
			sleep(5);
		}
	}

	long dir_size = 0;
	getFolderSize(log_dir, dir_size);
	EXPECT_TRUE(dir_size > 0);

	getFolderSize(string(def.resDir) + "../Active/TTSRes/log", dir_size);
	EXPECT_TRUE(dir_size == 0);

	EXPECT_TRUE(remove_all(log_dir));

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSSmokeGenResMode-SetParamExLogBeforeStart End");
}

// TTS_API_SMOKE_5003
// 合成过程中设置日志路径、日志写文件
TEST_P(TTSSmokeGenResMode, SetParamExLogAfterStart) {
	DEBUG_TRACE("Case TTSSmokeGenResMode-SetParamExLogAfterStart Start");
#if VA_ANDROID
	const char * log_dir = "/sdcard/tmp_tts_log";
#else
	const char * log_dir = "tmp_tts_log";
#endif
	ASSERT_TRUE(create_directories(log_dir));

	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetLogCfgParam(ISS_TTS_PARAM_LOG_LEVEL, "-1");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetLogCfgParam(ISS_TTS_PARAM_LOG_OUTPUT, "1");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSSetLogCfgParam(ISS_TTS_PARAM_LOG_FILE_NAME, "tmp_tts_log_file");
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "合成接口测试";
	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	int j = 0;
	while (!def.msgOnDataReady && j++ < 200)
		sleep(10);
	EXPECT_TRUE(j <= 200);

	sleep(200);

	err = ISSTTSAdapter::ISSTTSSetParamEx(phISSTTS, ISS_TTS_PARAM_TMP_LOG_DIR, log_dir);
	EXPECT_EQ(ISS_SUCCESS, err);

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	while (err != ISS_ERROR_TTS_COMPLETED) {
		err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);

		if (piBufferFramesGot == 0) {
			sleep(5);
		}
	}

	long dir_size = 0;
	getFolderSize(log_dir, dir_size);
	EXPECT_TRUE(dir_size == 0);

	getFolderSize(string(def.resDir) + "../Active/TTSRes/log", dir_size);
	EXPECT_TRUE(dir_size == 0);

	def.initMsg();

	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	j = 0;
	while (!def.msgOnDataReady && j++ < 200)
		sleep(10);
	EXPECT_TRUE(j <= 200);

	while (err != ISS_ERROR_TTS_COMPLETED) {
		err = ISSTTSAdapter::ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);

		if (piBufferFramesGot == 0) {
			sleep(5);
		}
	}

	getFolderSize(log_dir, dir_size);
	EXPECT_TRUE(dir_size > 0);

	getFolderSize(string(def.resDir) + "../Active/TTSRes/log", dir_size);
	EXPECT_TRUE(dir_size == 0);

	EXPECT_TRUE(remove_all(log_dir));

	err = ISSTTSAdapter::ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case TTSSmokeGenResMode-SetParamExLogAfterStart End");
}

// TTS_API_SMOKE_6000
// 合成GBK编码文本
TEST_P(TTSSmokeGenResMode, StartGBK) {
	DEBUG_TRACE("Case TTSSmokeGenResMode-StartGBK Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
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
	DEBUG_TRACE("Case TTSSmokeGenResMode-StartGBK End");
}

#if VA_VC
// TTS_API_SMOKE_6001
// 合成UTF16编码文本
TEST_P(TTSSmokeGenResMode, StartUTF16) {
	DEBUG_TRACE("Case TTSSmokeGenResMode-StartUTF16 Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, (const wchar_t *)VA::GBKStrToU16Str("合成接口测试"), wcslen((const wchar_t *)VA::GBKStrToU16Str("合成接口测试")), ISS_TTS_CODEPAGE_GBK);
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
	DEBUG_TRACE("Case TTSSmokeGenResMode-StartUTF16 End");
}
#endif

// TTS_API_SMOKE_6002
// 合成UTF8编码文本
TEST_P(TTSSmokeGenResMode, StartUTF8) {
	DEBUG_TRACE("Case TTSSmokeGenResMode-StartUTF8 Start");
	err = ISSTTSAdapter::ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSAdapter::ISSTTSStart(phISSTTS, (const char *)VA::GBKStrToU8Str("合成接口测试"), strlen((const char *)VA::GBKStrToU8Str("合成接口测试")), ISS_TTS_CODEPAGE_GBK);
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
	DEBUG_TRACE("Case TTSSmokeGenResMode-StartUTF8 End");
}
#endif