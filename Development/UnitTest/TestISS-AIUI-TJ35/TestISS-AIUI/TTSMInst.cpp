#include "TestApiTTS.h"
#include "CommonFun.h"
#include "VAThread.h"

using namespace VA;

#define TTS_INST_THREAD_NUM 20

class TTSMInst : public ::testing::Test {
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

// TTS_API_MInst_0001
// 同一个资源句柄，创建20个实例，单线程同时进行合成
TEST_F(TTSMInst, MInst20SingleThread) {
	const int handles_num = 20;

	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
	EXPECT_EQ(ISS_SUCCESS, err);

	HISSTTS	 phISSTTS[handles_num] = { NULL };
	char * text = "合成接口测试";
	DEBUG_TRACE_DEBUG("before create tts handles");
	for (int i = 0; i < handles_num; i++)
	{
		DEBUG_TRACE_DEBUG("create number: %d", i + 1);

		err = ISSTTSCreate(&phISSTTS[i], phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	DEBUG_TRACE_DEBUG("create tts handles done");

	DEBUG_TRACE_DEBUG("before start tts handles");
	for (int i = 0; i < handles_num; i++)
	{
		DEBUG_TRACE_DEBUG("start number: %d", i + 1);

		err = ISSTTSStart(phISSTTS[i], text, strlen(text), ISS_TTS_CODEPAGE_GBK);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	DEBUG_TRACE_DEBUG("start tts handles done");

	int j = 0;
	while (def.msgOnDataReady < handles_num && j++ < 1000)
		sleep(10);
	EXPECT_EQ(handles_num, def.msgOnDataReady);

	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;

	for (int k = 0; k < 2; k++)
	{
		for (int i = 0; i < handles_num; i++) {
			err = ISSTTSGetAudioData(phISSTTS[i], buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
			EXPECT_EQ(ISS_SUCCESS, err);
		}
	}
	DEBUG_TRACE_DEBUG("tts handles getAudioData done");

	for (int i = 0; i < handles_num; i++)
	{
		err = ISSTTSStop(phISSTTS[i]);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	DEBUG_TRACE_DEBUG("stop tts handles done");

	for (int i = 0; i < handles_num; i++)
	{
		DEBUG_TRACE_DEBUG("destroy number: %d", i + 1);

		err = ISSTTSDestroy(phISSTTS[i]);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	DEBUG_TRACE_DEBUG("destroy tts handles done");

	err = ISSTTSUnInitRes(phISSTTSRES);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// TTS_API_MInst_0002
// 同一个资源句柄，多线程创建20个实例，进行合成，销毁实例
static VAThread_result_t tts_inst_fun(void *args)
{
	HISSTTSRES phISSTTSRES = NULL;
	HISSTTS	 phISSTTS = NULL;

	DefTTS def;

	ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);

	ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);

	char * text = "合成接口测试";
	ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);

	int j = 0;
	while (!def.msgOnDataReady && j++ < 200)
		sleep(10);

	j = 0;
	char buffer[1600] = { 0 };
	unsigned int piBufferFramesGot = 0;
	unsigned int piProgBegin = 0;
	unsigned int piProgLen = 0;
	while (piBufferFramesGot == 0 && j++<10) {
		ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
	}

	ISSTTSDestroy(phISSTTS);

	ISSTTSUnInitRes(phISSTTSRES);

	def.initMsg();

	return NULL;
}

TEST_F(TTSMInst, MInst20MThread) {
	VAThread_handle_t thread_handles[TTS_INST_THREAD_NUM] = {0};

	for (int i=0; i<TTS_INST_THREAD_NUM; i++)
	{
		int ret = VAThreadCreate(&thread_handles[i], (VAThread_func_t)&tts_inst_fun, NULL, "MInst20MThread");
		// TODO: check ret == 0
	}

	for (int i=0; i<TTS_INST_THREAD_NUM; i++)
	{
		int ret = VAThreadExit(thread_handles[i]);
	}
}
