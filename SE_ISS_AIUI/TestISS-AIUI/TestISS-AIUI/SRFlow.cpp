#include "TestApiSR.h"
#include "CommonFun.h"

#include "rapidjson/document.h"

#include <string>

#include <boost/timer.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
using namespace VA;
using boost::timer;
using std::string;
using namespace rapidjson;

// �ӿ���������
class SRFlowTest : public ::testing::Test {
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

// SR_API_FLOW_2001
// �ϴ��ʵ����ǰdestroy
TEST_F(SRFlowTest, DestroyBeforeUploadDictComplete) {
	DEBUG_TRACE("Case SRFlowTest-DestroyBeforeUploadDictComplete Start");
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

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);

	boost::random::mt19937 gen;
	boost::uniform_int<> real(1, 200);
	int sleep_time = real(gen);

	sleep(sleep_time);

	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-DestroyBeforeUploadDictComplete End");
}

// SR_API_FLOW_2002
// ����Ƶʱdestroy
TEST_F(SRFlowTest, DestroyBeforeAppendAudioDataComplete) {
	DEBUG_TRACE("Case SRFlowTest-DestroyBeforeAppendAudioDataComplete Start");
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
	int append_time = 0;
	while (append_time++ < 5) {
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

	EXPECT_FALSE(def.msgResult);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-DestroyBeforeAppendAudioDataComplete End");
}

// SR_API_FLOW_3001
// ����Ƶʱstart
TEST_F(SRFlowTest,StartWhenAppendAudioData) {
	DEBUG_TRACE("Case SRFlowTest-StartWhenAppendAudioData Start");
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
	int append_time = 0;
	while (append_time++ < 5) {
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

	EXPECT_FALSE(def.msgResult);

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-StartWhenAppendAudioData End");
}

// SR_API_FLOW_3002
// �ϴ��ʵ����ǰstart
TEST_F(SRFlowTest, StartWhenUploadDict) {
	DEBUG_TRACE("Case SRFlowTest-StartWhenUploadDict Start");
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

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
	EXPECT_EQ(ISS_SUCCESS, err);

	boost::random::mt19937 gen;
	boost::uniform_int<> real(0, 150);
	int sleep_time = real(gen);

	sleep(sleep_time);

	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	
	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	EXPECT_EQ(ISS_ERROR_BUILDING_GRM, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-StartWhenUploadDict End");
}

// SR_API_FLOW_4001
// start��stop
TEST_F(SRFlowTest, StopAfterStart) {
	DEBUG_TRACE("Case SRFlowTest-StopAfterStart Start");
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

	err = ISSSRAdapter::ISSSRStop(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-StopAfterStart End");
}

// SR_API_FLOW_4002
// ��startֱ��stop
TEST_F(SRFlowTest, StopWithoutStart) {
	DEBUG_TRACE("Case SRFlowTest-StopWithoutStart Start");
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

	err = ISSSRAdapter::ISSSRStop(phISSSR);
	EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-StopWithoutStart End");
}

// SR_API_FLOW_6001
// �ϴ��ʵ�ʱ����Ƶ
TEST_F(SRFlowTest, AppendAudioDataWhenUploadDict) {
	DEBUG_TRACE("Case SRFlowTest-AppendAudioDataWhenUploadDict Start");
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

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
	EXPECT_EQ(ISS_SUCCESS, err);

	boost::random::mt19937 gen;
	boost::uniform_int<> real(0, 600);
	int sleep_time = real(gen);

	sleep(sleep_time);

	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);

	char buffer[320];
	err = ISSSRAdapter::ISSSRAppendAudioData(phISSSR, (short *)buffer, 160, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-AppendAudioDataWhenUploadDict End");
}

// SR_API_FLOW_6002
// stop������Ƶ
TEST_F(SRFlowTest, AppendAudioDataAfterStop) {
	DEBUG_TRACE("Case SRFlowTest-AppendAudioDataAfterStop Start");
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

	err = ISSSRAdapter::ISSSRStop(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	char buffer[320];
	err = ISSSRAdapter::ISSSRAppendAudioData(phISSSR, (short *)buffer, 160, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-AppendAudioDataAfterStop End");
}

// SR_API_FLOW_7001
// ����Ƶʱ�ϴ��ʵ�
// ʶ��ʱ�ϴ��ʵ䣬�ϴ����ȹ��𣬵ȴ�ʶ��������ٽ����ϴ�
//֧��ʶ�����ϴ����Ի�
TEST_F(SRFlowTest, UploadDictWhenAppendAudioData) {
	DEBUG_TRACE("Case SRFlowTest-UploadDictWhenAppendAudioData Start");
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
	int temp = 1;
	while (current_position < audio_size && temp++ > 0) {
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

		if (temp == 3) {
			err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
			EXPECT_EQ(ISS_SUCCESS, err);
		}
	}
	delete[] buffer;
	buffer = NULL;

	int i = 0;
	while (!def.msgResult && i++ < 1000 && !def.msgError)
		sleep(10);

	EXPECT_TRUE(def.msgResult);
	EXPECT_FALSE(def.msgUpLoadDictToCloudStatus);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);

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
		//DEBUG_TRACE("ʶ����:%s",SR_result.c_str());
		EXPECT_STREQ("�������ƴ�Ѷ��", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

#ifndef ISS_SR_3
	err = ISSSRAdapter::ISSSRStop(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
#endif // !ISS_SR_3

	i = 0;
	while ((!def.msgUpLoadDictToLocalStatus || !def.msgUpLoadDictToCloudStatus) && i++ < 1000)
		sleep(10);

	EXPECT_TRUE(def.msgUpLoadDictToCloudStatus);
	EXPECT_TRUE(def.msgUpLoadDictToLocalStatus);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-UploadDictWhenAppendAudioData End");
}

// SR_API_FLOW_7002
// start���ϴ��ʵ�
TEST_F(SRFlowTest, UploadDictAfterStart) {
	DEBUG_TRACE("Case SRFlowTest-UploadDictAfterStart Start");
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

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
	EXPECT_EQ(ISS_SUCCESS, err);

	sleep(10000);

	EXPECT_FALSE(def.msgUpLoadDictToCloudStatus);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);

	err = ISSSRAdapter::ISSSRStop(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	int i = 0;
	while ((!def.msgUpLoadDictToLocalStatus || !def.msgUpLoadDictToCloudStatus) && i++ < 1000)
		sleep(10);

	EXPECT_TRUE(def.msgUpLoadDictToCloudStatus);
	EXPECT_TRUE(def.msgUpLoadDictToLocalStatus);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-UploadDictAfterStart End");
}


// SR_API_FLOW_8001
// �ϴ���ʽ�쳣�Ĵʵ�
TEST_F(SRFlowTest, UploadDictwrong_format) {
	DEBUG_TRACE("Case SRFlowTest-UploadDictwrong_format Start");
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

	//err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	//EXPECT_EQ(ISS_SUCCESS, err);//Ŀǰ��֧��start֮���ϴ��ʵ�

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_format.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_format1.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_format2.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_format3.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_format4.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_format5.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_format6.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_format7.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_format8.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_format9.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-UploadDictwrong_format End");
}

// SR_API_FLOW_8002
// �ϴ������쳣�Ĵʵ�
TEST_F(SRFlowTest, UploadDictwrong_info) {
	DEBUG_TRACE("Case SRFlowTest-UploadDictwrong_info Start");
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

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_info.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_info1.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_info2.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_info3.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_info4.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_info5.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.wrong_info6.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	def.initMsg();

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-UploadDictwrong_info End");
}

// SR_API_FLOW_9001
// ��ѯ�ƶ˴ʵ��ϴ�״̬����ѯ�ϴ����ƶ˵Ĵʵ��Ƿ�ɹ�
TEST_F(SRFlowTest, QuerySyncStatus) {
	DEBUG_TRACE("Case SRFlowTest-QuerySyncStatus Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}
	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
	EXPECT_EQ(ISS_SUCCESS, err);

	int i = 0;
	while ((!def.msgUpLoadDictToLocalStatus || !def.msgUpLoadDictToCloudStatus) && i++ < 3000)
		sleep(10);

	EXPECT_TRUE(def.msgUpLoadDictToCloudStatus);
	EXPECT_TRUE(def.msgUpLoadDictToLocalStatus);
	if (def.msgSid == "") {
		DEBUG_TRACE("msgSid is empty");
	}
	else {
		std::string strSid;
		Document doc;
		doc.Parse(def.msgSid.c_str());
		if (doc.HasMember("sid")) {
			Value & sid = doc["sid"];
			strSid = string(sid.GetString());
		}
		DEBUG_TRACE("sid is %s", strSid.c_str());
		err = ISSSRAdapter::ISSSRQuerySyncStatus(phISSSR, strSid.c_str());
		EXPECT_EQ(ISS_SUCCESS, err);

	}
	
	def.initMsg();
	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-QuerySyncStatus End");

}


// ֧�ֻ��ʶ��ģʽ������δ��ʼ�����ʱ�����ƶ�ʶ��
TEST_F(SRFlowTest, BeforeLocalSRInitCanCloudSR) {
	DEBUG_TRACE("Case SRFlowTest-BeforeLocalSRInitCanCloudSR Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int Count = 0;
	while (!def.msgCloudInitStatus&&Count++<300) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgCloudInitStatus);

	/*err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
	EXPECT_EQ(ISS_SUCCESS, err);

	while (!def.msgUpLoadDictToLocalStatus)
		sleep(10);*/

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_MIX_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_CallBaiYaWei, def);

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
		EXPECT_STREQ("��绰������γ", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-BeforeLocalSRInitCanCloudSR End");
}


// create���֮ǰ�ϴ��ʵ�
TEST_F(SRFlowTest, BeforeSRInitUpLoadDict) {
	DEBUG_TRACE("Case SRFlowTest-BeforeSRInitUpLoadDict Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	/*int Count = 0;
	while (!def.msgCloudInitStatus&&Count++<300) {
		sleep(10);
	}*/

	//EXPECT_EQ(0, def.msgCloudInitStatus);
	EXPECT_EQ(0, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
	EXPECT_EQ(ISS_ERROR_FAIL, err);

	/*while (!def.msgUpLoadDictToLocalStatus)
		sleep(10);*/

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_MIX_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_CallBaiYaWei, def);

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
		EXPECT_STREQ("��绰������γ", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-BeforeSRInitUpLoadDict End");
}


// ����city
TEST_F(SRFlowTest, SetParamCity) {
	DEBUG_TRACE("Case SRFlowTest-SetParamCity Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int Count = 0;
	while (!def.msgInitStatus&&Count++<3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRAdapter::ISSSRSetParam(phISSSR, ISS_SR_PARAM_CITY, (const char*)GBKStrToU8Str("�Ϻ�"));
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_CITY, "�Ϻ�", err));

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_MIX_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_TodayWeather, def);

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
		EXPECT_STREQ("���������", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-SetParamCity End");
}


// ���þ�γ��
TEST_F(SRFlowTest, SetParamJWD) {
	DEBUG_TRACE("Case SRFlowTest-SetParamJWD Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int Count = 0;
	while (!def.msgInitStatus&&Count++ < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	//�Ͼ�
	err = ISSSRAdapter::ISSSRSetParam(phISSSR, ISS_SR_PARAM_LONGTITUDE, "118.79647");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LONGTITUDE, "118.79647", err));
	err = ISSSRAdapter::ISSSRSetParam(phISSSR, ISS_SR_PARAM_LATITUDE, "32.05838");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LATITUDE, "32.05838", err));

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_MIX_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_TodayWeather, def);

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
		EXPECT_STREQ("���������", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	def.initMsg();

	//�Ϸ�
	err = ISSSRAdapter::ISSSRSetParam(phISSSR, ISS_SR_PARAM_LONGTITUDE, "117.22901");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LONGTITUDE, "117.22901", err));
	err = ISSSRAdapter::ISSSRSetParam(phISSSR, ISS_SR_PARAM_LATITUDE, "31.82057");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LATITUDE, "31.82057", err));

	//err = ISSSRSetParam(phISSSR, ISS_SR_PARAM_CITY, (const char*)GBKStrToU8Str("�Ϸ�"));
	//isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_CITY, "�Ϸ�", err));

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_MIX_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_TodayWeather, def);

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
		EXPECT_STREQ("���������", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-SetParamJWD End");
}


// ����
TEST_F(SRFlowTest, SetParamCityAndJWD) {
	DEBUG_TRACE("Case SRFlowTest-SetParamCityAndJWD Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int Count = 0;
	while (!def.msgInitStatus&&Count++ < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	//�Ϸ�
	err = ISSSRAdapter::ISSSRSetParam(phISSSR, ISS_SR_PARAM_LONGTITUDE, "117.22901");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LONGTITUDE, "117.22901", err));
	err = ISSSRAdapter::ISSSRSetParam(phISSSR, ISS_SR_PARAM_LATITUDE, "31.82057");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LATITUDE, "31.82057", err));

	err = ISSSRAdapter::ISSSRSetParam(phISSSR, ISS_SR_PARAM_CITY, (const char*)GBKStrToU8Str("����"));
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_CITY, "����", err));

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_MIX_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_NavigateWXD, def);

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
		EXPECT_STREQ("�����������", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-SetParamCityAndJWD End");
}


