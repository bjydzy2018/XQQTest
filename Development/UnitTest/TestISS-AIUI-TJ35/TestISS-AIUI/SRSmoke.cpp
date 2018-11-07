#include "TestApiSR.h"
#include "CommonFun.h"
#include "TimerForTest.h"

#include "rapidjson/document.h"

#include <string>

using std::string;
using namespace rapidjson;

// ð������
class SRSmokeTest : public ::testing::Test {
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

// SR_API_SMOKE_1000
// ISSSRCreateǰ������ISSSetMachineCode
TEST_F(SRSmokeTest, CreateDestroyNoMachineCode) {
#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_ERROR_MACHINE_CODE_NOT_SET, err);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);
}

// SR_API_SMOKE_1001
// ������������create��destroy
TEST_F(SRSmokeTest, CreateExCHDestroy) {
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

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

#ifdef ISS_SR_EN
// SR_API_SMOKE_1002
// Ӣ����������create��destroy
TEST_F(SRSmokeTest, CreateExENDestroy) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}
#endif

// SR_API_SMOKE_4001
// stop������Ƶ
TEST_F(SRSmokeTest, AppendAudioDataAfterStop) {
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

	if (err == ISS_SUCCESS) {
		err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
		EXPECT_EQ(ISS_SUCCESS, err);

		if (err == ISS_SUCCESS) {
			err = ISSSRStop(phISSSR);
			EXPECT_EQ(ISS_SUCCESS, err);

			if (err == ISS_SUCCESS) {
				unsigned int pNumberOfWritten = 0;
				char buffer[320];
				err = ISSSRAppendAudioData(phISSSR, (short *)buffer, 160, &pNumberOfWritten);
				EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);
			}
		}
	}

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SR_API_SMOKE_6001
// ʶ��һ��������Ƶ
TEST_F(SRSmokeTest, AppendAudioDataCHAudio) {
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

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_NavigateIflytek, def);

	err = ISSSREndAudioData(phISSSR);

	sleep(3000);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		Document doc;
		doc.Parse(def.sr_result.c_str());
		string SR_result = doc["text"].GetString();
		EXPECT_STREQ("�������ƴ�Ѷ��", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

#ifdef ISS_SR_EN
// SR_API_SMOKE_6002
// ʶ��һ��Ӣ����Ƶ
TEST_F(SRSmokeTest, AppendAudioDataENAudio) {
	err = ISSSetMachineCode(def.machineCode.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_AnyHotel, def);

	err = ISSSREndAudioData(phISSSR);

	sleep(3000);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		Document doc;
		doc.Parse(def.sr_result.c_str());
		string SR_result = doc["text"].GetString();
		EXPECT_STREQ("any hotel along ", SR_result.c_str());
	}

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}
#endif

// SR_API_SMOKE_6003
// �����ʹ���Ƶ
TEST_F(SRSmokeTest, AppendSuperBigAudioData) {
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

	unsigned int pNumberOfWritten = 0;
	char buffer[140000];
	for (int i = 0; i < 1000; i++)
	{
		err = ISSSRAppendAudioData(phISSSR, (short *)buffer, 70000, &pNumberOfWritten);
		EXPECT_EQ(ISS_SUCCESS, err);
	}

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

#ifdef ISS_SR_EN
// SR_API_SMOKE_6004
// ��Ӣ���л�����
TEST_F(SRSmokeTest, AppendAudioDataCHSwitchEN) {
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

	appendAudio(phISSSR, "sr", def.srPcm_NavigateIflytek, def);

	err = ISSSREndAudioData(phISSSR);

	sleep(3000);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		Document doc;
		doc.Parse(def.sr_result.c_str());
		string SR_result = doc["text"].GetString();
		EXPECT_STREQ("�������ƴ�Ѷ��", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	err = ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	def.initMsg();
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_AnyHotel, def);

	err = ISSSREndAudioData(phISSSR);

	sleep(3000);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		Document doc;
		doc.Parse(def.sr_result.c_str());
		string SR_result = doc["text"].GetString();
		EXPECT_STREQ("any hotel along ", SR_result.c_str());
	}

	err = ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	def.initMsg();
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_NavigateIflytek, def);

