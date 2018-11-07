#include "TestApiSR.h"
#include "TestApiCATA.h"
#include "CommonFun.h"
#include "TimerForTest.h"

#include "rapidjson/document.h"
#include <string>

using namespace rapidjson;
using std::string;


// 接口流程用例
class SROutsideTest : public ::testing::Test {
protected:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;

	HISSCATAINDEX phISSCATAINDEX;
	HISSCATASEARCH phISSCATASEARCH;

	DefCATA defCata;

	virtual void SetUp() {
		err = -1;
		phISSSR = NULL;
		phISSCATAINDEX = NULL;
		phISSCATASEARCH = NULL;
	}

	virtual void TearDown() {
		phISSSR = NULL;
		phISSCATAINDEX = NULL;
		phISSCATASEARCH = NULL;
		DefSR::initMsg();
	}
};

//// 上传内容异常的词典，之前反馈是卡死
//TEST_F(SROutsideTest, UploadDictContent1) {
//	DEBUG_TRACE("Case SROutsideTest-UploadDictContent1 Start");
//
//	def.OutsideDict1 = readFile(testRes_path + "dict/OutsideDict1.txt", false);
//
//	err = ISSSetMachineCode(def.machineCode.c_str());
//	EXPECT_EQ(ISS_SUCCESS, err);
//
//#ifdef ISS_SR_3
//	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
//#else
//	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
//#endif
//	EXPECT_EQ(ISS_SUCCESS, err);
//	int count = 0;
//	while (!def.msgInitStatus&&++count<3000) {
//		sleep(10);
//	}
//
//	EXPECT_EQ(1, def.msgInitStatus);
//
//	err = ISSSRUpLoadDict(phISSSR, def.OutsideDict1.c_str(), 0);
//	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
//	if(err == 0){
//		count = 0;
//		while (!def.msgUpLoadDictToLocalStatus&&count<3000){
//			sleep(10);
//			count++;
//		}
//		EXPECT_TRUE(def.msgUpLoadDictToLocalStatus);
//	}
//	def.initMsg();
//
//	{
//		isslog_tracer_perf("ISSSRDestroy");
//		err = ISSSRDestroy(phISSSR);
//	}
//	EXPECT_EQ(ISS_SUCCESS, err);
//	DEBUG_TRACE("Case SROutsideTest-UploadDictContent1 End");
//}
//
//
//// 上传内容异常的词典，之前反馈是崩溃
//TEST_F(SROutsideTest, UploadDictContent2) {
//	DEBUG_TRACE("Case SROutsideTest-UploadDictContent2 Start");
//
//	def.OutsideDict2 = readFile(testRes_path + "dict/OutsideDict2.txt", false);
//
//	err = ISSSetMachineCode(def.machineCode.c_str());
//	EXPECT_EQ(ISS_SUCCESS, err);
//
//#ifdef ISS_SR_3
//	err = ISSSRCreateEx(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
//#else
//	err = ISSSRCreate(&phISSSR, ISS_SR_ACOUS_LANG_VALUE_MANDARIN, def.resDir, DefSR::onMsgProc, NULL);
//#endif
//	EXPECT_EQ(ISS_SUCCESS, err);
//	int count = 0;
//	while (!def.msgInitStatus&&++count<3000) {
//		sleep(10);
//	}
//
//	EXPECT_EQ(1, def.msgInitStatus);
//
//	err = ISSSRUpLoadDict(phISSSR, def.OutsideDict2.c_str(), 0);
//	DEBUG_TRACE("ISSSRUpLoadDict err:%d", err);
//	if(err == 0){
//		count = 0;
//		while (!def.msgUpLoadDictToLocalStatus&&count<3000){
//			sleep(10);
//			count++;
//		}
//		EXPECT_TRUE(def.msgUpLoadDictToLocalStatus);
//	}
//	def.initMsg();
//
//	{
//		isslog_tracer_perf("ISSSRDestroy");
//		err = ISSSRDestroy(phISSSR);
//	}
//	EXPECT_EQ(ISS_SUCCESS, err);
//	DEBUG_TRACE("Case SROutsideTest-UploadDictContent2 End");
//}


TEST_F(SROutsideTest, TestCataInst) {

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

	/*DEBUG_TRACE("ISSCataIndexCreate");
	err = ISSCataIndexCreate(&phISSCATAINDEX, defCata.resDir, "test.bin", 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("ISSCataIndexDestroy");
	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);*/

	DEBUG_TRACE("ISSCataSearchCreate");
	err = ISSCataSearchCreate(&phISSCATASEARCH, defCata.resDir, "pinyin_fuzzy,test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("ISSCataSearchDestroy");
	err = ISSCataSearchDestroy(phISSCATASEARCH);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSSR, "sr", def.srPcm_CallZhangSan, def);

	err = ISSSREndAudioData( phISSSR);

	sleep(3000);

	EXPECT_TRUE(def.msgResult);

	if (def.msgResult) {
		Document doc;
		doc.Parse(def.sr_result.c_str());
		string SR_result = doc["text"].GetString();
	}

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}