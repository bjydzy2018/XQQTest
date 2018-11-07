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
	err = ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL); 
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWDestroy(phISSMVW);
	EXPECT_EQ(ISS_SUCCESS, err);
}

TEST_F(MVWSmokeTest, WakeupGlobalScene) {
	err = ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL); 
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWStart(phISSMVW, ISS_MVW_SCENE_GLOBAL);
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSMVW, "mvw", def.mvwPcm_global, def);
	sleep(1000);
	EXPECT_EQ(1, DefMVW::msgWakeup);

	err = ISSMVWDestroy(phISSMVW);
	EXPECT_EQ(ISS_SUCCESS, err);
}

TEST_F(MVWSmokeTest, SetKeyWordGlobalWakeUp) {
	err = ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL); 
	EXPECT_EQ(ISS_SUCCESS, err);


	//global场景下自定义唤醒词
	err=ISSMVWSetKeyWords(phISSMVW,ISS_MVW_SCENE_GLOBAL,def.mvwKeyWordTxt.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWStart(phISSMVW, ISS_MVW_SCENE_GLOBAL);
	EXPECT_EQ(ISS_SUCCESS, err);

	appendAudio(phISSMVW, "mvw", def.mvwKeyWordPcm, def);
	sleep(1000);
	EXPECT_EQ(1, DefMVW::msgWakeup);

	ISSMVWSetDefaultKeyWords(phISSMVW,ISS_MVW_SCENE_GLOBAL);

	err = ISSMVWDestroy(phISSMVW);
	EXPECT_EQ(ISS_SUCCESS, err);
}