	err = ISSSREndAudioData(phISSSR);

	sleep(3000);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		Document doc;
		doc.Parse(def.sr_result.c_str());
		string SR_result = doc["text"].GetString();
		EXPECT_STREQ("�������ƴ�Ѷ��", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	err = ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	def.initMsg();
	EXPECT_EQ(ISS_SUCCESS, err);

#ifdef ISS_SR_3
	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#else
	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_ENGLISH, def.resDir, DefSR::onMsgProc, NULL);
#endif
	EXPECT_EQ(ISS_SUCCESS, err);

	count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	EXPECT_EQ(1, def.msgInitStatus);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_AnyHotel, def);

	err = ISSSREndAudioData( phISSSR);

	sleep(3000);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		Document doc;
		doc.Parse(def.sr_result.c_str());
		string SR_result = doc["text"].GetString();
		EXPECT_STREQ("any hotel along ", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	err = ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	def.initMsg();
	EXPECT_EQ(ISS_SUCCESS, err);
}
#endif

// SR_API_SMOKE_7001
// �ϴ���ϵ�˴ʵ䣬������ʶ��
TEST_F(SRSmokeTest, UploadCHContactDict) {
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

	count = 0;
	while (!def.msgUpLoadDictToLocalStatus&&count<3000){
		sleep(10);
		count++;
	}

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_CallBaiYaWei, def);

	err = ISSSREndAudioData( phISSSR);

	sleep(3000);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		Document doc;
		doc.Parse(def.sr_result.c_str());
		string SR_result = doc["text"].GetString();
		EXPECT_STREQ("��绰������γ", (const char *)VA::U8StrToGBKStr(SR_result.c_str()));
	}

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}



// SR_API_SMOKE_8001
// ��������
TEST_F(SRSmokeTest, SRLocalNli) {
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

	count = 0;
	while (!def.msgUpLoadDictToLocalStatus&&count<3000){
		sleep(10);
		count++;
	}

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	string ret="";

	ret=ISSSRLocalNli(phISSSR,(const char*)GBKStrToU8Str("����ؼ���"),ISS_SR_SCENE_ALL,&err);
	DEBUG_TRACE("ISSSRLocalNli result=%s",PrtU8(ret.c_str()));	

	EXPECT_EQ(err, ISS_SUCCESS);  

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}


// SR_API_SMOKE_9001
// �ƶ�����
TEST_F(SRSmokeTest, SRMspNli) {
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

	count = 0;
	while (!def.msgUpLoadDictToLocalStatus&&count<3000){
		sleep(10);
		count++;
	}

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	string ret="";

	ret=ISSSRMspSearch(phISSSR,(const char*)GBKStrToU8Str("��绰 ��ɺ� "),NULL,&err);
	DEBUG_TRACE("ISSSRMspSearch result=%s",PrtU8(ret.c_str()));	
	
	EXPECT_EQ(err, ISS_SUCCESS);  

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}

