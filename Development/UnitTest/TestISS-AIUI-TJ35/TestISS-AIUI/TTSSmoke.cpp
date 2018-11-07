#include "TestApiTTS.h"
#include "CommonFun.h"
#include "TimerForTest.h"
#include "VAFileSystem.h"
#include <string>

using namespace VA;
using std::string;

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
// isstts.cfg��loadtype����Ϊ0��iResMode��Ϊ1
TEST_F(TTSSmoke, InitResLoadType0IResMode1) {
	def.setResLoadType("all:0");

	//timer t;
	TimerForTest t;
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	//double init_time = t.elapsed() * 1000;
	double init_time = t.elapsed();

	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_TRUE(init_time < 100);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);

	def.setResLoadType("all:2");
}

// TTS_API_SMOKE_1001
// isstts.cfg��loadtype����Ϊ1��iResMode��Ϊ0
TEST_F(TTSSmoke, InitResLoadType1IResMode0) {
	def.setResLoadType("all:1");

	//timer t;
	TimerForTest t;
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 0);
	//double init_time = t.elapsed() * 1000;
	double init_time = t.elapsed();

	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_TRUE(init_time >= 100);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);

	def.setResLoadType("all:2");
}

// TTS_API_SMOKE_1002
// isstts.cfg��loadtype����Ϊ2��iResMode��Ϊ0
TEST_F(TTSSmoke, InitResLoadType2IResMode0) {
	//timer t;
	TimerForTest t;
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 0);
	//double init_time = t.elapsed() * 1000;
	double init_time = t.elapsed();

	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_TRUE(init_time < 100);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_SMOKE_1003
// isstts.cfg��loadtype����Ϊ2��iResMode��Ϊ1
TEST_F(TTSSmoke, InitResLoadType2IResMode1) {
	//timer t;
	TimerForTest t;
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	//double init_time = t.elapsed() * 1000;
	double init_time = t.elapsed();

	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_TRUE(init_time >= 100);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

#if VA_VC
// TTS_API_SMOKE_2000
// isstts.cfg��loadtype����Ϊ2��iResMode��Ϊ0
TEST_F(TTSSmoke, InitResWLoadType2IResMode0) {
	//timer t;
	TimerForTest t;
	err = ISSTTSInitResW(&phISSTTSRES, def.resDirW, 0);
	//double init_time = t.elapsed() * 1000;
	double init_time = t.elapsed();

	EXPECT_EQ(ISS_SUCCESS, err);
	EXPECT_TRUE(init_time < 100);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}
#endif

// TTS_API_SMOKE_3000
// ������ص���������
TEST_P(TTSSmokeGenResMode, CreateDestroy) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_SMOKE_4000
// ���ò��������������ֵ������ѭ���������в���������ֵ���
TEST_P(TTSSmokeGenResMode, SetParamBeforeStart) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
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
		err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_SPEAKER, speakers[i]);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_USERMODE, i);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOICE_SPEED, i * 7235);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOICE_PITCH, i * 7235);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOLUME, i * 7235);
		EXPECT_EQ(ISS_SUCCESS, err);

		char * text = "�ϳɽӿڲ���";
		err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
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
			err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
			EXPECT_EQ(ISS_SUCCESS, err);
		}
		EXPECT_TRUE(j <= 10);

		def.initMsg();
	}

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_SMOKE_4001
// ���ںϳɹ��������ã�ѭ���������в���������ֵ���
TEST_P(TTSSmokeGenResMode, SetParamAfterStart) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	int speakers[5] = {
		60020, // XIAOYAN
		60030, // XIAOFENG
		65040, // XIAOXUE
		65060, // XIAOJIE
		65180  // JIAJIA
	};
	char * text = "�ϳɽӿڲ���";

	for (int i = 0; i < 5; i++)
	{
		err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
		EXPECT_EQ(ISS_SUCCESS, err);

		int j = 0;
		while (!def.msgOnDataReady && j++ < 200)
			sleep(10);
		EXPECT_TRUE(j <= 200);

		sleep(200);

		err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_SPEAKER, speakers[i]);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_USERMODE, i);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOICE_SPEED, i * 7235);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOICE_PITCH, i * 7235);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOLUME, i * 7235);
		EXPECT_EQ(ISS_SUCCESS, err);

		j = 0;
		char buffer[1600] = { 0 };
		unsigned int piBufferFramesGot = 0;
		unsigned int piProgBegin = 0;
		unsigned int piProgLen = 0;
		while (piBufferFramesGot == 0 && j++<10) {
			err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
			EXPECT_EQ(ISS_SUCCESS, err);
		}
		EXPECT_TRUE(j <= 10);

		def.initMsg();
	}

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_SMOKE_4002~4003
// �������ٲ���ֵȡ��Сֵ�����ֵ
TEST_P(TTSSmokeGenParamValue, SetParamVoiceSpeed) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOICE_SPEED, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "�ϳɽӿڲ���";
	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
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
		err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	EXPECT_TRUE(j <= 10);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_SMOKE_4004~4005
// ������������ֵȡ��Сֵ�����ֵ
TEST_P(TTSSmokeGenParamValue, SetParamVoiceVolume) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOLUME, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "�ϳɽӿڲ���";
	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
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
		err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	EXPECT_TRUE(j <= 10);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_SMOKE_4006~4007
