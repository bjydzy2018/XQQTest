#include "TestApiSR.h"
#include "CommonFun.h"
#include "TimerForTest.h"

#include "rapidjson/document.h"
#include <string>

using namespace VA;
using namespace rapidjson;
using std::string;


// À¿—≠ª∑”√¿˝
class EndlessLoopSRTest : public ::testing::Test {
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

// (start) * À¿—≠ª∑
TEST_F(EndlessLoopSRTest, EndlessLoopStart) {
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
	count = 0;
	//while (true)
	while (count<1000)
	{
		err = ISSSRStart(phISSSR, ISS_SR_SCENE_ALL, ISS_SR_MODE_LOCAL_REC, "");
		DEBUG_TRACE("call ISSSRStart");
		//EXPECT_EQ(ISS_SUCCESS, err);
		count++;
	}

	err = ISSSRDestroy(phISSSR);
	EXPECT_EQ(ISS_SUCCESS, err);
}