//����ϴ��ʵ�״̬
TEST_F(SRSmokeTest, CheckGrmBuilding) {

	const char* szList = 
		"{\n"
		"    \"grm\": [\n"
		"    {\n"
		"        \"dictname\": \"contact\",\n"
		"        \"dictcontant\": [\n"
		"           { \"name\": \" ��ɺ� \", \"id\": 0 },\n"
		"           { \"name\": \"���ǳ�\", \"id\": 2 }\n"
		"       ]\n"
		"    },\n"
		"    {\n"
		"        \"dictname\": \"singers\",\n"
		"        \"dictcontant\": [\n"
		"           { \"name\": \"�ܽ���\", \"id\": 0 },\n"
		"           { \"name\": \"�ֿ���\", \"id\": 2 }\n"
		"       ]\n"
		"    },\n"
		"    {\n"
		"        \"dictname\": \"songs\",\n"
		"        \"dictcontant\": [\n"
		"           { \"name\": \"�ջ�̨\", \"id\": 0 },\n"
		"           { \"name\": \"����\", \"id\": 3 }\n"
		"       ]\n"
		"    },\n"
		"    {\n"
		"        \"dictname\": \"apps\",\n"
		"        \"dictcontant\": [\n"
		"           { \"name\": \"��ŭ��С��\", \"id\": 0 },\n"
		"           { \"name\": \"��������\", \"id\": 1 }\n"
		"       ]\n"
		"    },\n"
		"	 {\n"
		"        \"dictname\": \"musercmd\",\n"
		"        \"dictcontant\": [\n"
		"           { \"name\": \"һվʽѧϰ\", \"id\": 0 },\n"
		"           { \"name\": \"����������\", \"id\": 1 }\n"
		"       ]\n"
		"    }\n"
		"    ]\n"
		"}";
	
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

	ISSSRCheckGrmBuilding(phISSSR,"contact",NULL,&err);
	EXPECT_EQ(err, 0); 

	err = ISSSRUpLoadDict( phISSSR, (const U8Char*)GBKStrToU8Str(szList), 0);

	EXPECT_EQ(err, ISS_SUCCESS); 
	count = 0;
	while( def.msgUpLoadDictToLocalStatus !=1&&count<3000) {
		ISSSRCheckGrmBuilding(phISSSR,"apps",NULL,&err);
		sleep(10);
		count++;
	}

	ISSSRCheckGrmBuilding(phISSSR,"contact",NULL,&err);
	EXPECT_EQ(err, 0);  
	
	//�ͷŻỰ��8��
	err = ISSSRDestroy( phISSSR);
	EXPECT_EQ(err, ISS_SUCCESS);  

}

//ͨ���ӿ�������־����
TEST_F(SRSmokeTest, SRSetLogCfgParam) {

	DEBUG_TRACE("Case SRSmokeTest-SRSetLogCfgParam Start");

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

	err=ISSSRSetParam(phISSSR,ISS_SR_PARAM_TMP_LOG_DIR,"testlog/");
	EXPECT_EQ(err, ISS_SUCCESS);  

	err = ISSSRSetLogCfgParam(ISS_SR_PARAM_LOG_FILE_NAME, "TraceLog/");
	EXPECT_EQ(err, ISS_SUCCESS);  

	err = ISSSRSetLogCfgParam(ISS_SR_PARAM_LOG_LEVEL, "-1");
	EXPECT_EQ(err, ISS_SUCCESS);  
	
	err = ISSSRSetLogCfgParam(ISS_SR_PARAM_LOG_OUTPUT, "3");
	

	//�ͷŻỰ��8��
	err = ISSSRDestroy( phISSSR);
	EXPECT_EQ(err, ISS_SUCCESS);  
	
	DEBUG_TRACE("Case SRSmokeTest-SRSetLogCfgParam End");
}



//����һ�����ȳ�ʱ����Ƶ����ʶ��
TEST_F(SRSmokeTest, SpeechTimeOutAudio)
{
	DEBUG_TRACE("Case SRSmokeTest-SpeechTimeOutAudio Start");

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

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_SpeechTimeOut, def);

	err = ISSSREndAudioData(phISSSR);

	sleep(3000);

	EXPECT_TRUE(def.msgSpeechTimeOut);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRSmokeTest-SpeechTimeOutAudio End");

}
//����һ��û��˵������Ƶ����ʶ��
TEST_F(SRSmokeTest, ResponseTimeOutAudio)
{
	DEBUG_TRACE("Case SRSmokeTest-ResponseTimeOutAudio Start");

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

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_ResponseTimeOut, def);

	err = ISSSREndAudioData(phISSSR);

	sleep(3000);

	EXPECT_TRUE(def.msgResponseTimeout);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRSmokeTest-ResponseTimeOutAudio End");

}
//����100ms��Ƶ
TEST_F(SRSmokeTest, AppendAudioData100ms){
	DEBUG_TRACE("Case SRSmokeTest-AppendAudioData100ms Start");

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

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_AppendAudioData100ms, def);

	err = ISSSREndAudioData(phISSSR);
	EXPECT_EQ(ISS_ERROR_NO_SPEECH, err);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRSmokeTest-AppendAudioData100ms End");
}