// ������������ֵȡ��Сֵ�����ֵ
TEST_P(TTSSmokeGenParamValue, SetParamVoicePitch) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParam(phISSTTS, ISS_TTS_PARAM_VOICE_PITCH, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "�ϳɽӿڲ���";
	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
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
		err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	EXPECT_TRUE(j <= 10);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_SMOKE_5000
// �������÷�����
TEST_P(TTSSmokeGenResMode, SetParamExSpeakerBeforeStart) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParamEx(phISSTTS, ISS_TTS_PARAM_SPEAKER, "xiaofeng");
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "�ϳɽӿڲ���";
	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
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
		err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	EXPECT_TRUE(j <= 10);

	def.initMsg();

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_SMOKE_5001
// ���ںϳɹ������������÷�����
TEST_P(TTSSmokeGenResMode, SetParamExSpeakerAfterStart) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "�ϳɽӿڲ���";
	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	int j = 0;
	while (!def.msgOnDataReady && j++ < 200)
		sleep(10);
	EXPECT_TRUE(j <= 200);

	sleep(200);

	err = ISSTTSSetParamEx(phISSTTS, ISS_TTS_PARAM_SPEAKER, "xiaofeng");
	EXPECT_EQ(ISS_SUCCESS, err);

	j = 0;
	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	while (piBufferFramesGot == 0 && j++<10) {
		err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	EXPECT_TRUE(j <= 10);

	def.initMsg();

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_SMOKE_5002
// ������־·������־д�ļ�
TEST_P(TTSSmokeGenResMode, SetParamExLogBeforeStart) {
#if VA_ANDROID
	const char * log_dir = "/sdcard/tmp_tts_log";
#else
	const char * log_dir = "tmp_tts_log";
#endif
	//ASSERT_TRUE(create_directories(log_dir));
	ASSERT_TRUE(MakeDir(log_dir));

	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetLogCfgParam(ISS_TTS_PARAM_LOG_LEVEL, "-1");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetLogCfgParam(ISS_TTS_PARAM_LOG_OUTPUT, "1");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetLogCfgParam(ISS_TTS_PARAM_LOG_FILE_NAME, "tmp_tts_log_file");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetParamEx(phISSTTS, ISS_TTS_PARAM_TMP_LOG_DIR, log_dir);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "�ϳɽӿڲ���";
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
	while (err != ISS_ERROR_TTS_COMPLETED) {
		err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);

		if (piBufferFramesGot != 0) {
			sleep(5);
		}
	}

	long dir_size = 0;
	getFolderSize(log_dir, dir_size);
	EXPECT_TRUE(dir_size > 0);

	getFolderSize(string(def.resDir) + "../Active/TTSRes/log", dir_size);
	EXPECT_TRUE(dir_size == 0);

	EXPECT_TRUE(DeleteFolder(log_dir));

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_SMOKE_5003
// �ϳɹ�����������־·������־д�ļ�
TEST_P(TTSSmokeGenResMode, SetParamExLogAfterStart) {
#if VA_ANDROID
	const char * log_dir = "/sdcard/tmp_tts_log";
#else
	const char * log_dir = "tmp_tts_log";
#endif
	ASSERT_TRUE(MakeDir(log_dir));

	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetLogCfgParam(ISS_TTS_PARAM_LOG_LEVEL, "-1");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetLogCfgParam(ISS_TTS_PARAM_LOG_OUTPUT, "1");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSSetLogCfgParam(ISS_TTS_PARAM_LOG_FILE_NAME, "tmp_tts_log_file");
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "�ϳɽӿڲ���";
	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	int j = 0;
	while (!def.msgOnDataReady && j++ < 200)
		sleep(10);
	EXPECT_TRUE(j <= 200);

	sleep(200);

	err = ISSTTSSetParamEx(phISSTTS, ISS_TTS_PARAM_TMP_LOG_DIR, log_dir);
	EXPECT_EQ(ISS_SUCCESS, err);

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	while (err != ISS_ERROR_TTS_COMPLETED) {
		err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);

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

	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_SUCCESS, err);

	j = 0;
	while (!def.msgOnDataReady && j++ < 200)
		sleep(10);
	EXPECT_TRUE(j <= 200);

	while (err != ISS_ERROR_TTS_COMPLETED) {
		err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);

		if (piBufferFramesGot == 0) {
			sleep(5);
		}
	}

	getFolderSize(log_dir, dir_size);
	EXPECT_TRUE(dir_size > 0);

	getFolderSize(string(def.resDir) + "../Active/TTSRes/log", dir_size);
	EXPECT_TRUE(dir_size == 0);

	EXPECT_TRUE(DeleteFolder(log_dir));

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_SMOKE_6000
// �ϳ�GBK�����ı�
TEST_P(TTSSmokeGenResMode, StartGBK) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "�ϳɽӿڲ���";
	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
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
		err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	EXPECT_TRUE(j <= 10);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

#if VA_VC
// TTS_API_SMOKE_6001
// �ϳ�UTF16�����ı�
TEST_P(TTSSmokeGenResMode, StartUTF16) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSStart(phISSTTS, (const wchar_t *)VA::GBKStrToU16Str("�ϳɽӿڲ���"), wcslen((const wchar_t *)VA::GBKStrToU16Str("�ϳɽӿڲ���")), ISS_TTS_CODEPAGE_GBK);
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
		err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	EXPECT_TRUE(j <= 10);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}
#endif

// TTS_API_SMOKE_6002
// �ϳ�UTF8�����ı�
TEST_P(TTSSmokeGenResMode, StartUTF8) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, GetParam());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSStart(phISSTTS, (const char *)VA::GBKStrToU8Str("�ϳɽӿڲ���"), strlen((const char *)VA::GBKStrToU8Str("�ϳɽӿڲ���")), ISS_TTS_CODEPAGE_GBK);
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
		err = ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	EXPECT_TRUE(j <= 10);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}
