#include "TestApiSR.h"
#include "CommonFun.h"

#include "rapidjson/document.h"

#include <string>
#include <climits>

#include <boost/timer.hpp>
using namespace VA;
using boost::timer;
using std::string;
using namespace rapidjson;

// ��Ч����
class SRInvalidPara : public ::testing::Test {
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

// ��Ч����
// ������Դ·������
class SRInvalidParaGenResDir : public ::testing::TestWithParam<const char *> {
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
INSTANTIATE_TEST_CASE_P(ResDir, SRInvalidParaGenResDir, ::testing::Values(".", "123", "asdf"));

// SR_API_INVALID_PARA_0001
// create�������
TEST_F(SRInvalidPara, CreateNULL) {
	DEBUG_TRACE("Case SRInvalidPara-CreateNULL Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);
#ifdef ISS_SR_3
	err = ISSSRCreateEx(NULL, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(NULL, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	DEBUG_TRACE("Case SRInvalidPara-CreateNULL End");
	
}

// SR_API_INVALID_PARA_0002
// create��Դ·������
TEST_F(SRInvalidPara, CreateResDirNULL) {
	DEBUG_TRACE("Case SRInvalidPara-CreateResDirNULL Start");
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, NULL, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, NULL, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	DEBUG_TRACE("Case SRInvalidPara-CreateResDirNULL End");
}

// SR_API_INVALID_PARA_0003~0004
// create��Դ·����Ч
TEST_P(SRInvalidParaGenResDir, CreateResDirInvalid) {
	DEBUG_TRACE("Case SRInvalidPara-CreateResDirInvalid Start");
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, GetParam(), DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, GetParam(), DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_FILE_NOT_FOUND, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	DEBUG_TRACE("Case SRInvalidPara-CreateResDirInvalid End");
}

// SR_API_INVALID_PARA_0005
// create��Դ·�������ַ���
TEST_F(SRInvalidPara, CreateResDirNULLStr) {
	DEBUG_TRACE("Case SRInvalidPara-CreateResDirNULLStr Start");
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "", DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "", DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	DEBUG_TRACE("Case SRInvalidPara-CreateResDirNULLStr End");
}

// SR_API_INVALID_PARA_0006
// create�ص���������
TEST_F(SRInvalidPara, CreateCallbackNULL) {
	DEBUG_TRACE("Case SRInvalidPara-CreateCallbackNULL Start");
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, NULL, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, NULL, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	DEBUG_TRACE("Case SRInvalidPara-CreateCallbackNULL End");
}

#ifdef ISS_SR_EN
// SR_API_INVALID_PARA_1001
// createEx�������
TEST_F(SRInvalidPara, CreateExNULL) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(NULL, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(NULL, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
}

// SR_API_INVALID_PARA_1002
// createEx��Դ·������
TEST_F(SRInvalidPara, CreateExResDirNULL) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, NULL, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, NULL, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
}

// SR_API_INVALID_PARA_1003~1004
// createEx��Դ·����Ч
TEST_P(SRInvalidParaGenResDir, CreateExResDirInvalid) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, GetParam(), DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, GetParam(), DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_FILE_NOT_FOUND, err);
}

// SR_API_INVALID_PARA_1005
// createEx��Դ·������
TEST_F(SRInvalidPara, CreateExResDirNULLStr) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, "", DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, "", DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
}


// SR_API_INVALID_PARA_1006
// createEx�ص����������ַ���
TEST_F(SRInvalidPara, CreateExCallbackNULL) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, "", NULL, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, "", NULL, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
}
#endif

// SR_API_INVALID_PARA_1007
// createEx���ִ���Чֵ
TEST_F(SRInvalidPara, CreateExLangInvalid) {
	DEBUG_TRACE("Case SRInvalidPara-CreateExLangInvalid Start");
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(4), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(4), def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	DEBUG_TRACE("Case SRInvalidPara-CreateExLangInvalid End");
}

// SR_API_INVALID_PARA_3001
// start��������
TEST_F(SRInvalidPara, StartSceneNULL) {
	DEBUG_TRACE("Case SRInvalidPara-StartSceneNULL Start");
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
	err = ISSSRAdapter::ISSSRStart(phISSSR, NULL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRInvalidPara-StartSceneNULL End");
}

// SR_API_INVALID_PARA_3002
// start��������Ч�ַ���
TEST_F(SRInvalidPara, StartSceneInvalid) {
	DEBUG_TRACE("Case SRInvalidPara-StartSceneInvalid Start");
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
	err = ISSSRAdapter::ISSSRStart(phISSSR, "asdf", ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRInvalidPara-StartSceneInvalid End");
}

// SR_API_INVALID_PARA_3003
// startģʽ����ֵ
TEST_F(SRInvalidPara, StartModeNegative) {
	DEBUG_TRACE("Case SRInvalidPara-StartModeNegative Start");
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
	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, -1, "");
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRInvalidPara-StartModeNegative End");
}

// SR_API_INVALID_PARA_3004
// startģʽ��int���ֵ
TEST_F(SRInvalidPara, StartModeIntMax) {
	DEBUG_TRACE("Case SRInvalidPara-StartModeIntMax Start");
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
	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, INT_MAX, "");
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRInvalidPara-StartModeIntMax End");
}

// SR_API_INVALID_PARA_3005
// start�����Ч
TEST_F(SRInvalidPara, StartHandlerInvalid) {
	DEBUG_TRACE("Case SRInvalidPara-StartHandlerInvalid Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "asdf", DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "asdf", DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_FILE_NOT_FOUND, err);
	
	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	DEBUG_TRACE("Case SRInvalidPara-StartHandlerInvalid End");
}

// SR_API_INVALID_PARA_4001
// stop�����Ч
TEST_F(SRInvalidPara, StopHandlerInvalid) {
	DEBUG_TRACE("Case SRInvalidPara-StopHandlerInvalid Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "asdf", DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "asdf", DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_FILE_NOT_FOUND, err);
	
	err = ISSSRAdapter::ISSSRStop(phISSSR);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	DEBUG_TRACE("Case SRInvalidPara-StopHandlerInvalid End");
}

// SR_API_INVALID_PARA_6001
// AppendAudioData�����Ч
TEST_F(SRInvalidPara, AppendAudioDataHandlerInvalid) {
	DEBUG_TRACE("Case SRInvalidPara-AppendAudioDataHandlerInvalid Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "asdf", DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "asdf", DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_FILE_NOT_FOUND, err);
	
	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	unsigned int pNumberOfWritten = 0;
	char buffer[320];
	err = ISSSRAdapter::ISSSRAppendAudioData(phISSSR, (short *)buffer, 160, &pNumberOfWritten);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	DEBUG_TRACE("Case SRInvalidPara-AppendAudioDataHandlerInvalid End");
}

// SR_API_INVALID_PARA_6002
// pNumberOfWritten����
TEST_F(SRInvalidPara, AppendAudioDataNumberOfWrittenNull) {
	DEBUG_TRACE("Case SRInvalidPara-AppendAudioDataNumberOfWrittenNull Start");
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

	int append_size_each_time = 320; // ÿ������Ƶ���ֽ���
	long current_position = 0; // ��ǰ����Ƶ��λ��
	unsigned int nNumberOfToWrite = (append_size_each_time - current_position) / 2; // ÿ������Ƶ��֡��
	char * buffer; // ��Ƶ����

	long append_size_sum = 0; // �������Ƶ�ܴ�С
	long append_time_sum = 0; // �������Ƶ��ʱ��
	double pass_time = 0;	  // ��ʵ������ʱ��

	bool first_append = true;

	ifstream fin(def.srPcm_NavigateIflytek.c_str(), ios::in | ios::binary);

	fin.seekg(0, ios_base::end);
	long audio_size = fin.tellg();

	fin.seekg(0, ios_base::beg);
	buffer = new char[audio_size];
	fin.read(buffer, sizeof(char) * audio_size);
	fin.close();

	timer t;
	while (current_position < audio_size) {
		if (audio_size - current_position < append_size_each_time) {
			nNumberOfToWrite = (audio_size - current_position) / 2;
		}

		if (first_append) {
			// �ж��Ƿ���Ҫȥwavͷ
			if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F') {
				err = ISSSRAdapter::ISSSRAppendAudioData(phISSSR, (short *)(buffer + current_position + 44), nNumberOfToWrite, NULL);
				current_position += nNumberOfToWrite * 2 + 44;
			}

			first_append = false;
		}
		else {
			err = ISSSRAdapter::ISSSRAppendAudioData(phISSSR, (short *)(buffer + current_position), nNumberOfToWrite, NULL);
			current_position += nNumberOfToWrite * 2;
		}

		pass_time = t.elapsed() * 1000;
		append_size_sum += append_size_each_time;
		append_time_sum = append_size_sum / 32;
		if (append_time_sum > pass_time)
			sleep((int)(append_time_sum - pass_time));

		if (err != ISS_SUCCESS || (err != ISS_SUCCESS && err != ISS_ERROR_NO_ENOUGH_BUFFER) || def.msgResult
			|| def.msgSpeechEnd || def.msgSpeechTimeOut || def.msgResponseTimeout || def.msgError)
			break;
	}
	delete[] buffer;
	buffer = NULL;

	int i = 0;
	while (!def.msgResult && i++ < 1000 && !def.msgError)
		sleep(10);

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
		EXPECT_STREQ("�������ƴ�Ѷ��", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRInvalidPara-AppendAudioDataNumberOfWrittenNull End");
}
