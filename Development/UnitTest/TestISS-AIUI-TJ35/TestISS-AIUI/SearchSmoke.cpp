#include "TestApiSearch.h"

#ifndef ISS_SR_3

#include "CommonFun.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <cstdio>
#include "VAFileSystem.h"

using std::string;
using namespace VA;
using namespace rapidjson;

string index_contact_1((const char *)VA::GBKStrToU8Str("{\"type\":\"contact\",\"index\":[\"[{\\\"field\\\":\\\"name\\\","
	"\\\"val\\\":[\\\"刘洪初\\\"],\\\"mode\\\":1},{\\\"field\\\":\\\"phoneNumber\\\",\\\"val\\\":[\\\"15395160815\\\"],"
	"\\\"mode\\\":1},{\\\"field\\\":\\\"teleOper\\\",\\\"val\\\":[\\\"电信\\\"],\\\"mode\\\":1},{\\\"field\\\":\\\"city\\\","
	"\\\"val\\\":[\\\"合肥\\\"],\\\"mode\\\":1}]\"]}"));
string index_contact_2((const char *)VA::GBKStrToU8Str("{\"type\":\"contact\",\"index\":[\"[{\\\"field\\\":\\\"name\\\","
	"\\\"val\\\":[\\\"刘洪初\\\"],\\\"mode\\\":1},{\\\"field\\\":\\\"phoneNumber\\\",\\\"val\\\":[\\\"15395160815\\\"],"
	"\\\"mode\\\":1},{\\\"field\\\":\\\"teleOper\\\",\\\"val\\\":[\\\"电信\\\"],\\\"mode\\\":1},{\\\"field\\\":\\\"city\\\","
	"\\\"val\\\":[\\\"合肥\\\"],\\\"mode\\\":1}]\", \"[{\\\"field\\\":\\\"name\\\",\\\"val\\\":[\\\"刘洪初\\\"],"
	"\\\"mode\\\":1},{\\\"field\\\":\\\"phoneNumber\\\",\\\"val\\\":[\\\"15755173379\\\"],\\\"mode\\\":1},"
	"{\\\"field\\\":\\\"teleOper\\\",\\\"val\\\":[\\\"移动\\\"],\\\"mode\\\":1},{\\\"field\\\":\\\"city\\\","
	"\\\"val\\\":[\\\"合肥\\\"],\\\"mode\\\":1}]\"]}"));

string index_music_4((const char *)VA::GBKStrToU8Str(
	"{\"type\":\"music\",\"index\":["
		"\"[{\\\"field\\\":\\\"song\\\",\\\"val\\\":[\\\"富士山下\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"artist\\\",\\\"val\\\":[\\\"陈奕迅\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"album\\\",\\\"val\\\":[\\\"What's Going on...?\\\"],\\\"mode\\\":1}]\","
		"\"[{\\\"field\\\":\\\"song\\\",\\\"val\\\":[\\\"Belle\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"artist\\\",\\\"val\\\":[\\\"艾玛\\\",\\\"路克\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"album\\\",\\\"val\\\":[\\\"Beauty and the Beast\\\"],\\\"mode\\\":1}]\","
		"\"[{\\\"field\\\":\\\"song\\\",\\\"val\\\":[\\\"爱情转移\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"artist\\\",\\\"val\\\":[\\\"陈奕迅\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"album\\\",\\\"val\\\":[\\\"认了吧\\\"],\\\"mode\\\":1}]\","
		"\"[{\\\"field\\\":\\\"song\\\",\\\"val\\\":[\\\"Beauty and the Beast\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"artist\\\",\\\"val\\\":[\\\"爱莉安娜\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"album\\\",\\\"val\\\":[\\\"Beauty and the Beast\\\"],\\\"mode\\\":1}]\""
	"]}"
));

string index_app_1((const char *)VA::GBKStrToU8Str("{\"type\":\"app\",\"index\":[\"[{\\\"field\\\":\\\"name\\\","
	"\\\"val\\\":[\\\"Kindle\\\"],\\\"mode\\\":1}]\"]}"));

// 冒烟用例
class SearchSmoke : public ::testing::Test {
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

		remove((string(def.resDir) + "/CataRes/appForNli.bin").c_str());
		remove((string(def.resDir) + "/CataRes/contactForNli.bin").c_str());
		remove((string(def.resDir) + "/CataRes/musicForNli.bin").c_str());
	}
};