//����ط�������
TEST_F(SRSmokeTest, AppendNoiseAudio){
	DEBUG_TRACE("Case SRSmokeTest-AppendNoiseAudio Start");

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

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_AppendNoiseAudioData, def);

	err = ISSSREndAudioData(phISSSR);
	sleep(3000);
	EXPECT_TRUE(def.msgResponseTimeout);

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("Case SRSmokeTest-AppendNoiseAudio End");
}
//����ģʽ�����½���ʶ��
TEST_F(SRSmokeTest, DiffModeAndSceneSR){

	DEBUG_TRACE("Case SRSmokeTest-DiffModeAndSceneSR Start");

	const char* szListMvw = 
		"{ \"Keywords\" : [   {\"KeyWordId\" : 0, \"KeyWord\" : \"���ǹ��\", \"DefaultThreshold40\": 0},{\"KeyWordId\" : 1,\"KeyWord\" : \"�ƴ�Ѷ��\", \"DefaultThreshold40\": 0},{\"KeyWordId\" : 2, \"KeyWord\" : \"С�����\", \"DefaultThreshold40\": 0} ]}";

	/*��������*/		
	const char* szCmd_NLP = 
		"{\n"
		"	\"nlptext\": \"��Ҫ�ؼ���\"\n"
		"}";

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

	err = ISSSRStart( phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC ,NULL);
	appendAudio(phISSSR, "sr", def.srPcm_SceneAll, def);
	err = ISSSREndAudioData(phISSSR);
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRStart( phISSSR, ISS_SR_SCENE_POI, ISS_SR_MODE_LOCAL_REC ,NULL);
	appendAudio(phISSSR, "sr", def.srPcm_ScenePoi, def);
	err = ISSSREndAudioData(phISSSR);
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRStart( phISSSR, ISS_SR_SCENE_CONTACTS, ISS_SR_MODE_LOCAL_REC ,NULL);
	appendAudio(phISSSR, "sr", def.srPcm_SceneContact, def);
	err = ISSSREndAudioData(phISSSR);
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRStart( phISSSR, ISS_SR_SCENE_SELECT, ISS_SR_MODE_LOCAL_REC ,NULL);
	appendAudio(phISSSR, "sr", def.srPcm_SceneSelect, def);
	err = ISSSREndAudioData(phISSSR);
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRStart( phISSSR, ISS_SR_SCENE_CONFIRM, ISS_SR_MODE_LOCAL_REC ,NULL);
	appendAudio(phISSSR, "sr", def.srPcm_SceneConfirm, def);
	err = ISSSREndAudioData(phISSSR);
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRStart( phISSSR, ISS_SR_SCENE_ANSWER_CALL, ISS_SR_MODE_LOCAL_REC ,NULL);
	appendAudio(phISSSR, "sr", def.srPcm_SceneAnswerCall, def);
	err = ISSSREndAudioData(phISSSR);
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRStart( phISSSR, ISS_SR_SCENE_SELECT_MVW, ISS_SR_MODE_LOCAL_REC ,NULL);
	appendAudio(phISSSR, "sr", def.srPcm_SceneSelect, def);
	err = ISSSREndAudioData(phISSSR);
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRStart( phISSSR, ISS_SR_SCENE_CONFIRM_MVW, ISS_SR_MODE_LOCAL_REC ,NULL);
	appendAudio(phISSSR, "sr", def.srPcm_SceneConfirm, def);
	err = ISSSREndAudioData(phISSSR);
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRStart( phISSSR, ISS_SR_SCENE_ANSWER_CALL_MVW, ISS_SR_MODE_LOCAL_REC ,NULL);
	appendAudio(phISSSR, "sr", def.srPcm_SceneAnswerCall, def);
	err = ISSSREndAudioData(phISSSR);
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRStart( phISSSR, ISS_SR_SCENE_BUILD_GRM_MVW, ISS_SR_MODE_LOCAL_REC ,(const U8Char*)GBKStrToU8Str(szListMvw));
	appendAudio(phISSSR, "sr", def.srPcm_ScenePoi, def);
	err = ISSSREndAudioData(phISSSR);
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRStart( phISSSR, "Select", ISS_SR_MODE_LOCAL_REC ,NULL);
	appendAudio(phISSSR, "sr", def.srPcm_SceneSelect, def);
	err = ISSSREndAudioData(phISSSR);
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRStart( phISSSR, "Confirm", ISS_SR_MODE_LOCAL_REC ,NULL);
	appendAudio(phISSSR, "sr", def.srPcm_SceneConfirm, def);
	err = ISSSREndAudioData(phISSSR);
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRStart( phISSSR, "AnswerCall", ISS_SR_MODE_LOCAL_REC ,NULL);
	appendAudio(phISSSR, "sr", def.srPcm_SceneAnswerCall, def);
	err = ISSSREndAudioData(phISSSR);
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRStart( phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_CLOUD_REC ,NULL);
	appendAudio(phISSSR, "sr", def.srPcm_SceneAll, def);
	err = ISSSREndAudioData(phISSSR);
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRStart( phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_MIX_REC ,NULL);
	appendAudio(phISSSR, "sr", def.srPcm_SceneAll, def);
	err = ISSSREndAudioData(phISSSR);
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRStart( phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_NLP ,(const U8Char*)GBKStrToU8Str(szCmd_NLP));
	count = 0;
	while (!def.msgResult&&++count<1000) {
		sleep(10);
	}
	EXPECT_TRUE(def.msgResult);
	DefSR::initMsg();

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRSmokeTest-DiffModeAndSceneSR End");

}

