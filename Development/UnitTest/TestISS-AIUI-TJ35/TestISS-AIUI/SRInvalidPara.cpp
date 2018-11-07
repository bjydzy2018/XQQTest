#include "TestApiSR.h"
#include "CommonFun.h"
#include "TimerForTest.h"

#include "rapidjson/document.h"

#include <string>
#include <climits>

using std::string;
using namespace rapidjson;

// 无效参数
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

// 无效参数
// 生成资源路径参数
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
// create句柄传空
TEST_F(SRInvalidPara, CreateNULL) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(NULL, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(NULL, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
}

// SR_API_INVALID_PARA_0002
// create资源路径传空
TEST_F(SRInvalidPara, CreateResDirNULL) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, NULL, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, NULL, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
}

// SR_API_INVALID_PARA_0003~0004
// create资源路径无效
TEST_P(SRInvalidParaGenResDir, CreateResDirInvalid) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, GetParam(), DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, GetParam(), DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_FILE_NOT_FOUND, err);
}

// SR_API_INVALID_PARA_0005
// create资源路径传空字符串
TEST_F(SRInvalidPara, CreateResDirNULLStr) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "", DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "", DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
}

// SR_API_INVALID_PARA_0006
// create回调函数传空
TEST_F(SRInvalidPara, CreateCallbackNULL) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, NULL, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, NULL, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
}

#ifdef ISS_SR_EN
// SR_API_INVALID_PARA_1001
// createEx句柄传空
TEST_F(SRInvalidPara, CreateExNULL) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(NULL, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(NULL, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
}

// SR_API_INVALID_PARA_1002
// createEx资源路径传空
TEST_F(SRInvalidPara, CreateExResDirNULL) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, NULL, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, NULL, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
}

// SR_API_INVALID_PARA_1003~1004
// createEx资源路径无效
TEST_P(SRInvalidParaGenResDir, CreateExResDirInvalid) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, GetParam(), DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, GetParam(), DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_FILE_NOT_FOUND, err);
}

// SR_API_INVALID_PARA_1005
// createEx资源路径传空
TEST_F(SRInvalidPara, CreateExResDirNULLStr) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, "", DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, "", DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
}


// SR_API_INVALID_PARA_1006
// createEx回调函数传空字符串
TEST_F(SRInvalidPara, CreateExCallbackNULL) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, "", NULL, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, "", NULL, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
}
#endif

// SR_API_INVALID_PARA_1007
// createEx语种传无效值
TEST_F(SRInvalidPara, CreateExLangInvalid) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(4), def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(4), def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
}

