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
// isstts.cfg��loadtype����Ϊ-1
TEST_F(TTSInvalidPara, InitResLoadTypeN1) {
	def.setResLoadType("all:-1");

	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_ERROR_TTS_RESOURCE_COUNT, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);

	def.setResLoadType("all:2");
}

// TTS_API_INVALID_PARA_1001
// isstts.cfg��loadtype����Ϊ3
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
// �������
TEST_F(TTSInvalidPara, InitResHandleNULL) {
	err = ISSTTSInitRes(NULL, def.resDir, 1);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_1003
// ��Դ·������
TEST_F(TTSInvalidPara, InitResResDirNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, NULL, 1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_1004
// ��Դ·����û����Դ
TEST_F(TTSInvalidPara, InitResResDirNoRes) {
	err = ISSTTSInitRes(&phISSTTSRES, ".", 1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_1005
// ��Դ·��Ϊ��Ч�ַ�����"///��/"��
TEST_F(TTSInvalidPara, InitResResDirInvalid) {
	err = ISSTTSInitRes(&phISSTTSRES, "/*&^%\\", 1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_1006
// ��Դ·��Ϊ��Ч�ַ�����""��
TEST_F(TTSInvalidPara, InitResResDirNULLStr) {
	err = ISSTTSInitRes(&phISSTTSRES, "", 1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_1007
// ���ط�ʽ����
TEST_F(TTSInvalidPara, InitResResMode) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, -1);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_3000
// �ϳ�ʵ�������NULL
TEST_F(TTSInvalidPara, CreateHandleNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(NULL, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_3001
// �ϳ���Դʵ�������NULL
TEST_F(TTSInvalidPara, CreateResHandleNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, NULL, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_3002
// �ص������NULL
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
// SetParam�������
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
// ����id������
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
// �쳣�ķ����˲���ֵ
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
// �������ٲ���ֵȡ��Сֵ-1�����ֵ+1
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
// ��������ֵȡ��Сֵ-1�����ֵ+1
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
// ��������ֵȡ��Сֵ-1�����ֵ+1
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
// ʹ��ģʽ�������ڵ���ֵ
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
// ʹ��ģʽ�������ڵĸ�ֵ
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
// SetParamEx�����NULL
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
// ����id������
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
// �����˲���ֵ������
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
// �����˲���ֵΪ��д
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
// ��־·��������
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
// start�����NULL
TEST_F(TTSInvalidPara, StartHandleNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "�ϳɽӿڲ���";
	err = ISSTTSStart(NULL, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_6001
// start�ı���NULL
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
// start���ȴ���ֵ
TEST_F(TTSInvalidPara, StartSizeNegative) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "�ϳɽӿڲ���";
	err = ISSTTSStart(phISSTTS, text, -1, ISS_TTS_CODEPAGE_GBK);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_6003
// �쳣�ı����ʽ
TEST_F(TTSInvalidPara, StartCodepageInvalid) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "�ϳɽӿڲ���";
	err = ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE(500));
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSTTSDestroy(phISSTTS);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_INVALID_PARA_7000
// GetAudioData�����NULL
TEST_F(TTSInvalidPara, GetAudioDataHandleNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
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
// GetAudioData buffer��NULL
TEST_F(TTSInvalidPara, GetAudioDataBufferNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
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
// GetAudioData piBufferFramesGot��NULL
TEST_F(TTSInvalidPara, GetAudioDataBufferFramesGotNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
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
// GetAudioData piProgBegin��NULL
TEST_F(TTSInvalidPara, GetAudioDataProgBeginNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
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
// GetAudioData piProgLen��NULL
TEST_F(TTSInvalidPara, GetAudioDataProgLenNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
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
// stop�����NULL
TEST_F(TTSInvalidPara, StopHandleNULL) {
	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	char * text = "�ϳɽӿڲ���";
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
// destroy�����NULL
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
// uninit �����NULL
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
