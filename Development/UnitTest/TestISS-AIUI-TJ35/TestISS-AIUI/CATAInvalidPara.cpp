#include "TestApiCATA.h"
#include "VAInc.h"
#include "test_log.h"

#include <cstdio>

using namespace VA;
using std::string;

class CATAInvalidParaTest : public ::testing::Test {
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

	}
};

TEST_F(CATAInvalidParaTest,IndexCreateHandleNULL){

	err = ISSCataIndexCreateEx(NULL, def.resDir, "test.bin", ISS_CATA_LANG_ENGLISH, 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

}

TEST_F(CATAInvalidParaTest,IndexCreateResDirNULL){

	err = ISSCataIndexCreateEx(&phISSCATAINDEX, NULL, "test.bin", ISS_CATA_LANG_ENGLISH, 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

}

TEST_F(CATAInvalidParaTest,IndexCreateResDirNULLStr){

	err = ISSCataIndexCreateEx(&phISSCATAINDEX, "", "test.bin", ISS_CATA_LANG_ENGLISH, 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

}

TEST_F(CATAInvalidParaTest,IndexCreateResDirLongStr){

	err = ISSCataIndexCreateEx(&phISSCATAINDEX, def.resDir, "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq", 
		ISS_CATA_LANG_ENGLISH, 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

}

TEST_F(CATAInvalidParaTest,IndexCreateResDirUniStr){

	err = ISSCataIndexCreateEx(&phISSCATAINDEX, def.resDir, "!@#$%^&*()_+.\\ `~<>/", ISS_CATA_LANG_ENGLISH, 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_ERROR_FILE_NOT_FOUND, err);

}

TEST_F(CATAInvalidParaTest,IndexCreateLangTypeInvalid){

	err = ISSCataIndexCreateEx(&phISSCATAINDEX, def.resDir, "test.bin", 1111, 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

}

TEST_F(CATAInvalidParaTest,IndexCreateCallbackNULL){

	err = ISSCataIndexCreateEx(&phISSCATAINDEX, def.resDir, "test.bin", ISS_CATA_LANG_ENGLISH, 1, NULL, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

}

TEST_F(CATAInvalidParaTest,IndexAddHandleNULL){
	string    pdata="[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"chma\" ]}]";
	string query_text  = "(field=name,query=chma,fuzzy=1,searchType=KEYBOARD)";

	err = ISSCataIndexCreateEx(&phISSCATAINDEX, def.resDir, "test.bin", ISS_CATA_LANG_MANDARIN, 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataIndexAddIdxEntity(NULL, pdata.c_str());
	EXPECT_EQ(err, ISS_ERROR_INVALID_PARA); 

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

}

TEST_F(CATAInvalidParaTest,IndexAddWrongData){
	string    pdata="[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"chma\" ]}]";

	err = ISSCataIndexCreateEx(&phISSCATAINDEX, def.resDir, "test.bin", ISS_CATA_LANG_MANDARIN, 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataIndexAddIdxEntity(phISSCATAINDEX, pdata.c_str());
	EXPECT_EQ(err, ISS_ERROR_INVALID_PARA); 

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

}

TEST_F(CATAInvalidParaTest,IndexDelHandleNULL){

	string   pdata="[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"chma\" ]}]";

	err = ISSCataIndexCreateEx(&phISSCATAINDEX, def.resDir, "test.bin", ISS_CATA_LANG_MANDARIN, 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataIndexDelIdxEntity(NULL, pdata.c_str());
	EXPECT_EQ(err, ISS_ERROR_INVALID_PARA); 

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

}

TEST_F(CATAInvalidParaTest,IndexDelWrongData){

	string   pdata="[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"chma\" ]}]";
	
	err = ISSCataIndexCreateEx(&phISSCATAINDEX, def.resDir, "test.bin", ISS_CATA_LANG_MANDARIN, 1, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataIndexDelIdxEntity(phISSCATAINDEX, pdata.c_str());
	EXPECT_EQ(err, ISS_ERROR_INVALID_PARA); 

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

}

TEST_F(CATAInvalidParaTest,IndexDropRes){

	err = ISSCataIndexCreateEx(&phISSCATAINDEX, def.resDir, "test.bin", ISS_CATA_LANG_MANDARIN, 0, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataIndexDropRes( phISSCATAINDEX);
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSCataIndexDropRes( NULL);
	EXPECT_EQ(err, ISS_ERROR_INVALID_PARA); 

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

}

TEST_F(CATAInvalidParaTest,IndexDestroy){

	err = ISSCataIndexCreateEx(&phISSCATAINDEX, def.resDir, "test.bin", ISS_CATA_LANG_MANDARIN, 0, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSCataIndexDestroy( NULL);
	EXPECT_EQ(err, ISS_ERROR_INVALID_PARA); 

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	EXPECT_EQ(ISS_SUCCESS, err);

}

TEST_F(CATAInvalidParaTest , SearchCreate){

	string res_wrong = "/res/cata";

	// phISSCATASEARCHObj = NULL
	err = ISSCataSearchCreate( NULL, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_ERROR_INVALID_PARA); 
	
	//szResourceDir=NULL
	err = ISSCataSearchCreate( &phISSCATASEARCH, NULL, "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_ERROR_INVALID_PARA); 
	
	//szResourceDir=iv("")	
	err = ISSCataSearchCreate( &phISSCATASEARCH, "", "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_ERROR_INVALID_PARA); 

	//szResourceDir=iv(错误路径)
	err = ISSCataSearchCreate( &phISSCATASEARCH, res_wrong.c_str(), "test.bin", DefCATA::onCataMsgProc, NULL);				//未加载索引资源也会返回0
	EXPECT_EQ(err, ISS_ERROR_FILE_NOT_FOUND); 
	
	//szIndexResName=NULL
	err = ISSCataSearchCreate( &phISSCATASEARCH, def.resDir, NULL, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_ERROR_INVALID_PARA); 
	
	//szIndexResName=iv("")
	err = ISSCataSearchCreate( &phISSCATASEARCH, def.resDir, "", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_ERROR_INVALID_PARA);
	
	//	
	err = ISSCataSearchCreate( &phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_SUCCESS);

	err = ISSCataSearchDestroy( phISSCATASEARCH);
	EXPECT_EQ(err, ISS_SUCCESS); 

}

TEST_F(CATAInvalidParaTest,SearchSync){

	string pdata="[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"chma\" ]}]";
	string query_text  = "(field=name,query=chma,fuzzy=1,searchType=KEYBOARD)";
	string srh_result = "";

	err = ISSCataIndexCreate( &phISSCATAINDEX,  def.resDir, "test.bin", 0, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 
	
	err = ISSCataIndexDropRes( phISSCATAINDEX);
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSCataIndexAddIdxEntity( phISSCATAINDEX, pdata.c_str());
	EXPECT_EQ(err, ISS_SUCCESS); 
	
	err = ISSCataIndexEndIdxEntity( phISSCATAINDEX);
	EXPECT_EQ(err, ISS_SUCCESS); 
	
	err = ISSCataIndexDestroy( phISSCATAINDEX);
	EXPECT_EQ(err, ISS_SUCCESS); 
	

	//创建搜索句柄
	err = ISSCataSearchCreate( &phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 
	
	//hISSCATASEARCHObj=NULL
	srh_result = (char*)ISSCataSearchSync( NULL, query_text.c_str(), &err);
	EXPECT_EQ(err, ISS_ERROR_INVALID_HANDLE);
	EXPECT_STREQ(srh_result.c_str(), "");

	//szQuery=NULL
	srh_result = (char*)ISSCataSearchSync( phISSCATASEARCH, NULL, &err);
	EXPECT_EQ(err, ISS_ERROR_CATA_SRH_PARA_VALID);
	EXPECT_STREQ(srh_result.c_str(), "");

	//szQuery=iv("")
	srh_result = (char*)ISSCataSearchSync( phISSCATASEARCH, "", &err);
	EXPECT_EQ(err, ISS_ERROR_CATA_SRH_PARA_VALID);
	EXPECT_STREQ(srh_result.c_str(), "");

	//pErr=NULL
	srh_result = (char*)ISSCataSearchSync( phISSCATASEARCH, query_text.c_str(), NULL);
	EXPECT_STRNE(srh_result.c_str(), "");

	//hISSCATASEARCHObj=v,szQuery=v,pErr=v
	srh_result = (char*)ISSCataSearchSync( phISSCATASEARCH, query_text.c_str(), &err);
	EXPECT_STRNE(srh_result.c_str(), "");
	EXPECT_EQ(err, ISS_SUCCESS); 
	DEBUG_TRACE("srh_result %s", srh_result.c_str());

	err = ISSCataSearchDestroy( phISSCATASEARCH);
	EXPECT_EQ(err, ISS_SUCCESS); 

}

TEST_F(CATAInvalidParaTest,SearchAsync){

	string pdata="[{\"field\" : \"name\",\"mode\" : 1,\"val\" : [ \"chma\" ]}]";
	string query_text  = "(field=name,query=chma,fuzzy=1,searchType=KEYBOARD)";
	string srh_result = "";

	err = ISSCataIndexCreate( &phISSCATAINDEX,  def.resDir, "test.bin", 0, DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSCataIndexDropRes( phISSCATAINDEX);
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSCataIndexAddIdxEntity( phISSCATAINDEX, pdata.c_str());
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSCataIndexEndIdxEntity( phISSCATAINDEX);
	EXPECT_EQ(err, ISS_SUCCESS); 

	err = ISSCataIndexDestroy( phISSCATAINDEX);
	EXPECT_EQ(err, ISS_SUCCESS); 


	//创建搜索句柄
	err = ISSCataSearchCreate( &phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 

	//hISSCATASEARCHObj=NULL
	err = ISSCataSearchAsync( NULL, query_text.c_str());
	EXPECT_EQ(err, ISS_ERROR_INVALID_HANDLE);

	//szQuery=NULL
	err = ISSCataSearchAsync( phISSCATASEARCH, NULL);
	EXPECT_EQ(err, ISS_ERROR_CATA_SRH_PARA_VALID);

	//szQuery=iv("")
	err = ISSCataSearchAsync( phISSCATASEARCH, "");
	EXPECT_EQ(err, ISS_ERROR_CATA_SRH_PARA_VALID);

	//hISSCATASEARCHObj=v,szQuery=v
	err = ISSCataSearchAsync( phISSCATASEARCH, query_text.c_str());
	EXPECT_EQ(err, ISS_SUCCESS); 

	int count = 0;
	while ( !def.msgSearchResult && count<10)
	{
		sleep(10);
		count ++ ;
	}
	if(def.msgSearchResult){
		srh_result = (const char*)(def.msgProcInfo.getlParam());
		DEBUG_TRACE("srh_result = %s.", srh_result.c_str());
	}
	
	EXPECT_STRNE(srh_result.c_str(), "");
	EXPECT_EQ(def.msgSearchResult, 1); 

	err = ISSCataSearchDestroy( phISSCATASEARCH);
	EXPECT_EQ(err, ISS_SUCCESS); 

}

TEST_F(CATAInvalidParaTest,SetParam){
	//创建搜索句柄
	err = ISSCataSearchCreate( &phISSCATASEARCH, def.resDir, "test.bin", DefCATA::onCataMsgProc, NULL);
	EXPECT_EQ(err, ISS_SUCCESS); 

	//hISSCATASEARCHObj=NULL,pmType=v,val=v
	err = ISSCataSetParam(NULL,ISS_CATA_OUTPUT_LIMIT,1);
	EXPECT_EQ(err, ISS_ERROR_INVALID_HANDLE); 
	
	//hISSCATASEARCHObj=v,pmType=NULL,val=v
	err = ISSCataSetParam(phISSCATASEARCH,NULL,1);
	EXPECT_EQ(err, ISS_ERROR_INVALID_PARA); 

	//hISSCATASEARCHObj=v,pmType=iv(3),val=v
	err = ISSCataSetParam(phISSCATASEARCH,3,1);
	EXPECT_EQ(err, ISS_ERROR_INVALID_PARA); 

	//hISSCATASEARCHObj=v,pmType=v(2),val=iv(11)
	err = ISSCataSetParam(phISSCATASEARCH,ISS_CATA_OUTPUT_LIMIT,11);
	EXPECT_EQ(err, ISS_SUCCESS); 

	//hISSCATASEARCHObj=v,pmType=v(1),val=iv(2)
	err = ISSCataSetParam(phISSCATASEARCH,ISS_CATA_NEED_PY_CONTAIN,2);
	EXPECT_EQ(err, ISS_SUCCESS); 

	//hISSCATASEARCHObj=v,pmType=v,val=v
	err = ISSCataSetParam(phISSCATASEARCH,ISS_CATA_OUTPUT_LIMIT,1);
	EXPECT_EQ(err, ISS_SUCCESS);

	err = ISSCataSearchDestroy( phISSCATASEARCH);
	EXPECT_EQ(err, ISS_SUCCESS); 
	
}