// SR_API_INVALID_PARA_3001
// start场景传空
TEST_F(SRInvalidPara, StartSceneNULL) {
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

	err = ISSSRStart(phISSSR, NULL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SR_API_INVALID_PARA_3002
// start场景传无效字符串
TEST_F(SRInvalidPara, StartSceneInvalid) {
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

	err = ISSSRStart(phISSSR, "asdf", ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SR_API_INVALID_PARA_3003
// start模式传负值
TEST_F(SRInvalidPara, StartModeNegative) {
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

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, -1, "");
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SR_API_INVALID_PARA_3004
// start模式传int最大值
TEST_F(SRInvalidPara, StartModeIntMax) {
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

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, INT_MAX, "");
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SR_API_INVALID_PARA_3005
// start句柄无效
TEST_F(SRInvalidPara, StartHandlerInvalid) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "asdf", DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "asdf", DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_FILE_NOT_FOUND, err);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);
}

// start的所有正常参数
TEST_F(SRInvalidPara, StartAll) {

	const char* szCmd = 
		"{\n"
		"    \"grm\": [\n"
		"    {\n"
		"        \"dictname\": \"cmd\",\n"
		"        \"dictcontant\": [\n"
		"           { \"name\": \"下一首\", \"id\": 0 },\n"
		"           { \"name\": \"复制黄是个王八蛋\", \"id\": 1 },\n"
		"           { \"name\": \"播放\", \"id\": 2 },\n"
		"           { \"name\": \"下一首\", \"id\": 3 },\n"
		"           { \"name\": \"循环播放\", \"id\": 4},\n"
		"           { \"name\": \"空调-调至十八度\", \"id\": 5 },\n"
		"           { \"name\": \"空调-调到十八度\", \"id\": 6 },\n"
		"           { \"name\": \"空调-调节到十八度\", \"id\": 7 },\n"
		"			{ \"name\": \"一站式学习\", \"id\": 8 }\n"
		"       ]\n"
		"    }\n"
		"    ]\n"
		"}";
	/*本地语义*/		
	const char* szCmd_NLP = 
		"{\n"
		"	\"nlptext\": \"我要回家了\"\n"
		"}";
	const char* szListMvw = 
		"{ \"Keywords\" : [   {\"KeyWordId\" : 0, \"KeyWord\" : \"美亚光电\", \"DefaultThreshold40\": 0},{\"KeyWordId\" : 1,\"KeyWord\" : \"科大讯飞\", \"DefaultThreshold40\": 0},{\"KeyWordId\" : 2, \"KeyWord\" : \"小艾你好\", \"DefaultThreshold40\": 0} ]}";

	string Scenes[17] = {"all","poi","contacts","select","confirm","answer_call","cmdlist_withall",
		"stks","oneshot","selectlist_poi","selectlist_contacts","music","himalayaFM",
	"select_mvw","confirm_mvw","answer_call_mvw","build_grm_mvw"};

	int Modes[6] = {0,1,2,3,4,5}; 

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

	for(int i =0;i<17;i++){
		DEBUG_TRACE("ISSSRStart scene is: %s",Scenes[i].c_str());
		switch(i){
			case 6 :{
				err = ISSSRStart(phISSSR, Scenes[i].c_str(), Modes[1],"");
				EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
				break;
			}
			case 7 :{
				err = ISSSRStart(phISSSR, Scenes[i].c_str(), Modes[1],"");
				EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
				break;
			}
			case 8 :{
				err = ISSSRStart(phISSSR, Scenes[i].c_str(), Modes[1],"");
				EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
				break;
			}
			case 9 :{
				err = ISSSRStart(phISSSR, Scenes[i].c_str(), Modes[1],"");
				EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
				break;
			}
			case 10 :{
				err = ISSSRStart(phISSSR, Scenes[i].c_str(), Modes[1],"");
				EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);
				break;
			}
			case 16 :{
				err = ISSSRStart(phISSSR, Scenes[i].c_str(), Modes[1],GBKStrToU8Str(szListMvw));
				EXPECT_EQ(ISS_SUCCESS, err);
				break;
			}
			default:{
				err = ISSSRStart(phISSSR, Scenes[i].c_str(), Modes[1], "");
				EXPECT_EQ(ISS_SUCCESS, err);
				break;
			}

		}
		
	}
	

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SR_API_INVALID_PARA_4001
// stop句柄无效
TEST_F(SRInvalidPara, StopHandlerInvalid) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "asdf", DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "asdf", DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_FILE_NOT_FOUND, err);

	err = ISSSRStop(phISSSR);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);
}

// SR_API_INVALID_PARA_6001
// AppendAudioData句柄无效
TEST_F(SRInvalidPara, AppendAudioDataHandlerInvalid) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "asdf", DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, "asdf", DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_FILE_NOT_FOUND, err);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	unsigned int pNumberOfWritten = 0;
	char buffer[320];
	err = ISSSRAppendAudioData(phISSSR, (short *)buffer, 160, &pNumberOfWritten);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);
}

// SR_API_INVALID_PARA_6002
// pNumberOfWritten传空
TEST_F(SRInvalidPara, AppendAudioDataNumberOfWrittenNull) {
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
	while (current_position < audio_size) {
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

	int i = 0;
	while (!def.msgResult && i++ < 1000 && !def.msgError)
		sleep(10);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		Document doc;
		doc.Parse(def.sr_result.c_str());
		string SR_result = doc["text"].GetString();
		EXPECT_STREQ("导航到科大讯飞", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}
