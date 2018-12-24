#include "TestApiSearch.h"

#ifndef ISS_SR_3

#include "CommonFun.h"

using std::string;

// 无效参数
class SearchInvalidPara : public ::testing::Test {
protected:
	ISSErrID err;
	ISSSearchHandle	 phISSSearch;

	DefSearchForNLI def;

	const char * search_ret;

	virtual void SetUp() {
		err = -1;
		phISSSearch = NULL;
		search_ret = NULL;
	}

	virtual void TearDown() {
		phISSSearch = NULL;
	}
};

class SearchInvalidParaGenResdir : public ::testing::TestWithParam<string> {
protected:
	ISSErrID err;
	ISSSearchHandle	 phISSSearch;

	DefSearchForNLI def;

	virtual void SetUp() {
		err = -1;
		phISSSearch = NULL;
	}

	virtual void TearDown() {
		phISSSearch = NULL;
	}
};
INSTANTIATE_TEST_CASE_P(GenResdir, SearchInvalidParaGenResdir, ::testing::Values(".","asdf",""));

class SearchInvalidParaGenBuildResText : public ::testing::TestWithParam<string> {
protected:
	ISSErrID err;
	ISSSearchHandle	 phISSSearch;

	DefSearchForNLI def;

	virtual void SetUp() {
		err = -1;
		phISSSearch = NULL;
	}

	virtual void TearDown() {
		phISSSearch = NULL;
	}
};
INSTANTIATE_TEST_CASE_P(GenBuildResText, SearchInvalidParaGenBuildResText,
	::testing::Values("asdf", 
		"",
		string((const char *)VA::GBKStrToU8Str("{\"type\":\"contact\",\"index\":[\"[{\\\"field\\\":\\\"name\\\", \\\"val\\\" : [\\\"刘洪初\\\"], \\\"mode\\\" : 1}]\"]")),
		string((const char *)VA::GBKStrToU8Str("{\"index\":[\"[{\\\"field\\\":\\\"name\\\", \\\"val\\\" : [\\\"刘洪初\\\"], \\\"mode\\\" : 1}]\"]}")),
		string((const char *)VA::GBKStrToU8Str("{\"type\":\"contact\"}")),
		string((const char *)VA::GBKStrToU8Str("{\"type\":\"contact*\",\"index\":[\"[{\\\"field\\\":\\\"name\\\", \\\"val\\\" : [\\\"刘洪初\\\"], \\\"mode\\\" : 1}]\"]}")),
		string((const char *)VA::GBKStrToU8Str("{\"type\":15451665484515454,\"index\":[\"[{\\\"field\\\":\\\"name\\\", \\\"val\\\" : [\\\"刘洪初\\\"], \\\"mode\\\" : 1}]\"]}")),
		string((const char *)VA::GBKStrToU8Str("{\"type\":[\"contact\"],\"index\":[\"[{\\\"field\\\":\\\"name\\\", \\\"val\\\" : [\\\"刘洪初\\\"], \\\"mode\\\" : 1}]\"]")),
		string((const char *)VA::GBKStrToU8Str("{\"type\":\"contact\",\"index\":{\"field\":\"name\", \"val\" : [\"刘洪初\"], \"mode\" : 1}}")),
		string((const char *)VA::GBKStrToU8Str("{\"type\":\"contact\",\"index\":\"[{\\\"field\\\":\\\"name\\\", \\\"val\\\" : [\\\"刘洪初\\\"], \\\"mode\\\" : 1}]\"}")),
		string((const char *)VA::GBKStrToU8Str("{\"type\":\"contact\",\"index\":[\"\"]}"))));

// SEARCH_API_INVALID_PARA_0001~0003
// 资源路径无效
TEST_P(SearchInvalidParaGenResdir, InitResdirInvalid) {
	err = ISSSearchInit(GetParam().c_str(), ISS_SFN_LANG_MANDARIN);
	EXPECT_NE(ISS_SUCCESS, err);
}

// SEARCH_API_INVALID_PARA_0004
// 资源路径传空
TEST_F(SearchInvalidPara, InitResdirNULL) {
	err = ISSSearchInit(NULL, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);
}

// SEARCH_API_INVALID_PARA_0005
// 语种传无效值
TEST_F(SearchInvalidPara, InitLangInvalid) {
	err = ISSSearchInit(def.resDir, 4);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);
}

// SEARCH_API_INVALID_PARA_3001
// create句柄传空
TEST_F(SearchInvalidPara, CreateHandleNULL) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_ENGLISH);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(NULL, NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_INVALID_PARA_3002
// create参数传任意字符串
TEST_F(SearchInvalidPara, CreateParamInvalid) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_ENGLISH);
	EXPECT_EQ(ISS_SUCCESS, err);

	char param[] = { "asdf" };
	err = ISSSearchCreateInst(&phISSSearch, param);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_INVALID_PARA_4001
// destroy句柄传空
TEST_F(SearchInvalidPara, DestroyHandleNULL) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_ENGLISH);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchDestroyInst(NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_HANDLE, err);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_INVALID_PARA_2001
// BuildRes接口szInput传空
TEST_F(SearchInvalidPara, BuildResNULL) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_ENGLISH);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(NULL);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_INVALID_PARA_2002~2012
// BuildRes接口szInput传无效字符串
TEST_P(SearchInvalidParaGenBuildResText, BuildResInvalid) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_ENGLISH);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(GetParam().c_str());
	EXPECT_NE(ISS_SUCCESS, err);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_INVALID_PARA_5001
// 构建一条索引，搜索的语义中不包含约定字段
TEST_F(SearchInvalidPara, SearchWithoutValidSlots) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_contact_1.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"nam\":\"刘洪初\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}"), &search_ret);
	EXPECT_EQ(ISS_ERROR_INVALID_PARA, err);

	string errcode = def.getSearchErrorCode(search_ret);
	ASSERT_STREQ("10106", errcode.c_str());

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_INVALID_PARA_5002
// 构建一条索引，搜索的语义中不包含有效语义
TEST_F(SearchInvalidPara, SearchNULLStr) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_contact_1.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchProcess(phISSSearch, "{}", &search_ret);
	EXPECT_EQ(ISS_ERROR_INVALID_JSON_INFO, err);

	string errcode = def.getSearchErrorCode(search_ret);
	ASSERT_STREQ("10002", errcode.c_str());

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_INVALID_PARA_5003
// 搜索的json格式错误
TEST_F(SearchInvalidPara, SearchJSONInvalid) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_contact_1.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"刘洪初\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}},\"service\":\"telephone\"}"), &search_ret);
	EXPECT_EQ(ISS_ERROR_INVALID_JSON_FMT, err);

	string errcode = def.getSearchErrorCode(search_ret);
	EXPECT_STREQ("10001", errcode.c_str());

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

#endif // ISS_SR_3