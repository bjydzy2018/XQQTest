#include "TestApiCATA.h"
#include "VAInc.h"
#include "test_log.h"

#include <cstdio>

using namespace VA;

class CATASmokeTest : public ::testing::TestWithParam<int> {
protected:
	ISSErrID err;
	HISSCATAINDEX phISSCATAINDEX;
	HISSCATASEARCH phISSCATASEARCH;

	DefCATA def;

	virtual void SetUp() {
		err = -1;
		phISSCATAINDEX = NULL;
		phISSCATASEARCH = NULL;
	}

	virtual void TearDown() {
		phISSCATAINDEX = NULL;
		phISSCATASEARCH = NULL;

		DefCATA::initMsg();
		string test_bin = string(def.resDir) + "/test.bin";
		if (remove(test_bin.c_str()) != 0) {
			DEBUG_TRACE_ERROR("remove %s failed", test_bin.c_str());
		}
	}
};

INSTANTIATE_TEST_CASE_P(DropOldResFlag, CATASmokeTest, ::testing::Values(0, 1));

TEST_P(CATASmokeTest, CreateDestroy) {

	err = ISSCATAAdapter::ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, "test.bin", GetParam(), DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCATAAdapter::ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCATAAdapter::ISSCataSearchCreate(&phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCATAAdapter::ISSCataSearchDestroy(phISSCATASEARCH);
	EXPECT_EQ(ISS_SUCCESS, err);
}