// ����
TEST_F(SRFlowTest, SetParamTheme) {
	DEBUG_TRACE("Case SRFlowTest-SetParamTheme Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int Count = 0;
	while (!def.msgInitStatus&&Count++ < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	//�Ϸ�
	err = ISSSRAdapter::ISSSRSetParam(phISSSR, "theme", "themeForTest");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", "theme", "themeForTest", err));

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_CLOUD_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_NavigateWXD, def);

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
		EXPECT_STREQ("�����������", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-SetParamTheme End");
}

// ����
TEST_F(SRFlowTest, SetParamAttachParams) {
	DEBUG_TRACE("Case SRFlowTest-SetParamAttachParams Start");
	err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRAdapter::ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int Count = 0;
	while (!def.msgInitStatus&&Count++ < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	string attachParamsStr = "{ \"nlp_params\":{\"city\":\"Hefei\",\"theme\" : \"themeForTest\"},\"iat_params\" : {\"k1\":\"v1\",\"k2\" : \"v2\"} }";

	//�Ϸ�
	err = ISSSRAdapter::ISSSRSetParam(phISSSR, "attachparams", attachParamsStr.c_str());
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", "attachParams", attachParamsStr.c_str(), err));

	err = ISSSRAdapter::ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_CLOUD_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_NavigateWXD, def);

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
		EXPECT_STREQ("�����������", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-SetParamAttachParams End");
}