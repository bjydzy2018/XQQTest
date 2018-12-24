#include "TestApiMVW.h"
#include "CommonFun.h"

class MVWFlowTest : public ::testing::Test {
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

TEST_F(MVWFlowTest, StartStop) {
	
	err = ISSMVWAdapter::ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWAdapter::ISSMVWStart(phISSMVW, ISS_MVW_SCENE_GLOBAL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWAdapter::ISSMVWStop(phISSMVW);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	EXPECT_EQ(ISS_SUCCESS, err);
}


TEST_F(MVWFlowTest, StopStart) {

	err = ISSMVWAdapter::ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWAdapter::ISSMVWStop(phISSMVW);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWAdapter::ISSMVWStart(phISSMVW, ISS_MVW_SCENE_GLOBAL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	EXPECT_EQ(ISS_SUCCESS, err);
}

TEST_F(MVWFlowTest, RollCreateDestroy100) {

	for (int i = 0; i < 100; i++) {
		err = ISSMVWAdapter::ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
}

TEST_F(MVWFlowTest, RollStartStop100) {

	err = ISSMVWAdapter::ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	for (int i = 0; i < 100; i++) {

		err = ISSMVWAdapter::ISSMVWStart(phISSMVW, ISS_MVW_SCENE_GLOBAL);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSMVWAdapter::ISSMVWStop(phISSMVW);
		EXPECT_EQ(ISS_SUCCESS, err);
	}

	err = ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	EXPECT_EQ(ISS_SUCCESS, err);

}