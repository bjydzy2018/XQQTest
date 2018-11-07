//#ifndef __ANDROID__

#include "TestApiSeopt.h"
#include "CommonFun.h"

#include <string>
#include <climits>

using namespace VA;
using std::string;
#define AUDIO_CHANNELS 4

int seopt_real2(){
	return rand() % 2;
}

int seopt_real4(){
	return rand() % 4;
}

class RandomSeoptTest : public ::testing::Test {
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

//RandomSeoptTest 0001
TEST_F(RandomSeoptTest, RandomTest) {

	const void* pBufOut = NULL;
	int nSamplesOut = 0;
	int nChannelsOut = 0;
	static const int SAMPLES_NUM = ISS_SEOPT_FRAME_SHIFT * AUDIO_CHANNELS;
	short bufIn[SAMPLES_NUM] = { 0 };

	float fPower = 0;
	float fAngle = 0;

	err = ISSSeoptCreate(&phISSSeopt, def.resDir);
	EXPECT_EQ(ISS_SUCCESS, err);

	while (true) {
		switch (seopt_real2())
		{
			case 0: {
				ISSSeoptProcess(phISSSeopt, bufIn, ISS_SEOPT_FRAME_SHIFT, &pBufOut, &nSamplesOut, &nChannelsOut);
				break;
			}
			case 1: {
				switch(seopt_real4()){
					case 0:{
						ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAB_VAD_ONLY_MODE);
						ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_BEAM_INDEX, ISS_SEOPT_PARAM_VALUE_BEAM_LEFT);
						break;
					}
					case 1:{
						ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAB_MODE);
						ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_BEAM_INDEX, ISS_SEOPT_PARAM_VALUE_BEAM_LEFT);
						break;
					}
					case 2:{
						ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAE_MODE);
						break;
					}
					case 3:{
						//MABºÍMAEÄ£Ê½
						ISSSeoptSetParam(phISSSeopt, ISS_SEOPT_PARAM_WORK_MODE, ISS_SEOPT_PARAM_VALUE_MAB_AND_MAE_MODE);
						break;
					}
					default:
						break;
				}
				
				break;
			}
			default:
				break;
		}
	}

	{
		isslog_tracer_perf("ISSSeoptDestroy");
		err = ISSSeoptDestroy(phISSSeopt);
	}

	
}
