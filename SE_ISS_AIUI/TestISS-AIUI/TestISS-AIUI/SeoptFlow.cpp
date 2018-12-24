//#ifndef __ANDROID__
#ifdef ISS_AIUI

#ifndef __x86_64__
#include "TestApiSeopt.h"
#include "CommonFun.h"


#include <string>
#include <climits>

using namespace VA;

using std::string;

#define AUDIO_CHANNELS 4

class SeoptFlowTest : public ::testing::Test {
protected:
	ISSErrID err;
	ISSSeoptHandle	 phISSSeopt;

	DefSeopt def;

	virtual void SetUp() {
		err = -1;
		phISSSeopt = NULL;
	}

	virtual void TearDown() {
		phISSSeopt = NULL;
	}
};

//SeoptFlowTest 0001
TEST_F(SeoptFlowTest, CreateDestroy) {
	err = ISSSeoptAdapter::ISSSeoptCreate(&phISSSeopt, def.resDir);
	EXPECT_EQ(ISS_SUCCESS, err);

	{
		isslog_tracer_perf("ISSSeoptDestroy");
		err = ISSSeoptAdapter::ISSSeoptDestroy(phISSSeopt);
	}

	phISSSeopt = NULL;

	err = ISSSeoptAdapter::ISSSeoptCreate(&phISSSeopt, def.resDir);
	EXPECT_EQ(ISS_SUCCESS, err);

	{
		isslog_tracer_perf("ISSSeoptDestroy");
		err = ISSSeoptAdapter::ISSSeoptDestroy(phISSSeopt);
	}
}

//SeoptFlowTest 0002
TEST_F(SeoptFlowTest, CreateDestroy100) {
	for (int i = 0;i < 100;i++) {
		err = ISSSeoptAdapter::ISSSeoptCreate(&phISSSeopt, def.resDir);
		EXPECT_EQ(ISS_SUCCESS, err);

		{
			isslog_tracer_perf("ISSSeoptDestroy");
			err = ISSSeoptAdapter::ISSSeoptDestroy(phISSSeopt);
		}
	}
	
}


//SeoptFlowTest 0003
TEST_F(SeoptFlowTest, SeoptProcess100) {
	const void* pBufOut = NULL;
	int nSamplesOut = 0;
	int nChannelsOut = 0;
	static const int SAMPLES_NUM = ISS_SEOPT_FRAME_SHIFT * AUDIO_CHANNELS;
	short bufIn[SAMPLES_NUM] = { 0 };

	err = ISSSeoptAdapter::ISSSeoptCreate(&phISSSeopt, def.resDir);
	EXPECT_EQ(ISS_SUCCESS, err);

	for (int i = 0;i < 100;i++) {
		err = ISSSeoptAdapter::ISSSeoptProcess(phISSSeopt, bufIn, ISS_SEOPT_FRAME_SHIFT, &pBufOut, &nSamplesOut, &nChannelsOut);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	{
		isslog_tracer_perf("ISSSeoptDestroy");
		err = ISSSeoptAdapter::ISSSeoptDestroy(phISSSeopt);
	}
}
#endif

#endif
//#endif