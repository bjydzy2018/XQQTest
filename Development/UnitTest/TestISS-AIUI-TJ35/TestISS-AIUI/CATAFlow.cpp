#include "TestApiCATA.h"
#include "VAInc.h"
#include "test_log.h"

#include <cstdio>
using namespace VA;
using std::string;

class CATAFlowTest : public ::testing::Test {
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

TEST_F(CATAFlowTest,AddSameIndex3Times){
	std::string pdata="[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"xqq\" ]}]";
	std::string query_text  = "(field=name,query=xqq,fuzzy=0,searchType=KEYBOARD)";

	err = ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, "test.bin", 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataIndexAddIdxEntity(phISSCATAINDEX, pdata.c_str());
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSCataIndexAddIdxEntity(phISSCATAINDEX, pdata.c_str());
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSCataIndexAddIdxEntity(phISSCATAINDEX, pdata.c_str());
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSCataIndexEndIdxEntity(phISSCATAINDEX);
	EXPECT_EQ(err, ISS_SUCCESS);

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

	//创建搜索句柄
	err = ISSCataSearchCreate( &phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 

	std::string srh_result = (char*)ISSCataSearchSync( phISSCATASEARCH, query_text.c_str(), &err);
	EXPECT_EQ(err, ISS_SUCCESS); 
	EXPECT_STRNE(srh_result.c_str(), "");

	DEBUG_TRACE("srh_result:%s", PrtU8(srh_result.c_str()));

	err = ISSCataSearchDestroy( phISSCATASEARCH);
	EXPECT_EQ(err, ISS_SUCCESS); 
}

TEST_F(CATAFlowTest,DelSameIndex3Times){

	//只能删除mode=2,只能搜到mode=1
	string pdata="[{\"field\" : \"name\",\"mode\" :1,\"val\" : [ \"chma\" ]},"
		"{\"field\" : \"id\",\"mode\" :2,\"val\" : [ \"333\" ]}]";
	string pdata_del="[{\"field\" : \"id\",\"mode\" :2,\"val\" : [ \"333\" ]}]";
	string query_text  = "(field=name,query=chma,fuzzy=1,searchType=SR)";

	err = ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, "test.bin", 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataIndexAddIdxEntity(phISSCATAINDEX, pdata.c_str());
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSCataIndexEndIdxEntity(phISSCATAINDEX);
	EXPECT_EQ(err, ISS_SUCCESS);

	//创建搜索句柄
	err = ISSCataSearchCreate( &phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 

	std::string srh_result = (char*)ISSCataSearchSync( phISSCATASEARCH, query_text.c_str(), &err);
	EXPECT_EQ(err, ISS_SUCCESS); 
	EXPECT_STRNE(srh_result.c_str(), "");

	DEBUG_TRACE("srh_result:%s", PrtU8(srh_result.c_str()));

	// phISSCATAINDEXObj = v, pdata = v								
	err = ISSCataIndexDelIdxEntity( phISSCATAINDEX, pdata_del.c_str());
	EXPECT_EQ(err, ISS_SUCCESS); 

	// phISSCATAINDEXObj = v, pdata = v								
	err = ISSCataIndexDelIdxEntity( phISSCATAINDEX, pdata_del.c_str());
	EXPECT_EQ(err, ISS_SUCCESS); 

	// phISSCATAINDEXObj = v, pdata = v								
	err = ISSCataIndexDelIdxEntity( phISSCATAINDEX, pdata_del.c_str());
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSCataIndexEndIdxEntity( phISSCATAINDEX);
	EXPECT_EQ(err, ISS_SUCCESS);

	srh_result = (char*)ISSCataSearchSync( phISSCATASEARCH, query_text.c_str(), &err);
	EXPECT_EQ(err, ISS_ERROR_CATA_SRH_WAR_NO_RESULT); 
	EXPECT_STREQ(srh_result.c_str(), "");

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataSearchDestroy( phISSCATASEARCH);
	EXPECT_EQ(err, ISS_SUCCESS); 
}

TEST_F(CATAFlowTest,SearchSameText3Times){
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

	//创建搜索句柄
	err = ISSCataSearchCreate( &phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 

	std::string srh_result = (char*)ISSCataSearchSync( phISSCATASEARCH, query_text.c_str(), &err);
	EXPECT_EQ(err, ISS_SUCCESS); 
	EXPECT_STRNE(srh_result.c_str(), "");

	srh_result = (char*)ISSCataSearchSync( phISSCATASEARCH, query_text.c_str(), &err);
	EXPECT_EQ(err, ISS_SUCCESS); 
	EXPECT_STRNE(srh_result.c_str(), "");

	srh_result = (char*)ISSCataSearchSync( phISSCATASEARCH, query_text.c_str(), &err);
	EXPECT_EQ(err, ISS_SUCCESS); 
	EXPECT_STRNE(srh_result.c_str(), "");

	DEBUG_TRACE("srh_result:%s", PrtU8(srh_result.c_str()));

	err = ISSCataSearchDestroy( phISSCATASEARCH);
	EXPECT_EQ(err, ISS_SUCCESS); 
}
