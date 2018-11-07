#include "TestApiSR.h"
#include "CommonFun.h"
#include "TimerForTest.h"

#include "rapidjson/document.h"
#include <string>

using namespace rapidjson;
using std::string;


// 接口流程用例
class SRFlowTest : public ::testing::Test {
protected:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;

	virtual void SetUp() {
		err = -1;
		phISSSR = NULL;
		srand((unsigned)time(0));
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

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);

	//boost::random::mt19937 gen;
	//boost::uniform_int<> real(1, 200);
	//int sleep_time = real(gen);

	int sleep_time = rand()%200 + 1;

	sleep(sleep_time);

	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-DestroyBeforeUploadDictComplete End");
}

// SR_API_FLOW_2002
// 送音频时destroy
TEST_F(SRFlowTest, DestroyBeforeAppendAudioDataComplete) {
	DEBUG_TRACE("Case SRFlowTest-DestroyBeforeAppendAudioDataComplete Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	
	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
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

	//timer t;
	TimerForTest t;
	int append_time = 0;
	while (append_time++ < 5) {
		if (audio_size - current_position < append_size_each_time) {
			nNumberOfToWrite = (audio_size - current_position) / 2;
		}

		if (first_append) {
			// 判断是否需要去wav头
			if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F') {
				err = ISSSRAppendAudioData(phISSSR, (short *)(buffer + current_position + 44), nNumberOfToWrite, NULL);
				current_position += nNumberOfToWrite * 2 + 44;
			}

			first_append = false;
		}
		else {
			err = ISSSRAppendAudioData(phISSSR, (short *)(buffer + current_position), nNumberOfToWrite, NULL);
			current_position += nNumberOfToWrite * 2;
		}

		//pass_time = t.elapsed() * 1000;
		pass_time = t.elapsed();
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

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-DestroyBeforeAppendAudioDataComplete End");
}

// SR_API_FLOW_3001
// 送音频时start
TEST_F(SRFlowTest,StartWhenAppendAudioData) {
	DEBUG_TRACE("Case SRFlowTest-StartWhenAppendAudioData Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
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

	//timer t;
	TimerForTest t;
	int append_time = 0;
	while (append_time++ < 5) {
		if (audio_size - current_position < append_size_each_time) {
			nNumberOfToWrite = (audio_size - current_position) / 2;
		}

		if (first_append) {
			// 判断是否需要去wav头
			if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F') {
				err = ISSSRAppendAudioData(phISSSR, (short *)(buffer + current_position + 44), nNumberOfToWrite, NULL);
				current_position += nNumberOfToWrite * 2 + 44;
			}

			first_append = false;
		}
		else {
			err = ISSSRAppendAudioData(phISSSR, (short *)(buffer + current_position), nNumberOfToWrite, NULL);
			current_position += nNumberOfToWrite * 2;
		}

		//pass_time = t.elapsed() * 1000;
		pass_time = t.elapsed();
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

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-StartWhenAppendAudioData End");
}

// SR_API_FLOW_3002
// 上传词典完成前start
TEST_F(SRFlowTest, StartWhenUploadDict) {
	DEBUG_TRACE("Case SRFlowTest-StartWhenUploadDict Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	
	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
	EXPECT_EQ(ISS_SUCCESS, err);

	/*boost::random::mt19937 gen;
	boost::uniform_int<> real(0, 150);
	int sleep_time = real(gen);*/

	int sleep_time = rand()%150;

	sleep(sleep_time);

	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	
	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);
	EXPECT_EQ(ISS_ERROR_BUILDING_GRM, err);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-StartWhenUploadDict End");
}

// SR_API_FLOW_4001
// start后stop
TEST_F(SRFlowTest, StopAfterStart) {
	DEBUG_TRACE("Case SRFlowTest-StopAfterStart Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSRStop(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-StopAfterStart End");
}

// SR_API_FLOW_4002
// 不start直接stop
TEST_F(SRFlowTest, StopWithoutStart) {
	DEBUG_TRACE("Case SRFlowTest-StopWithoutStart Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStop(phISSSR);
	EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-StopWithoutStart End");
}

// SR_API_FLOW_6001
// 上传词典时送音频
TEST_F(SRFlowTest, AppendAudioDataWhenUploadDict) {
	DEBUG_TRACE("Case SRFlowTest-AppendAudioDataWhenUploadDict Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
	EXPECT_EQ(ISS_SUCCESS, err);

	//boost::random::mt19937 gen;
	//boost::uniform_int<> real(0, 600);
	//int sleep_time = real(gen);

	int sleep_time = rand()%600;

	sleep(sleep_time);

	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);

	char buffer[320];
	err = ISSSRAppendAudioData(phISSSR, (short *)buffer, 160, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-AppendAudioDataWhenUploadDict End");
}

// SR_API_FLOW_6002
// stop后送音频
TEST_F(SRFlowTest, AppendAudioDataAfterStop) {
	DEBUG_TRACE("Case SRFlowTest-AppendAudioDataAfterStop Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	
	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSRStop(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	char buffer[320];
	err = ISSSRAppendAudioData(phISSSR, (short *)buffer, 160, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-AppendAudioDataAfterStop End");
}

// SR_API_FLOW_7001
// 送音频时上传词典
// 识别时上传词典，上传会先挂起，等待识别结束后再进行上传
TEST_F(SRFlowTest, UploadDictWhenAppendAudioData) {
	DEBUG_TRACE("Case SRFlowTest-UploadDictWhenAppendAudioData Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	
	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
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

	//timer t;
	TimerForTest t;
	int temp = 1;
	while (current_position < audio_size && temp++ > 0) {
		if (audio_size - current_position < append_size_each_time) {
			nNumberOfToWrite = (audio_size - current_position) / 2;
		}

		if (first_append) {
			// 判断是否需要去wav头
			if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F') {
				err = ISSSRAppendAudioData(phISSSR, (short *)(buffer + current_position + 44), nNumberOfToWrite, NULL);
				current_position += nNumberOfToWrite * 2 + 44;
			}

			first_append = false;
		}
		else {
			err = ISSSRAppendAudioData(phISSSR, (short *)(buffer + current_position), nNumberOfToWrite, NULL);
			current_position += nNumberOfToWrite * 2;
		}

		//pass_time = t.elapsed() * 1000;
		pass_time = t.elapsed();
		append_size_sum += append_size_each_time;
		append_time_sum = append_size_sum / 32;
		if (append_time_sum > pass_time)
			sleep((int)(append_time_sum - pass_time));

		if (err != ISS_SUCCESS || (err != ISS_SUCCESS && err != ISS_ERROR_NO_ENOUGH_BUFFER) || def.msgResult
			|| def.msgSpeechEnd || def.msgSpeechTimeOut || def.msgResponseTimeout || def.msgError)
			break;

		if (temp == 3) {
			err = ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
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
		Document doc;
		doc.Parse(def.sr_result.c_str());
		string SR_result = doc["text"].GetString();
		EXPECT_STREQ("导航到科大讯飞", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

#ifndef ISS_SR_3
	err = ISSSRStop(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
#endif // !ISS_SR_3

	i = 0;
	while ((!def.msgUpLoadDictToLocalStatus || !def.msgUpLoadDictToCloudStatus) && i++ < 1000)
		sleep(10);

	EXPECT_TRUE(def.msgUpLoadDictToCloudStatus);
	EXPECT_TRUE(def.msgUpLoadDictToLocalStatus);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SR_API_FLOW_7002
// start后上传词典
TEST_F(SRFlowTest, UploadDictAfterStart) {
	DEBUG_TRACE("Case SRFlowTest-UploadDictAfterStart Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
	EXPECT_EQ(ISS_SUCCESS, err);

	sleep(10000);

	EXPECT_FALSE(def.msgUpLoadDictToCloudStatus);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatus);

	err = ISSSRStop(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	int i = 0;
	while ((!def.msgUpLoadDictToLocalStatus || !def.msgUpLoadDictToCloudStatus) && i++ < 1000)
		sleep(10);

	EXPECT_TRUE(def.msgUpLoadDictToCloudStatus);
	EXPECT_TRUE(def.msgUpLoadDictToLocalStatus);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-UploadDictAfterStart End");
}


//oneshot功能冒烟
TEST_F(SRFlowTest, OneShot) {
	DEBUG_TRACE("Case SRFlowTest-OneShot Start");

	const char* szListMvwOneShot = 
		"{\n"
		"	 \"KeywordsType\": 1,\n"
		"	 \"MvwScene\": 33,\n"
		"    \"Keywords\": [\n"
		"        {\"KeyWordId\":0,\"KeyWord\" : \"导航到\",\"DefaultThreshold40\": 0}, \n"
		"        {\"KeyWordId\":1,\"KeyWord\" : \"我想听\",\"DefaultThreshold40\": 0}, \n"
		"        {\"KeyWordId\":2,\"KeyWord\" : \"打电话给\",\"DefaultThreshold40\": 0} \n"
		"    ]\n"
		"}";

	const char* szListMvwOneShot_error = 
		"{\n"
		"	 \"KeywordsType\": 1,\n"
		"	 \"MvwScene\": 33,\n"
		"    \"Keywords\": \n"
		"        {\"KeyWordId\":0,\"KeyWord\" : \"导航到\",\"DefaultThreshold40\": 0}, \n"
		"        {\"KeyWordId\":1,\"KeyWord\" : \"我想听\",\"DefaultThreshold40\": 0}, \n"
		"        {\"KeyWordId\":2,\"KeyWord\" : \"打电话给\",\"DefaultThreshold40\": 0} \n"
		"    ]\n"
		"}";

	const char* szOneshotCmd = 
		"{\n"
		"    \"MvwScene\": 35,\n"
		"    \"type\": \"oneshot\",\n"
		"    \"list\": [\"李慕白\", \"朱字辛\"]\n"
		"}";

	unsigned int nNumberOfToWrite = 800 ;
	unsigned int pNumberOfWritten = 0;
	short* res = NULL;
	int nSize = 0 ;

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

//#if VA_VC
	err = ISSSRSetMvwKeyWords(phISSSR, 32, (const char *)GBKStrToU8Str(szListMvwOneShot_error));
	EXPECT_EQ(err, ISS_ERROR_INVALID_PARA);  
	
	err = ISSSRSetMvwKeyWords(phISSSR, 32, (const char *)GBKStrToU8Str(szListMvwOneShot));
	EXPECT_EQ(err, ISS_SUCCESS);  
	
	err = ISSSRStart( phISSSR, ISS_SR_SCENE_ONESHOT, ISS_SR_MODE_LOCAL_REC , (const char *)GBKStrToU8Str(szListMvwOneShot));
//#elif VA_GNUC
	/*err = ISSSRSetMvwKeyWords(phISSSR, 32, szListMvwOneShot_error);
	EXPECT_EQ(err, ISS_ERROR_INVALID_PARA);  
	
	err = ISSSRSetMvwKeyWords(phISSSR, 32, szListMvwOneShot);
	EXPECT_EQ(err, ISS_SUCCESS);  

	err = ISSSRStart( phISSSR, ISS_SR_SCENE_ONESHOT, ISS_SR_MODE_LOCAL_REC , szListMvwOneShot);*/
//#endif
	EXPECT_EQ(err, ISS_SUCCESS); 

	appendAudio(phISSSR, "sr", def.srPcm_OneShot, def);
	
	//结束录音（6）
	err = ISSSREndAudioData( phISSSR);

	count = 0;
	while( !def.msgONESHOTMVWResult||!def.msgResult) {
		sleep(10);  
		count ++;
		if(count==1000)
			break;
	}
	
	EXPECT_EQ(1,def.msgONESHOTMVWResult); 
	EXPECT_EQ(1,def.msgResult); 

	err = ISSSRStop( phISSSR);

	DefSR::initMsg();	

//#if VA_VC
	err = ISSSRStart( phISSSR, ISS_SR_SCENE_ONESHOT, ISS_SR_MODE_MIX_REC , (const char *)GBKStrToU8Str(szOneshotCmd));
//#elif VA_GNUC
	//err = ISSSRStart( phISSSR, ISS_SR_SCENE_ONESHOT, ISS_SR_MODE_MIX_REC , szOneshotCmd);
//#endif
	EXPECT_EQ(err, ISS_SUCCESS);

	appendAudio(phISSSR, "sr", def.srPcm_OSHW, def);
	
	//结束录音（6）
	err = ISSSREndAudioData( phISSSR);
	count = 0;
	while( !def.msgONESHOTMVWResult||!def.msgResult) {
		sleep(10);  
		count ++;
		if(count==1000)
			break;
	}

	EXPECT_EQ(1,def.msgONESHOTMVWResult); 
	EXPECT_EQ(1,def.msgResult); 

	err = ISSSRStop( phISSSR);
	
	err = ISSSRDestroy( phISSSR);
	EXPECT_EQ(err, ISS_SUCCESS);  
	
	DEBUG_TRACE("Case SRFlowTest-OneShot End");
}

//可见即可说功能冒烟
TEST_F(SRFlowTest, STKSSmoke) {
	DEBUG_TRACE("Case SRFlowTest-STKSSmoke Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStart( phISSSR, ISS_SR_SCENE_STKS, ISS_SR_MODE_LOCAL_REC , def.srText_STKS_error.c_str());
	DEBUG_TRACE("err = %d",err);
	EXPECT_EQ(err, ISS_ERROR_INVALID_JSON_FMT);  
	
	err = ISSSRStart( phISSSR, ISS_SR_SCENE_STKS, ISS_SR_MODE_LOCAL_REC , def.srText_STKS.c_str());
	EXPECT_EQ(err, ISS_SUCCESS);  

	appendAudio(phISSSR, "sr", def.srPcm_STKS, def);

	err = ISSSREndAudioData( phISSSR);
	
	err = ISSSRDestroy( phISSSR);
	EXPECT_EQ(err, ISS_SUCCESS);  

	DEBUG_TRACE("Case SRFlowTest-STKSSmoke End");
}

//start之前就送音频
TEST_F(SRFlowTest, AppendAudioDataBeforeStart) {
	DEBUG_TRACE("Case SRFlowTest-AppendAudioDataBeforeStart Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	char buffer[320];
	err = ISSSRAppendAudioData(phISSSR, (short *)buffer, 160, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC ,NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSSRStop(phISSSR);
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSSRDestroy( phISSSR);
	EXPECT_EQ(err, ISS_SUCCESS);  

	DEBUG_TRACE("Case SRFlowTest-AppendAudioDataBeforeStart End");
}


//start之前调用endAudioData
TEST_F(SRFlowTest, EndAudioDataBeforeStart) {
	DEBUG_TRACE("Case SRFlowTest-EndAudioDataBeforeStart Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSREndAudioData(phISSSR);
	EXPECT_EQ(err, ISS_ERROR_INVALID_CALL); 

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC ,NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 

	appendAudio(phISSSR, "sr", def.srPcm_NavigateIflytek, def);

	err = ISSSREndAudioData(phISSSR);
	sleep(3000);

	err = ISSSRDestroy( phISSSR);
	EXPECT_EQ(err, ISS_SUCCESS);  

	DEBUG_TRACE("Case SRFlowTest-EndAudioDataBeforeStart End");
}

//start之前调用stop
TEST_F(SRFlowTest, StopBeforeStart) {
	DEBUG_TRACE("Case SRFlowTest-StopBeforeStart Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStop(phISSSR);
	EXPECT_EQ(err, ISS_ERROR_INVALID_CALL); 

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC ,NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 

	appendAudio(phISSSR, "sr", def.srPcm_NavigateIflytek, def);

	err = ISSSREndAudioData(phISSSR);
	sleep(3000);

	err = ISSSRDestroy( phISSSR);
	EXPECT_EQ(err, ISS_SUCCESS);  

	DEBUG_TRACE("Case SRFlowTest-StopBeforeStart End");
}

//AppendAudioData之前调用EndAudioData
TEST_F(SRFlowTest, EndAudioDataBeforeAppendAudioData) {
	DEBUG_TRACE("Case SRFlowTest-EndAudioDataBeforeAppendAudioData Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC ,NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSSREndAudioData(phISSSR);
	EXPECT_EQ(err, ISS_ERROR_NO_SPEECH); 

	char buffer[320];
	err = ISSSRAppendAudioData(phISSSR, (short *)buffer, 160, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);

	err = ISSSREndAudioData(phISSSR);
	EXPECT_EQ(err, ISS_ERROR_INVALID_CALL); 

	err = ISSSRDestroy( phISSSR);
	EXPECT_EQ(err, ISS_SUCCESS);  

	DEBUG_TRACE("Case SRFlowTest-EndAudioDataBeforeAppendAudioData End");
}

//初始化成功之前就destroy
TEST_F(SRFlowTest, DestroyBeforeCreateSuccess){
	DEBUG_TRACE("Case SRFlowTest-DestroyBeforeCreateSuccess Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	sleep(500);

	err = ISSSRDestroy( phISSSR);
	EXPECT_EQ(err, ISS_SUCCESS);  

	DEBUG_TRACE("Case SRFlowTest-DestroyBeforeCreateSuccess End");
}

//初始化成功之前就上传词典
TEST_F(SRFlowTest, UploadDictBeforeCreateSuccess){
	DEBUG_TRACE("Case SRFlowTest-UploadDictBeforeCreateSuccess Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	//sleep(500);

	err = ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgUpLoadDictToLocalStatus&&count<3000){
		sleep(10);
		count++;
	}

	EXPECT_TRUE(def.msgUpLoadDictToLocalStatus);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-UploadDictBeforeCreateSuccess End");
}

//初始化成功之前就上传词典，上传词典成功之前就destroy
TEST_F(SRFlowTest, UploadDictBeforeCreateSuccessAndDestroyBeforeUploadDictSuccess){
	DEBUG_TRACE("Case SRFlowTest-UploadDictBeforeCreateSuccessAndDestroyBeforeUploadDictSuccess Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	//sleep(50);

	err = ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
	EXPECT_EQ(ISS_SUCCESS, err);
	//sleep(50);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-UploadDictBeforeCreateSuccessAndDestroyBeforeUploadDictSuccess End");
}

//上传词典成功之前进行识别
TEST_F(SRFlowTest, SRBeforeUploadDictSuccess){
	DEBUG_TRACE("Case SRFlowTest-SRBeforeUploadDictSuccess Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
	EXPECT_EQ(ISS_SUCCESS, err);
	sleep(50);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_CallBaiYaWei, def);

	err = ISSSREndAudioData( phISSSR);

	sleep(3000);

	EXPECT_TRUE(def.msgResult);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-SRBeforeUploadDictSuccess End");
}

//本地初始化成功之前进行识别
TEST_F(SRFlowTest, SRBeforeLocalSrInit){
	DEBUG_TRACE("Case SRFlowTest-SRBeforeLocalSrInit Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgCloudInitStatus&&++count < 300) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgCloudInitStatus);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_CLOUD_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_NavigateIflytek, def);

	err = ISSSREndAudioData( phISSSR);

	sleep(3000);

	EXPECT_TRUE(def.msgResult);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-SRBeforeLocalSrInit End");
}

//destroy之后再上传词典
TEST_F(SRFlowTest, UpLoadDictAfterDestroy){
	DEBUG_TRACE("Case SRFlowTest-UpLoadDictAfterDestroy Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	for(int i=0;i<10;i++){

#ifdef ISS_SR_3
		err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
		err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
		EXPECT_EQ(ISS_SUCCESS, err);

		int count = 0;
		while (!def.msgInitStatus&&++count < 3000) {
			sleep(10);
		}

		EXPECT_EQ(1, def.msgInitStatus);
	
		err = ISSSRDestroy(phISSSR);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSSRUpLoadDict(phISSSR, def.dict_2000.c_str(), 0);
		EXPECT_EQ(ISS_ERROR_FAIL, err);

		DefSR::initMsg();
	}

	DEBUG_TRACE("Case SRFlowTest-UpLoadDictAfterDestroy End");
}
//设置speechtail进行识别
TEST_F(SRFlowTest, SetSpeechTailSR){
	DEBUG_TRACE("Case SRFlowTest-SetSpeechTailSR Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	//设置ISS_SR_PARAM_SPEECH_TAIL
	err=ISSSRSetParam(phISSSR,ISS_SR_PARAM_SPEECH_TAIL,"5000");
	EXPECT_EQ(err, ISS_SUCCESS);  

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_SpeechTail, def);

	err = ISSSREndAudioData(phISSSR);

	sleep(3000);

	EXPECT_TRUE(def.msgResult);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-SetSpeechTailSR End");

}
//设置ResponseTimeOut
TEST_F(SRFlowTest, SetResponseTimeOut){
	DEBUG_TRACE("Case SRFlowTest-SetResponseTimeOut Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	//设置ISS_SR_PARAM_RESPONSE_TIMEOUT
	err=ISSSRSetParam(phISSSR,ISS_SR_PARAM_RESPONSE_TIMEOUT,"4200");//4000不可以，4200可以
	EXPECT_EQ(err, ISS_SUCCESS);  

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_SetResponseTimeOut, def);

	err = ISSSREndAudioData(phISSSR);

	sleep(3000);

	EXPECT_TRUE(def.msgResult);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-SetResponseTimeOut End");
}
//设置ISS_SR_PARAM_MVW_TIMEOUT
TEST_F(SRFlowTest, SetMvwTimeOut){
	DEBUG_TRACE("Case SRFlowTest-SetMvwTimeOut Start");

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	//设置ISS_SR_PARAM_MVW_TIMEOUT
	err=ISSSRSetParam(phISSSR,ISS_SR_PARAM_MVW_TIMEOUT,"2000");
	EXPECT_EQ(err, ISS_SUCCESS);  

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_SetMvwTimeOut, def);

	err = ISSSREndAudioData(phISSSR);

	sleep(3000);

	EXPECT_TRUE(def.msgResult);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRFlowTest-SetMvwTimeOut End");
}

// SR_API_FLOW_8001
// 上传格式异常的词典
TEST_F(SRFlowTest, UploadDictwrong_format) {
	DEBUG_TRACE("Case SRFlowTest-UploadDictwrong_format Start");

	def.wrong_format = readFile(testRes_path + "dict/wrong_format.txt", false);
	def.wrong_format1 = readFile(testRes_path + "dict/wrong_format1.txt", false);
	def.wrong_format2 = readFile(testRes_path + "dict/wrong_format2.txt", false);
	def.wrong_format3 = readFile(testRes_path + "dict/wrong_format3.txt", false);
	def.wrong_format4 = readFile(testRes_path + "dict/wrong_format4.txt", false);
	def.wrong_format5 = readFile(testRes_path + "dict/wrong_format5.txt", false);
	def.wrong_format6 = readFile(testRes_path + "dict/wrong_format6.txt", false);
	def.wrong_format7 = readFile(testRes_path + "dict/wrong_format7.txt", false);
	def.wrong_format8 = readFile(testRes_path + "dict/wrong_format8.txt", false);
	def.wrong_format9 = readFile(testRes_path + "dict/wrong_format9.txt", false);

	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRUpLoadDict(phISSSR, def.wrong_format.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	err = ISSSRUpLoadDict(phISSSR, def.wrong_format1.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	err = ISSSRUpLoadDict(phISSSR, def.wrong_format2.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	err = ISSSRUpLoadDict(phISSSR, def.wrong_format3.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	err = ISSSRUpLoadDict(phISSSR, def.wrong_format4.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	err = ISSSRUpLoadDict(phISSSR, def.wrong_format5.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	err = ISSSRUpLoadDict(phISSSR, def.wrong_format6.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	err = ISSSRUpLoadDict(phISSSR, def.wrong_format7.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	err = ISSSRUpLoadDict(phISSSR, def.wrong_format8.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	err = ISSSRUpLoadDict(phISSSR, def.wrong_format9.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-UploadDictwrong_format End");
}

// SR_API_FLOW_8002
// 上传内容异常的词典
TEST_F(SRFlowTest, UploadDictwrong_info) {
	DEBUG_TRACE("Case SRFlowTest-UploadDictwrong_info Start");

	def.wrong_info = readFile(testRes_path + "dict/wrong_info.txt", false);
	def.wrong_info1 = readFile(testRes_path + "dict/wrong_info1.txt", false);
	def.wrong_info2 = readFile(testRes_path + "dict/wrong_info2.txt", false);
	def.wrong_info3 = readFile(testRes_path + "dict/wrong_info3.txt", false);
	def.wrong_info4 = readFile(testRes_path + "dict/wrong_info4.txt", false);
	def.wrong_info5 = readFile(testRes_path + "dict/wrong_info5.txt", false);
	def.wrong_info6 = readFile(testRes_path + "dict/wrong_info6.txt", false);
	
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int count = 0;
	while (!def.msgInitStatus&&++count<3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRUpLoadDict(phISSSR, def.wrong_info.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	err = ISSSRUpLoadDict(phISSSR, def.wrong_info1.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	err = ISSSRUpLoadDict(phISSSR, def.wrong_info2.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	err = ISSSRUpLoadDict(phISSSR, def.wrong_info3.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	err = ISSSRUpLoadDict(phISSSR, def.wrong_info4.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	err = ISSSRUpLoadDict(phISSSR, def.wrong_info5.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	err = ISSSRUpLoadDict(phISSSR, def.wrong_info6.c_str(), 0);
	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
	//EXPECT_EQ(ISS_SUCCESS, err);
	sleep(1000);
	EXPECT_FALSE(def.msgUpLoadDictToLocalStatusSuccess);
	def.initMsg();

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-UploadDictwrong_info End");
}

// 设置city
TEST_F(SRFlowTest, SetParamCity) {
	DEBUG_TRACE("Case SRFlowTest-SetParamCity Start");
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int Count = 0;
	while (!def.msgInitStatus&&Count++<3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRSetParam(phISSSR, ISS_SR_PARAM_CITY, (const char*)GBKStrToU8Str("上海"));
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_CITY, "上海", err));

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_CLOUD_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_TodayWeather, def);

	err = ISSSREndAudioData(phISSSR);
	sleep(3000);

	EXPECT_TRUE(def.msgResult);
	
	def.initMsg();

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-SetParamCity End");
}


// 设置经纬度
TEST_F(SRFlowTest, SetParamJWD) {
	DEBUG_TRACE("Case SRFlowTest-SetParamJWD Start");
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int Count = 0;
	while (!def.msgInitStatus&&Count++ < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	//南京
	err = ISSSRSetParam(phISSSR, ISS_SR_PARAM_LONGTITUDE, "118.79647");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LONGTITUDE, "118.79647", err));
	err = ISSSRSetParam(phISSSR, ISS_SR_PARAM_LATITUDE, "32.05838");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LATITUDE, "32.05838", err));

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_MIX_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_TodayWeather, def);

	err = ISSSREndAudioData(phISSSR);
	sleep(3000);
	EXPECT_TRUE(def.msgResult);

	def.initMsg();

	//合肥
	err = ISSSRSetParam(phISSSR, ISS_SR_PARAM_LONGTITUDE, "117.22901");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LONGTITUDE, "117.22901", err));
	err = ISSSRSetParam(phISSSR, ISS_SR_PARAM_LATITUDE, "31.82057");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LATITUDE, "31.82057", err));

	//err = ISSSRSetParam(phISSSR, ISS_SR_PARAM_CITY, (const char*)GBKStrToU8Str("合肥"));
	//isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_CITY, "合肥", err));

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_MIX_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_TodayWeather, def);

	err = ISSSREndAudioData(phISSSR);
	sleep(3000);
	EXPECT_TRUE(def.msgResult);

	def.initMsg();

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-SetParamJWD End");
}


// 设置
TEST_F(SRFlowTest, SetParamCityAndJWD) {
	DEBUG_TRACE("Case SRFlowTest-SetParamCityAndJWD Start");
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int Count = 0;
	while (!def.msgInitStatus&&Count++ < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	//合肥
	err = ISSSRSetParam(phISSSR, ISS_SR_PARAM_LONGTITUDE, "117.22901");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LONGTITUDE, "117.22901", err));
	err = ISSSRSetParam(phISSSR, ISS_SR_PARAM_LATITUDE, "31.82057");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_LATITUDE, "31.82057", err));

	err = ISSSRSetParam(phISSSR, ISS_SR_PARAM_CITY, (const char*)GBKStrToU8Str("北京"));
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", ISS_SR_PARAM_CITY, "北京", err));

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_MIX_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_NavigateWXD, def);

	err = ISSSREndAudioData(phISSSR);
	sleep(3000);
	EXPECT_TRUE(def.msgResult);

	def.initMsg();

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-SetParamCityAndJWD End");
}


// 设置
TEST_F(SRFlowTest, SetParamTheme) {
	DEBUG_TRACE("Case SRFlowTest-SetParamTheme Start");
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int Count = 0;
	while (!def.msgInitStatus&&Count++ < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	//合肥
	err = ISSSRSetParam(phISSSR, "theme", "themeForTest");
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", "theme", "themeForTest", err));

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_CLOUD_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_NavigateWXD, def);

	err = ISSSREndAudioData(phISSSR);
	sleep(3000);
	EXPECT_TRUE(def.msgResult);

	def.initMsg();

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-SetParamTheme End");
}

// 设置
TEST_F(SRFlowTest, SetParamAttachParams) {
	DEBUG_TRACE("Case SRFlowTest-SetParamAttachParams Start");
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int Count = 0;
	while (!def.msgInitStatus&&Count++ < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	string attachParamsStr = "{ \"nlp_params\":{\"city\":\"Hefei\",\"theme\" : \"themeForTest\"},\"iat_params\" : {\"k1\":\"v1\",\"k2\" : \"v2\"} }";

	//合肥
	err = ISSSRSetParam(phISSSR, "attachparams", attachParamsStr.c_str());
	isslog_func(err, ("ISSSRSetParam %s %s ret %d.", "attachParams", attachParamsStr.c_str(), err));

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_CLOUD_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_NavigateWXD, def);

	err = ISSSREndAudioData(phISSSR);
	sleep(3000);
	EXPECT_TRUE(def.msgResult);

	def.initMsg();

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-SetParamAttachParams End");
}

// 设置
TEST_F(SRFlowTest, ChangeSceneAndTranslate) {
	DEBUG_TRACE("Case SRFlowTest-ChangeSceneAndTranslate Start");
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);
	int Count = 0;
	while (!def.msgInitStatus&&Count++ < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	string szAttachParams = "{\"iat_params\":{\"language\":\"zh-cn\",\"accent\":\"mandarin\",\"domain\":\"sms\",\"isFar\":\"0\"},\"trs_params\":{\"from\":\"cn\",\"to\":\"en\"}}";
	//1) 切换为翻译场景
	err = ISSSRSetParam(phISSSR, ISS_SR_PARAM_SCENE, "MaEn");
	EXPECT_EQ(ISS_SUCCESS, err);

	//2) 传attachparams参数
	err = ISSSRSetParam(phISSSR, ISS_SR_PARAM_ATTACHPARAMS, szAttachParams.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_CLOUD_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_TodayWeather, def);

	err = ISSSREndAudioData(phISSSR);
	sleep(3000);
	EXPECT_TRUE(def.msgTransResult);

	def.initMsg();

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRFlowTest-ChangeSceneAndTranslate End");
}