// 对构建一条联系人索引的情况进行结果的判断
class SearchSmokeJudgeSearchRetByOneIndexContact : public ::testing::TestWithParam<string> {
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
		remove((string(def.resDir) + "/CataRes/appForNli.bin").c_str());
		remove((string(def.resDir) + "/CataRes/contactForNli.bin").c_str());
		remove((string(def.resDir) + "/CataRes/musicForNli.bin").c_str());

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
		EXPECT_STREQ((const char *)VA::GBKStrToU8Str("合肥"), result[0]["location"]["city"].GetString());

		ASSERT_TRUE(result[0].HasMember("name"));
		EXPECT_STREQ((const char *)VA::GBKStrToU8Str("刘洪初"), result[0]["name"].GetString());

		ASSERT_TRUE(result[0].HasMember("phoneNumber"));
		EXPECT_STREQ((const char *)VA::GBKStrToU8Str("15395160815"), result[0]["phoneNumber"].GetString());

		ASSERT_TRUE(result[0].HasMember("teleOper"));
		EXPECT_STREQ((const char *)VA::GBKStrToU8Str("电信"), result[0]["teleOper"].GetString());

		ASSERT_TRUE(result[0].HasMember("score"));
		EXPECT_TRUE(result[0]["score"].GetDouble() > 0.5);
	}
};
INSTANTIATE_TEST_CASE_P(SearchTextInOneIndex, SearchSmokeJudgeSearchRetByOneIndexContact, 
	::testing::Values(string((const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"刘洪初\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}")),
					string((const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"刘洪初\",\"headNum\":\"153\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}")),
					string((const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"刘洪初\",\"tailNum\":\"0815\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}")),
					string((const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"刘洪初\",\"teleOperator\":\"电信\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}")),
					string((const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"刘洪初\",\"operation\":\"DIAL\",\"location\":{\"cityAddr\":\"合肥\"}}},\"service\":\"telephone\"}"))));

// SEARCH_API_SMOKE_0001
// 正常加载、卸载中文资源
TEST_F(SearchSmoke, InitUninitCH) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_SMOKE_0001
// 正常加载、卸载英文资源
TEST_F(SearchSmoke, InitUninitEN) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_ENGLISH);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_SMOKE_3001
// 正常create, destroy中文
TEST_F(SearchSmoke, CreateDestroyCH) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_SMOKE_3002
// 正常create, destroy英文
TEST_F(SearchSmoke, CreateDestroyEN) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_ENGLISH);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_SMOKE_2001
// 正常构建
TEST_F(SearchSmoke, BuildRes) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_contact_1.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_SMOKE_5001~5005
// 构建一条索引，并搜索
TEST_P(SearchSmokeJudgeSearchRetByOneIndexContact, SearchByNameInOneIndex) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_contact_1.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);


	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	DEBUG_TRACE("ISSSearchProcess");
	err = ISSSearchProcess(phISSSearch, GetParam().c_str(), &search_ret);
	EXPECT_EQ(ISS_SUCCESS, err);
	search_ret_bak = string(search_ret);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);
	
	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_SMOKE_5006
// 构建两条索引，按姓名搜索
TEST_F(SearchSmoke, SearchByNameInTwoIndex) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_contact_2.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	const char * search_ret;
	err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"刘洪初\",\"headNum\":\"157\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}"), &search_ret);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_SMOKE_5014
// 不构建索引，搜索黄页内容
TEST_F(SearchSmoke, SearchInYellowPage) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	const char * search_ret;
	err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"中国移动\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}"), &search_ret);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_SMOKE_5015
// 构建一条索引，修改配置文件，
// 在telephone业务的nliField2SearchField中增加test字段（{"nliField": "semantic.slots.test","searchField": "name"}），
// 然后按姓名进行搜索
TEST_F(SearchSmoke, SearchByNameWithDIYConfig) {
	// 修改配置文件
	//boost::filesystem::copy_file(string(def.resDir) + "/libSearchForNli.cfg", string(def.resDir) + "/libSearchForNli.cfg.bak", boost::filesystem::copy_option::overwrite_if_exists);
	VACopyFile((string(def.resDir) + "/libSearchForNli.cfg.bak").c_str(), (string(def.resDir) + "/libSearchForNli.cfg").c_str());
	string cfg_data = readFile(string(def.resDir) + "/libSearchForNli.cfg", false);
	Document document;
	document.Parse(cfg_data.c_str());
	Document::AllocatorType& allocator = document.GetAllocator();

	Value test(kObjectType);
	test.AddMember("nliField", "semantic.slots.test", allocator);
	test.AddMember("searchField", "name", allocator);
	document["CataCfg"][0]["searchCfg"][0]["nliField2SearchField"].PushBack(test, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	document.Accept(writer);
	ofstream fout((string(def.resDir) + "/libSearchForNli.cfg").c_str());
	fout << buffer.GetString();
	fout.close();

	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_contact_1.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	const char * search_ret;
	err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"test\":\"刘洪初\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}"), &search_ret);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);

	//boost::filesystem::copy_file(string(def.resDir) + "/libSearchForNli.cfg.bak", string(def.resDir) + "/libSearchForNli.cfg", boost::filesystem::copy_option::overwrite_if_exists);
	VACopyFile((string(def.resDir) + "/libSearchForNli.cfg").c_str(), (string(def.resDir) + "/libSearchForNli.cfg.bak").c_str());
	remove((string(def.resDir) + "/libSearchForNli.cfg.bak").c_str());
}

