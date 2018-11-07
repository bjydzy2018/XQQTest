#include "TestApiCATA.h"
#include "VAInc.h"
#include "test_log.h"

#include <cstdio>

using namespace VA;
using std::string;


class CATAStressTest : public ::testing::Test {
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

TEST_F(CATAStressTest, IndexCreateDestroy1000) {

	std::string pdata="[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"xqq\" ]}]";
	std::string query_text  = "(field=name,query=xqq,fuzzy=0,searchType=KEYBOARD)";
	for(int i =0;i<1000;i++){
		err = ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, "test.bin", 1, DefCATA::onCataMsgProc, NULL);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSCataIndexDestroy(phISSCATAINDEX);
		EXPECT_EQ(ISS_SUCCESS, err);
	}

}

TEST_F(CATAStressTest, SearchCreateDestroy1000) {

	std::string pdata="[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"xqq\" ]}]";
	std::string query_text  = "(field=name,query=xqq,fuzzy=0,searchType=KEYBOARD)";

	for(int i =0;i<1000;i++){
		err = ISSCataSearchCreate( &phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
		EXPECT_EQ(err, ISS_SUCCESS); 

		err = ISSCataSearchDestroy( phISSCATASEARCH);
		EXPECT_EQ(err, ISS_SUCCESS); 
	}

}

TEST_F(CATAStressTest, IndexAndSearchCreateDestroy1000) {

	std::string pdata="[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"xqq\" ]}]";
	std::string query_text  = "(field=name,query=xqq,fuzzy=0,searchType=KEYBOARD)";

	for(int i =0;i<1000;i++){
		err = ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, "test.bin", 1, DefCATA::onCataMsgProc, NULL);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSCataIndexDestroy(phISSCATAINDEX);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSCataSearchCreate( &phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
		EXPECT_EQ(err, ISS_SUCCESS); 

		err = ISSCataSearchDestroy( phISSCATASEARCH);
		EXPECT_EQ(err, ISS_SUCCESS); 
	}
	

}

TEST_F(CATAStressTest,AddDelSameIndex1000){

	//只能删除mode=2,只能搜到mode=1
	string pdata="[{\"field\" : \"name\",\"mode\" :1,\"val\" : [ \"chma\" ]},"
		"{\"field\" : \"id\",\"mode\" :2,\"val\" : [ \"333\" ]}]";
	string pdata_del="[{\"field\" : \"id\",\"mode\" :2,\"val\" : [ \"333\" ]}]";
	string query_text  = "(field=name,query=chma,fuzzy=1,searchType=SR)";

	err = ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, "test.bin", 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	for(int i =0;i<1000;i++){
		err = ISSCataIndexAddIdxEntity(phISSCATAINDEX, pdata.c_str());
		EXPECT_EQ(err, ISS_SUCCESS);

		err = ISSCataIndexDelIdxEntity( phISSCATAINDEX, pdata_del.c_str());
		EXPECT_EQ(err, ISS_SUCCESS); 

		err = ISSCataIndexEndIdxEntity( phISSCATAINDEX);
		EXPECT_EQ(err, ISS_SUCCESS);
	}

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

}

TEST_F(CATAStressTest,SearchSameIndex100){

	//只能删除mode=2,只能搜到mode=1
	string pdata="[{\"field\" : \"name\",\"mode\" :1,\"val\" : [ \"chma\" ]},"
		"{\"field\" : \"id\",\"mode\" :2,\"val\" : [ \"333\" ]}]";
	string pdata_del="[{\"field\" : \"id\",\"mode\" :2,\"val\" : [ \"333\" ]}]";
	string query_text  = "(field=name,query=chma,fuzzy=1,searchType=SR)";

	err = ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, "test.bin", 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataIndexAddIdxEntity(phISSCATAINDEX, pdata.c_str());
	EXPECT_EQ(err, ISS_SUCCESS);

	err = ISSCataIndexEndIdxEntity( phISSCATAINDEX);
	EXPECT_EQ(err, ISS_SUCCESS);

	err = ISSCataSearchCreate( &phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 

	for(int i =0;i<100;i++){
		std::string srh_result = (char*)ISSCataSearchSync( phISSCATASEARCH, query_text.c_str(), &err);
		EXPECT_EQ(err, ISS_SUCCESS); 
		EXPECT_STRNE(srh_result.c_str(), "");

		DEBUG_TRACE("srh_result:%s", PrtU8(srh_result.c_str()));
	}

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataSearchDestroy( phISSCATASEARCH);
	EXPECT_EQ(err, ISS_SUCCESS); 

}

