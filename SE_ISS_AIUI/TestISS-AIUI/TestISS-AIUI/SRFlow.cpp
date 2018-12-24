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

// 接口流程用例
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
// 上传词典完成前destroy
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
// 送音频时destroy
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

	int append_size_each_time = 320; // 每次送音频的字节数
	long current_position = 0; // 当前送音频的位置
	unsigned int nNumberOfToWrite = (append_size_each_time - current_position) / 2; // 每次送音频的帧数
	char * buffer; // 音频缓存

	long append_size_sum = 0; // 送入的音频总大小
	long append_time_sum = 0; // 送入的音频总时间
	double pass_time = 0;	  // 真实经过的时间

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
			// 判断是否需要去wav头
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
// 送音频时start
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

	int append_size_each_time = 320; // 每次送音频的字节数
	long current_position = 0; // 当前送音频的位置
	unsigned int nNumberOfToWrite = (append_size_each_time - current_position) / 2; // 每次送音频的帧数
	char * buffer; // 音频缓存

	long append_size_sum = 0; // 送入的音频总大小
	long append_time_sum = 0; // 送入的音频总时间
	double pass_time = 0;	  // 真实经过的时间

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
			// 判断是否需要去wav头
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
// 上传词典完成前start
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
// start后stop
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
// 不start直接stop
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
// 上传词典时送音频
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
// stop后送音频
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
// 送音频时上传词典
// 识别时上传词典，上传会先挂起，等待识别结束后再进行上传
//支持识别中上传个性化
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

	int append_size_each_time = 320; // 每次送音频的字节数
	long current_position = 0; // 当前送音频的位置
	unsigned int nNumberOfToWrite = (append_size_each_time - current_position) / 2; // 每次送音频的帧数
	char * buffer; // 音频缓存

	long append_size_sum = 0; // 送入的音频总大小
	long append_time_sum = 0; // 送入的音频总时间
	double pass_time = 0;	  // 真实经过的时间

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
			// 判断是否需要去wav头
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
		//DEBUG_TRACE("识别结果:%s",SR_result.c_str());
		EXPECT_STREQ("导航到科大讯飞", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
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
// start后上传词典
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
// 上传格式异常的词典
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
	//EXPECT_EQ(ISS_SUCCESS, err);//目前不支持start之后上传词典

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
// 上传内容异常的词典
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
// 查询云端词典上传状态，查询上传到云端的词典是否成功
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


// 支持混合识别模式，本地未初始化完成时可用云端识别
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
		EXPECT_STREQ("打电话给白亚纬", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-BeforeLocalSRInitCanCloudSR End");
}


// create完成之前上传词典
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
		EXPECT_STREQ("打电话给白亚纬", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-BeforeSRInitUpLoadDict End");
}


// 设置city
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

	err = ISSSRAdapter::ISSSRSetParam(phISSSR, ISS_SR_PARAM_CITY, (const char*)GBKStrToU8Str("上海"));
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_CITY, "上海", err));

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
		EXPECT_STREQ("今天的天气", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-SetParamCity End");
}


// 设置经纬度
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

	//南京
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
		EXPECT_STREQ("今天的天气", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	def.initMsg();

	//合肥
	err = ISSSRAdapter::ISSSRSetParam(phISSSR, ISS_SR_PARAM_LONGTITUDE, "117.22901");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LONGTITUDE, "117.22901", err));
	err = ISSSRAdapter::ISSSRSetParam(phISSSR, ISS_SR_PARAM_LATITUDE, "31.82057");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LATITUDE, "31.82057", err));

	//err = ISSSRSetParam(phISSSR, ISS_SR_PARAM_CITY, (const char*)GBKStrToU8Str("合肥"));
	//isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_CITY, "合肥", err));

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
		EXPECT_STREQ("今天的天气", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-SetParamJWD End");
}


// 设置
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

	//合肥
	err = ISSSRAdapter::ISSSRSetParam(phISSSR, ISS_SR_PARAM_LONGTITUDE, "117.22901");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LONGTITUDE, "117.22901", err));
	err = ISSSRAdapter::ISSSRSetParam(phISSSR, ISS_SR_PARAM_LATITUDE, "31.82057");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LATITUDE, "31.82057", err));

	err = ISSSRAdapter::ISSSRSetParam(phISSSR, ISS_SR_PARAM_CITY, (const char*)GBKStrToU8Str("北京"));
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_CITY, "北京", err));

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
		EXPECT_STREQ("导航到万象城", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-SetParamCityAndJWD End");
}


// 设置
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

	//合肥
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
		EXPECT_STREQ("导航到万象城", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-SetParamTheme End");
}

// 设置
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

	//合肥
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
		EXPECT_STREQ("导航到万象城", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-SetParamAttachParams End");
}