// 按歌曲名搜索单个歌手、单个专辑
TEST_F(SearchSmoke, SearchSingleArtistSingleAlbumBySong) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_music_4.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	const char * search_ret = NULL;
	err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"song\":\"富士山下\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"musicX\"}"), &search_ret);
	EXPECT_EQ(ISS_SUCCESS, err);

	string search_ret_bak = string(search_ret);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);

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

	ASSERT_TRUE(result[0].HasMember("songname"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("富士山下"), result[0]["songname"].GetString());

	ASSERT_TRUE(result[0].HasMember("albumname"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("What's Going on...?"), result[0]["albumname"].GetString());

	ASSERT_TRUE(result[0].HasMember("singernames"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("陈奕迅"), result[0]["singernames"][0].GetString());

	ASSERT_TRUE(result[0].HasMember("score"));
	EXPECT_TRUE(result[0]["score"].GetDouble() > 0.5);
}

// 按歌曲名搜索多个歌手
TEST_F(SearchSmoke, SearchMultiArtistBySong) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_music_4.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	const char * search_ret = NULL;
	err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"song\":\"Belle\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"musicX\"}"), &search_ret);
	EXPECT_EQ(ISS_SUCCESS, err);

	string search_ret_bak = string(search_ret);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);

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

	ASSERT_TRUE(result[0].HasMember("songname"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("Belle"), result[0]["songname"].GetString());

	ASSERT_TRUE(result[0].HasMember("albumname"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("Beauty and the Beast"), result[0]["albumname"].GetString());

	ASSERT_TRUE(result[0].HasMember("singernames"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("艾玛"), result[0]["singernames"][0].GetString());

	ASSERT_TRUE(result[0].HasMember("singernames"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("路克"), result[0]["singernames"][1].GetString());

	ASSERT_TRUE(result[0].HasMember("score"));
	EXPECT_TRUE(result[0]["score"].GetDouble() > 0.5);
}

// 按歌手搜索多个歌曲
TEST_F(SearchSmoke, SearchMultiArtistByArtist) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_music_4.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	const char * search_ret = NULL;
	err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"artist\":\"陈奕迅\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"musicX\"}"), &search_ret);
	EXPECT_EQ(ISS_SUCCESS, err);

	string search_ret_bak = string(search_ret);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);

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

	ASSERT_TRUE(result[0].HasMember("songname"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("富士山下"), result[0]["songname"].GetString());

	ASSERT_TRUE(result[0].HasMember("albumname"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("What's Going on...?"), result[0]["albumname"].GetString());

	ASSERT_TRUE(result[0].HasMember("singernames"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("陈奕迅"), result[0]["singernames"][0].GetString());

	ASSERT_TRUE(result[0].HasMember("score"));
	EXPECT_TRUE(result[0]["score"].GetDouble() > 0.5);

	ASSERT_TRUE(result[1].IsObject());

	ASSERT_TRUE(result[1].HasMember("songname"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("爱情转移"), result[1]["songname"].GetString());

	ASSERT_TRUE(result[1].HasMember("albumname"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("认了吧"), result[1]["albumname"].GetString());

	ASSERT_TRUE(result[1].HasMember("singernames"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("陈奕迅"), result[1]["singernames"][0].GetString());

	ASSERT_TRUE(result[1].HasMember("score"));
	EXPECT_TRUE(result[1]["score"].GetDouble() > 0.5);
}

// 搜索app
TEST_F(SearchSmoke, SearchAPP) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_app_1.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	const char * search_ret = NULL;
	err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"Kindle\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"app\"}"), &search_ret);
	EXPECT_EQ(ISS_SUCCESS, err);

	string search_ret_bak = string(search_ret);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);

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

	ASSERT_TRUE(result[0].HasMember("name"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("Kindle"), result[0]["name"].GetString());

	ASSERT_TRUE(result[0].HasMember("score"));
	EXPECT_TRUE(result[0]["score"].GetDouble() > 0.5);
}

#endif // !ISS_SR_3