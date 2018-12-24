#include "TestApiMVW.h"
#include "CommonFun.h"

class MVWSmokeTest : public ::testing::Test {
protected:
	ISSErrID err;
	HISSMVW	 phISSMVW;

	DefMVW def;

	virtual void SetUp() {
		err = -1;
		phISSMVW = NULL;
	}

	virtual void TearDown() {
		phISSMVW = NULL;

		DefMVW::initMsg();
	}
};

TEST_F(MVWSmokeTest, CreateDestroy) {
	err = ISSMVWAdapter::ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL); 
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	EXPECT_EQ(ISS_SUCCESS, err);
}

//TEST_F(MVWSmokeTest, EnWakeup) {
//	err = ISSMVWSetMvwLanguage(ISS_MVW_LANG_ENG);
//	EXPECT_EQ(ISS_SUCCESS, err);
//
//	err = ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL);
//	EXPECT_EQ(ISS_SUCCESS, err);
//
//	err = ISSMVWStart(phISSMVW, ISS_MVW_SCENE_SELECT);
//	EXPECT_EQ(ISS_SUCCESS, err);
//
//	appendAudio(phISSMVW, "mvw", def.mvwPcm_first_en, def);
//	//sleep(2000);
//	EXPECT_EQ(1, DefMVW::msgWakeup);
//	def.initMsg();
//
//	err = ISSMVWDestroy(phISSMVW);
//	EXPECT_EQ(ISS_SUCCESS, err);
//}

TEST_F(MVWSmokeTest, WakeupGlobalScene) {
	err = ISSMVWAdapter::ISSMVWSetMvwLanguage(ISS_MVW_LANG_CHN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWAdapter::ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL); 
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWAdapter::ISSMVWStart(phISSMVW, ISS_MVW_SCENE_GLOBAL);
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSMVW, "mvw", def.mvwPcm_global, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	err = ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	EXPECT_EQ(ISS_SUCCESS, err);
}

TEST_F(MVWSmokeTest, WakeupConfirmScene) {
	err = ISSMVWAdapter::ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWAdapter::ISSMVWStart(phISSMVW, ISS_MVW_SCENE_CONFIRM);
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSMVW, "mvw", def.mvwPcm_confirm, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_cancel, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	err = ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	EXPECT_EQ(ISS_SUCCESS, err);
}

TEST_F(MVWSmokeTest, WakeupSelectScene) {
	err = ISSMVWAdapter::ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWAdapter::ISSMVWStart(phISSMVW, ISS_MVW_SCENE_SELECT);
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSMVW, "mvw", def.mvwPcm_first, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_second, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_third, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_fourth, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_fifth, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_sixth, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_last, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	err = ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	EXPECT_EQ(ISS_SUCCESS, err);
}

TEST_F(MVWSmokeTest, WakeupAnswerCallScene) {
	err = ISSMVWAdapter::ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWAdapter::ISSMVWStart(phISSMVW, ISS_MVW_SCENE_ANSWER_CALL);
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSMVW, "mvw", def.mvwPcm_answer, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_hangup, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_cancel, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	err = ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	EXPECT_EQ(ISS_SUCCESS, err);
}


TEST_F(MVWSmokeTest, WakeupAllScene) {
	err = ISSMVWAdapter::ISSMVWSetMvwLanguage(ISS_MVW_LANG_CHN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWAdapter::ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL); 
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWAdapter::ISSMVWStart(phISSMVW, ISS_MVW_SCENE_GLOBAL);
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSMVW, "mvw", def.mvwPcm_global, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();
	
	err = ISSMVWAdapter::ISSMVWStart(phISSMVW, ISS_MVW_SCENE_CONFIRM);
	EXPECT_EQ(ISS_SUCCESS, err);
	
	appendAudio(phISSMVW, "mvw", def.mvwPcm_confirm, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_cancel, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();
	
	err = ISSMVWAdapter::ISSMVWStart(phISSMVW, ISS_MVW_SCENE_SELECT);
	EXPECT_EQ(ISS_SUCCESS, err);
	
	appendAudio(phISSMVW, "mvw", def.mvwPcm_first, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_second, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_third, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_fourth, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_fifth, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_sixth, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_last, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();
	
	err = ISSMVWAdapter::ISSMVWStart(phISSMVW, ISS_MVW_SCENE_ANSWER_CALL);
	EXPECT_EQ(ISS_SUCCESS, err);
	
	appendAudio(phISSMVW, "mvw", def.mvwPcm_answer, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_hangup, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	appendAudio(phISSMVW, "mvw", def.mvwPcm_cancel, def);
	//sleep(2000);
	EXPECT_EQ(1, DefMVW::msgWakeup);
	def.initMsg();

	err = ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	EXPECT_EQ(ISS_SUCCESS, err);
}



