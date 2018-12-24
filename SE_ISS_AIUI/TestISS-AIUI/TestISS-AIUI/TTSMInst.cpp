

#include "TestApiTTS.h"
#include "CommonFun.h"

#include <boost/thread/thread.hpp>

using namespace VA;
using boost::thread;
#ifdef __ANDROID__
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

//// TTS_API_MInst_0001
//// 同一个资源句柄，创建20个实例，单线程同时进行合成
//TEST_F(TTSMInst, MInst20SingleThread) {
//	const int handles_num = 20;
//
//	err = ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
//	EXPECT_EQ(ISS_SUCCESS, err);
//
//	HISSTTS	 phISSTTS[handles_num] = { NULL };
//	char * text = "合成接口测试";
//	DEBUG_TRACE_DEBUG("before create handles_num tts handles done");
//	for (int i = 0; i < handles_num; i++)
//	{
//		err = ISSTTSCreate(&phISSTTS[i], phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
//		EXPECT_EQ(ISS_SUCCESS, err);
//
//		err = ISSTTSStart(phISSTTS[i], text, strlen(text), ISS_TTS_CODEPAGE_GBK);
//		EXPECT_EQ(ISS_SUCCESS, err);
//	}
//	DEBUG_TRACE_DEBUG("create handles_num tts handles done");
//
//	int j = 0;
//	while (def.msgOnDataReady < handles_num && j++ < 1000)
//		sleep(10);
//	EXPECT_EQ(handles_num, def.msgOnDataReady);
//
//	char buffer[1600] = { 0 };
//	unsigned int piBufferFramesGot = 0;
//	unsigned int piProgBegin = 0;
//	unsigned int piProgLen = 0;
//
//	for (int k = 0; k < 10; k++)
//	{
//		for (int i = 0; i < handles_num; i++) {
//			err = ISSTTSGetAudioData(phISSTTS[i], buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
//			EXPECT_EQ(ISS_SUCCESS, err);
//		}
//	}
//	DEBUG_TRACE_DEBUG("tts handles getAudioData done");
//
//	for (int i = 0; i < handles_num; i++)
//	{
//		err = ISSTTSStop(phISSTTS[i]);
//		EXPECT_EQ(ISS_SUCCESS, err);
//
//		err = ISSTTSDestroy(phISSTTS[i]);
//		EXPECT_EQ(ISS_SUCCESS, err);
//	}
//	DEBUG_TRACE_DEBUG("destroy handles_num tts handles done");
//
//	err = ISSTTSUnInitRes(phISSTTSRES);
//	EXPECT_EQ(ISS_SUCCESS, err);
//}
//
//// TTS_API_MInst_0002
//// 同一个资源句柄，多线程创建20个实例，进行合成，销毁实例
//void fun() {
//	HISSTTSRES phISSTTSRES = NULL;
//	HISSTTS	 phISSTTS = NULL;
//
//	DefTTS def;
//
//	ISSTTSInitRes(&phISSTTSRES, def.resDir, 1);
//
//	ISSTTSCreate(&phISSTTS, phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
//
//	char * text = "合成接口测试";
//	ISSTTSStart(phISSTTS, text, strlen(text), ISS_TTS_CODEPAGE_GBK);
//
//	int j = 0;
//	while (!def.msgOnDataReady && j++ < 200)
//		sleep(10);
//
//	j = 0;
//	char buffer[1600] = { 0 };
//	unsigned int piBufferFramesGot = 0;
//	unsigned int piProgBegin = 0;
//	unsigned int piProgLen = 0;
//	while (piBufferFramesGot == 0 && j++<10) {
//		ISSTTSGetAudioData(phISSTTS, buffer, 800, &piBufferFramesGot, &piProgBegin, &piProgLen);
//	}
//
//	ISSTTSDestroy(phISSTTS);
//
//	ISSTTSUnInitRes(phISSTTSRES);
//
//	def.initMsg();
//}
//
//TEST_F(TTSMInst, MInst20MThread) {
//	for (int i = 0; i < 20; i++)
//	{
//		thread t(&fun);
//
//		if (i == 19)
//			t.join();
//	}
//}
#endif