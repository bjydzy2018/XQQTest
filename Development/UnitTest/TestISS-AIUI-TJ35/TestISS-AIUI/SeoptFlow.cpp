//#ifndef __ANDROID__

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
	err = ISSSeoptCreate(&phISSSeopt, def.resDir);
	EXPECT_EQ(ISS_SUCCESS, err);

	{
		isslog_tracer_perf("ISSSeoptDestroy");
		err = ISSSeoptDestroy(phISSSeopt);
	}

	phISSSeopt = NULL;

	err = ISSSeoptCreate(&phISSSeopt, def.resDir);
	EXPECT_EQ(ISS_SUCCESS, err);

	{
		isslog_tracer_perf("ISSSeoptDestroy");
		err = ISSSeoptDestroy(phISSSeopt);
	}
}

//SeoptFlowTest 0002
TEST_F(SeoptFlowTest, CreateDestroy100) {
	for (int i = 0;i < 100;i++) {
		err = ISSSeoptCreate(&phISSSeopt, def.resDir);
		EXPECT_EQ(ISS_SUCCESS, err);

		{
			isslog_tracer_perf("ISSSeoptDestroy");
			err = ISSSeoptDestroy(phISSSeopt);
		}
	}

}


//SeoptFlowTest 0003
TEST_F(SeoptFlowTest, SeoptProcess1000) {
	const void* pBufOut = NULL;
	int nSamplesOut = 0;
	int nChannelsOut = 0;
	static const int SAMPLES_NUM = ISS_SEOPT_FRAME_SHIFT * AUDIO_CHANNELS;
	short bufIn[SAMPLES_NUM] = { 0 };

	err = ISSSeoptCreate(&phISSSeopt, def.resDir);
	EXPECT_EQ(ISS_SUCCESS, err);

	for (int i = 0;i < 1000;i++) {
		err = ISSSeoptProcess(phISSSeopt, bufIn, ISS_SEOPT_FRAME_SHIFT, &pBufOut, &nSamplesOut, &nChannelsOut);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	{
		isslog_tracer_perf("ISSSeoptDestroy");
		err = ISSSeoptDestroy(phISSSeopt);
	}
}

//SeoptFlowTest 0003
TEST_F(SeoptFlowTest, SeoptSetParam) {
	const void* pBufOut = NULL;
	int nSamplesOut = 0;
	int nChannelsOut = 0;
	static const int SAMPLES_NUM = ISS_SEOPT_FRAME_SHIFT * AUDIO_CHANNELS;
	short bufIn[SAMPLES_NUM] = { 0 };

	err = ISSSeoptCreate(&phISSSeopt, def.resDir);
	EXPECT_EQ(ISS_SUCCESS, err);

	for(int count=0;count<10;count++){
		//MAB的onlyvad 模式
		ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAB_VAD_ONLY_MODE);
		ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_BEAM_INDEX, ISS_SEOPT_PARAM_VALUE_BEAM_LEFT);

		//MAB模式
		ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAB_MODE);
		ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_BEAM_INDEX, ISS_SEOPT_PARAM_VALUE_BEAM_LEFT);

		//MAE模式
		ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAE_MODE);

		//MAB和MAE模式
		ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAB_AND_MAE_MODE);
	}


	err = ISSSeoptProcess(phISSSeopt, bufIn, ISS_SEOPT_FRAME_SHIFT, &pBufOut, &nSamplesOut, &nChannelsOut);
	EXPECT_EQ(ISS_SUCCESS, err);

	{
		isslog_tracer_perf("ISSSeoptDestroy");
		err = ISSSeoptDestroy(phISSSeopt);
	}
}

