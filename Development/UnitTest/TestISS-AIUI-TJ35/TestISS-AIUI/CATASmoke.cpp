#include "TestApiCATA.h"
#include "VAInc.h"
#include "test_log.h"

#include <cstdio>

using namespace VA;

class CATASmokeTest : public ::testing::Test {
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


TEST_F(CATASmokeTest, IndexAndSearchCreateDestroy0) {
	err = ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, "test.bin", 0, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataSearchCreate(&phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataSearchDestroy(phISSCATASEARCH);
	EXPECT_EQ(ISS_SUCCESS, err);
}

TEST_F(CATASmokeTest, IndexAndSearchCreateDestroy1) {
	err = ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, "test.bin", 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataSearchCreate(&phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataSearchDestroy(phISSCATASEARCH);
	EXPECT_EQ(ISS_SUCCESS, err);
}

TEST_F(CATASmokeTest, IndexSearchSomkeCH) {

#if VA_VC
	std::string pdata = (const char*)GBKStrToU8Str("[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"ÐÜÇÙÇÙ\" ]}]");
	std::string query_text  = (const char*)GBKStrToU8Str("(field=name,query=ÐÜÇÙÇÙ,fuzzy=0,searchType=KEYBOARD)");
#elif VA_GNUC
	std::string pdata = "[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"ÐÜÇÙÇÙ\" ]}]";
	std::string query_text  = "(field=name,query=ÐÜÇÙÇÙ,fuzzy=0,searchType=KEYBOARD)";
#endif
	err = ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, "test.bin", 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataIndexAddIdxEntity(phISSCATAINDEX, pdata.c_str());
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSCataIndexEndIdxEntity(phISSCATAINDEX);
	EXPECT_EQ(err, ISS_SUCCESS);

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

	//´´½¨ËÑË÷¾ä±ú
	err = ISSCataSearchCreate( &phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 
	
	std::string srh_result = (char*)ISSCataSearchSync( phISSCATASEARCH, query_text.c_str(), &err);
	EXPECT_EQ(err, ISS_SUCCESS); 
	EXPECT_STRNE(srh_result.c_str(), "");

	DEBUG_TRACE("srh_result:%s", PrtU8(srh_result.c_str()));

	err = ISSCataSearchDestroy( phISSCATASEARCH);
	EXPECT_EQ(err, ISS_SUCCESS); 

}

TEST_F(CATASmokeTest, IndexSearchSomkeEn) {

	std::string pdata="[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"xqq\" ]}]";
	std::string query_text  = "(field=name,query=xqq,fuzzy=0,searchType=KEYBOARD)";

	err = ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, "test.bin", 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataIndexAddIdxEntity(phISSCATAINDEX, pdata.c_str());
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSCataIndexEndIdxEntity(phISSCATAINDEX);
	EXPECT_EQ(err, ISS_SUCCESS);

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

	//´´½¨ËÑË÷¾ä±ú
	err = ISSCataSearchCreate( &phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 
	
	std::string srh_result = (char*)ISSCataSearchSync( phISSCATASEARCH, query_text.c_str(), &err);
	EXPECT_EQ(err, ISS_SUCCESS); 
	EXPECT_STRNE(srh_result.c_str(), "");

	DEBUG_TRACE("srh_result:%s", PrtU8(srh_result.c_str()));

	err = ISSCataSearchDestroy( phISSCATASEARCH);
	EXPECT_EQ(err, ISS_SUCCESS); 

}

TEST_F(CATASmokeTest, IndexExSearchExSomkeCH) {

#if VA_VC
	std::string pdata = (const char*)GBKStrToU8Str("[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"ËÎæÌÈ»\" ]}]");
	std::string query_text  = (const char*)GBKStrToU8Str("(field=name,query=ËÎæÌÈ»,fuzzy=0,searchType=KEYBOARD)");
#elif VA_GNUC
	std::string pdata = "[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"ËÎæÌÈ»\" ]}]";
	std::string query_text  = "(field=name,query=ËÎæÌÈ»,fuzzy=0,searchType=KEYBOARD)";
#endif

	err = ISSCataIndexCreateEx(&phISSCATAINDEX, def.resDir, "test.bin", ISS_CATA_LANG_MANDARIN, 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataIndexAddIdxEntity(phISSCATAINDEX, pdata.c_str());
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSCataIndexEndIdxEntity(phISSCATAINDEX);
	EXPECT_EQ(err, ISS_SUCCESS);

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

	//´´½¨ËÑË÷¾ä±ú
	err = ISSCataSearchCreateEx( &phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, ISS_CATA_LANG_MANDARIN, NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 
	
	std::string srh_result = (char*)ISSCataSearchSync( phISSCATASEARCH, query_text.c_str(), &err);
	EXPECT_EQ(err, ISS_SUCCESS); 
	EXPECT_STRNE(srh_result.c_str(), "");

	DEBUG_TRACE("srh_result:%s", PrtU8(srh_result.c_str()));

	err = ISSCataSearchDestroy( phISSCATASEARCH);
	EXPECT_EQ(err, ISS_SUCCESS); 

}

TEST_F(CATASmokeTest, IndexExSearchExSomkeEn) {

	std::string pdata="[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"celery\" ]}]";
	std::string query_text  = "(field=name,query=celery,fuzzy=0,searchType=KEYBOARD)";

	err = ISSCataIndexCreateEx(&phISSCATAINDEX, def.resDir, "test.bin", ISS_CATA_LANG_ENGLISH, 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataIndexAddIdxEntity(phISSCATAINDEX, pdata.c_str());
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSCataIndexEndIdxEntity(phISSCATAINDEX);
	EXPECT_EQ(err, ISS_SUCCESS);

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

	//´´½¨ËÑË÷¾ä±ú
	err = ISSCataSearchCreateEx( &phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, ISS_CATA_LANG_ENGLISH, NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 
	
	std::string srh_result = (char*)ISSCataSearchSync( phISSCATASEARCH, query_text.c_str(), &err);
	EXPECT_EQ(err, ISS_SUCCESS); 
	EXPECT_STRNE(srh_result.c_str(), "");

	DEBUG_TRACE("srh_result:%s", PrtU8(srh_result.c_str()));

	err = ISSCataSearchDestroy( phISSCATASEARCH);
	EXPECT_EQ(err, ISS_SUCCESS); 

}