// SR_API_SMOKE_9001
// ������
TEST_F(SRSmokeTest, SRHouChuLi) {
	DEBUG_TRACE("Case SRSmokeTest-SRHouChuLi Start");
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

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_CLOUD_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_CallBaiYaWei, def);
	err = ISSSREndAudioData(phISSSR);
	sleep(3000);
	EXPECT_TRUE(def.msgTPPResult);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRSmokeTest-SRHouChuLi End");
}


// PGSʵʱתд
TEST_F(SRSmokeTest, SRPGS) {
	DEBUG_TRACE("Case SRSmokeTest-SRPGS Start");
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

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_CLOUD_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_CallBaiYaWei, def);

	err = ISSSREndAudioData(phISSSR);
	sleep(3000);

	EXPECT_TRUE(def.msgSRResult);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRSmokeTest-SRPGS End");
}


// ������ǰ��δ�׳���ʶ����
TEST_F(SRSmokeTest, ClearResult) {
	DEBUG_TRACE("Case SRSmokeTest-ClearResult Start");
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

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_CLOUD_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("call ISSSRClearResult before");
	ISSSRClearResult(phISSSR);
	DEBUG_TRACE("call ISSSRClearResult after");

	appendAudio(phISSSR, "sr", def.srPcm_CallBaiYaWei, def);

	err = ISSSREndAudioData(phISSSR);
	sleep(3000);

	EXPECT_FALSE(def.msgResult);

	{
		isslog_tracer_perf("ISSSRDestroy");
		err = ISSSRDestroy(phISSSR);
	}
	EXPECT_EQ(ISS_SUCCESS, err);
	DEBUG_TRACE("Case SRSmokeTest-ClearResult End");
}

