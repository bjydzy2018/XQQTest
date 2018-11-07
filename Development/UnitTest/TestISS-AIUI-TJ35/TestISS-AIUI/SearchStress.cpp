#include "TestApiSearch.h"

#ifndef ISS_SR_3

#include "CommonFun.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using std::string;
using namespace rapidjson;

// Ñ¹Á¦²âÊÔ
class SearchStressTest : public ::testing::Test {
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

class SearchStressJudgeSearchRetByOneIndex : public ::testing::Test {
protected:
	ISSErrID err;
	ISSSearchHandle	 phISSSearch;

	DefSearchForNLI def;

	const char * search_ret;
	string search_ret_bak;

	virtual void SetUp() {
		err = -1;
		phISSSearch = NULL;
		search_ret = NULL;
	}

	virtual void TearDown() {
		phISSSearch = NULL;

		ASSERT_STRNE(NULL, search_ret_bak.c_str());

		Document document;
		document.Parse(search_ret_bak.c_str());

		ASSERT_TRUE(document.IsObject());
		ASSERT_TRUE(document.HasMember("data"));

		ASSERT_TRUE(document["data"].IsObject());
		ASSERT_TRUE(document["data"].HasMember("result"));

		Value & result = document["data"]["result"];
		ASSERT_TRUE(result.IsArray());

		ASSERT_TRUE(result[0].IsObject());
		ASSERT_TRUE(result[0].HasMember("location"));
		ASSERT_TRUE(result[0]["location"].HasMember("city"));
		EXPECT_STREQ((const char *)VA::GBKStrToU8Str("ºÏ·Ê"), result[0]["location"]["city"].GetString());

		ASSERT_TRUE(result[0].HasMember("name"));
		EXPECT_STREQ((const char *)VA::GBKStrToU8Str("Áõºé³õ"), result[0]["name"].GetString());

		ASSERT_TRUE(result[0].HasMember("phoneNumber"));
		EXPECT_STREQ((const char *)VA::GBKStrToU8Str("15395160815"), result[0]["phoneNumber"].GetString());

		ASSERT_TRUE(result[0].HasMember("teleOper"));
		EXPECT_STREQ((const char *)VA::GBKStrToU8Str("µçÐÅ"), result[0]["teleOper"].GetString());

		ASSERT_TRUE(result[0].HasMember("score"));
		EXPECT_TRUE(result[0]["score"].GetDouble() > 0.5);
	}
};

// SEARCH_API_STRESS_0000
// UnInit * 1000
TEST_F(SearchStressTest, InitUninit1000) {
	for (int i = 0; i < 1000; i++)
	{
		err = ISSSearchUnInit();
		EXPECT_EQ(ISS_ERROR_INVALID_CALL, err);
	}
}

// SEARCH_API_STRESS_0002
// (Init, Create, Destroy, UnInit) * 1000
TEST_F(SearchStressTest, InitUninitCreateDestroy1000) {
	for (int i = 0; i < 1000; i++)
	{
		err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSSearchCreateInst(&phISSSearch, NULL);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSSearchDestroyInst(phISSSearch);
		EXPECT_EQ(ISS_SUCCESS, err);
		phISSSearch = NULL;

		err = ISSSearchUnInit();
		EXPECT_EQ(ISS_SUCCESS, err);
	}
}

// SEARCH_API_STRESS_0003
// Init, (Create, Destroy) * 1000, UnInit
TEST_F(SearchStressTest, CreateDestroy1000) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	for (int i = 0; i < 1000; i++)
	{
		err = ISSSearchCreateInst(&phISSSearch, NULL);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSSearchDestroyInst(phISSSearch);
		EXPECT_EQ(ISS_SUCCESS, err);
		phISSSearch = NULL;
	}

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_STRESS_0004
// Init, Create, BuildRes) * 1000, Destroy, UnInit
TEST_F(SearchStressTest, BuildRes1000) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	for (int i = 0; i < 1000; i++)
	{
		err = ISSSearchBuildRes(index_contact_1.c_str());
		EXPECT_EQ(ISS_SUCCESS, err);
	}

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_STRESS_0005
// Init, Create, BuildRes, Search * 1000, Destroy, UnInit
TEST_F(SearchStressJudgeSearchRetByOneIndex, Search1000) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_contact_1.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	for (int i = 0; i < 1000; i++)
	{
		err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"Áõºé³õ\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}"), &search_ret);
		EXPECT_EQ(ISS_SUCCESS, err);
	}
	search_ret_bak = string(search_ret);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_STRESS_0006
// (Init, Create, BuildRes, Search, Destroy, UnInit) * 1000
TEST_F(SearchStressJudgeSearchRetByOneIndex, All1000) {
	for (int i = 0; i < 1000; i++)
	{
		err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSSearchCreateInst(&phISSSearch, NULL);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSSearchBuildRes(index_contact_1.c_str());
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"Áõºé³õ\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}"), &search_ret);
		EXPECT_EQ(ISS_SUCCESS, err);
		search_ret_bak = string(search_ret);

		err = ISSSearchDestroyInst(phISSSearch);
		EXPECT_EQ(ISS_SUCCESS, err);

		err = ISSSearchUnInit();
		EXPECT_EQ(ISS_SUCCESS, err);
	}
}

#endif // ISS_SR_3