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
	"\\\"val\\\":[\\\"�����\\\"],\\\"mode\\\":1},{\\\"field\\\":\\\"phoneNumber\\\",\\\"val\\\":[\\\"15395160815\\\"],"
	"\\\"mode\\\":1},{\\\"field\\\":\\\"teleOper\\\",\\\"val\\\":[\\\"����\\\"],\\\"mode\\\":1},{\\\"field\\\":\\\"city\\\","
	"\\\"val\\\":[\\\"�Ϸ�\\\"],\\\"mode\\\":1}]\"]}"));
string index_contact_2((const char *)VA::GBKStrToU8Str("{\"type\":\"contact\",\"index\":[\"[{\\\"field\\\":\\\"name\\\","
	"\\\"val\\\":[\\\"�����\\\"],\\\"mode\\\":1},{\\\"field\\\":\\\"phoneNumber\\\",\\\"val\\\":[\\\"15395160815\\\"],"
	"\\\"mode\\\":1},{\\\"field\\\":\\\"teleOper\\\",\\\"val\\\":[\\\"����\\\"],\\\"mode\\\":1},{\\\"field\\\":\\\"city\\\","
	"\\\"val\\\":[\\\"�Ϸ�\\\"],\\\"mode\\\":1}]\", \"[{\\\"field\\\":\\\"name\\\",\\\"val\\\":[\\\"�����\\\"],"
	"\\\"mode\\\":1},{\\\"field\\\":\\\"phoneNumber\\\",\\\"val\\\":[\\\"15755173379\\\"],\\\"mode\\\":1},"
	"{\\\"field\\\":\\\"teleOper\\\",\\\"val\\\":[\\\"�ƶ�\\\"],\\\"mode\\\":1},{\\\"field\\\":\\\"city\\\","
	"\\\"val\\\":[\\\"�Ϸ�\\\"],\\\"mode\\\":1}]\"]}"));

string index_music_4((const char *)VA::GBKStrToU8Str(
	"{\"type\":\"music\",\"index\":["
		"\"[{\\\"field\\\":\\\"song\\\",\\\"val\\\":[\\\"��ʿɽ��\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"artist\\\",\\\"val\\\":[\\\"����Ѹ\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"album\\\",\\\"val\\\":[\\\"What's Going on...?\\\"],\\\"mode\\\":1}]\","
		"\"[{\\\"field\\\":\\\"song\\\",\\\"val\\\":[\\\"Belle\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"artist\\\",\\\"val\\\":[\\\"����\\\",\\\"·��\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"album\\\",\\\"val\\\":[\\\"Beauty and the Beast\\\"],\\\"mode\\\":1}]\","
		"\"[{\\\"field\\\":\\\"song\\\",\\\"val\\\":[\\\"����ת��\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"artist\\\",\\\"val\\\":[\\\"����Ѹ\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"album\\\",\\\"val\\\":[\\\"���˰�\\\"],\\\"mode\\\":1}]\","
		"\"[{\\\"field\\\":\\\"song\\\",\\\"val\\\":[\\\"Beauty and the Beast\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"artist\\\",\\\"val\\\":[\\\"������\\\"],\\\"mode\\\":1},"
		"{\\\"field\\\":\\\"album\\\",\\\"val\\\":[\\\"Beauty and the Beast\\\"],\\\"mode\\\":1}]\""
	"]}"
));

string index_app_1((const char *)VA::GBKStrToU8Str("{\"type\":\"app\",\"index\":[\"[{\\\"field\\\":\\\"name\\\","
	"\\\"val\\\":[\\\"Kindle\\\"],\\\"mode\\\":1}]\"]}"));

// ð������
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

// �Թ���һ����ϵ��������������н�����ж�
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
		EXPECT_STREQ((const char *)VA::GBKStrToU8Str("�Ϸ�"), result[0]["location"]["city"].GetString());

		ASSERT_TRUE(result[0].HasMember("name"));
		EXPECT_STREQ((const char *)VA::GBKStrToU8Str("�����"), result[0]["name"].GetString());

		ASSERT_TRUE(result[0].HasMember("phoneNumber"));
		EXPECT_STREQ((const char *)VA::GBKStrToU8Str("15395160815"), result[0]["phoneNumber"].GetString());

		ASSERT_TRUE(result[0].HasMember("teleOper"));
		EXPECT_STREQ((const char *)VA::GBKStrToU8Str("����"), result[0]["teleOper"].GetString());

		ASSERT_TRUE(result[0].HasMember("score"));
		EXPECT_TRUE(result[0]["score"].GetDouble() > 0.5);
	}
};
INSTANTIATE_TEST_CASE_P(SearchTextInOneIndex, SearchSmokeJudgeSearchRetByOneIndexContact, 
	::testing::Values(string((const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"�����\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}")),
					string((const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"�����\",\"headNum\":\"153\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}")),
					string((const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"�����\",\"tailNum\":\"0815\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}")),
					string((const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"�����\",\"teleOperator\":\"����\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}")),
					string((const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"�����\",\"operation\":\"DIAL\",\"location\":{\"cityAddr\":\"�Ϸ�\"}}},\"service\":\"telephone\"}"))));

// SEARCH_API_SMOKE_0001
// �������ء�ж��������Դ
TEST_F(SearchSmoke, InitUninitCH) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_SMOKE_0001
// �������ء�ж��Ӣ����Դ
TEST_F(SearchSmoke, InitUninitEN) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_ENGLISH);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_SMOKE_3001
// ����create, destroy����
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
// ����create, destroyӢ��
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
// ��������
TEST_F(SearchSmoke, BuildRes) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_contact_1.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_SMOKE_5001~5005
// ����һ��������������
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
// ������������������������
TEST_F(SearchSmoke, SearchByNameInTwoIndex) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_contact_2.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	const char * search_ret;
	err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"�����\",\"headNum\":\"157\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}"), &search_ret);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_SMOKE_5014
// ������������������ҳ����
TEST_F(SearchSmoke, SearchInYellowPage) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	const char * search_ret;
	err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"name\":\"�й��ƶ�\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}"), &search_ret);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);
}

// SEARCH_API_SMOKE_5015
// ����һ���������޸������ļ���
// ��telephoneҵ���nliField2SearchField������test�ֶΣ�{"nliField": "semantic.slots.test","searchField": "name"}����
// Ȼ��������������
TEST_F(SearchSmoke, SearchByNameWithDIYConfig) {
	// �޸������ļ�
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
	err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"test\":\"�����\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"telephone\"}"), &search_ret);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchDestroyInst(phISSSearch);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchUnInit();
	EXPECT_EQ(ISS_SUCCESS, err);

	//boost::filesystem::copy_file(string(def.resDir) + "/libSearchForNli.cfg.bak", string(def.resDir) + "/libSearchForNli.cfg", boost::filesystem::copy_option::overwrite_if_exists);
	VACopyFile((string(def.resDir) + "/libSearchForNli.cfg").c_str(), (string(def.resDir) + "/libSearchForNli.cfg.bak").c_str());
	remove((string(def.resDir) + "/libSearchForNli.cfg.bak").c_str());
}

// �������������������֡�����ר��
TEST_F(SearchSmoke, SearchSingleArtistSingleAlbumBySong) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_music_4.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	const char * search_ret = NULL;
	err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"song\":\"��ʿɽ��\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"musicX\"}"), &search_ret);
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
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("��ʿɽ��"), result[0]["songname"].GetString());

	ASSERT_TRUE(result[0].HasMember("albumname"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("What's Going on...?"), result[0]["albumname"].GetString());

	ASSERT_TRUE(result[0].HasMember("singernames"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("����Ѹ"), result[0]["singernames"][0].GetString());

	ASSERT_TRUE(result[0].HasMember("score"));
	EXPECT_TRUE(result[0]["score"].GetDouble() > 0.5);
}

// �������������������
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
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("����"), result[0]["singernames"][0].GetString());

	ASSERT_TRUE(result[0].HasMember("singernames"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("·��"), result[0]["singernames"][1].GetString());

	ASSERT_TRUE(result[0].HasMember("score"));
	EXPECT_TRUE(result[0]["score"].GetDouble() > 0.5);
}

// �����������������
TEST_F(SearchSmoke, SearchMultiArtistByArtist) {
	err = ISSSearchInit(def.resDir, ISS_SFN_LANG_MANDARIN);
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchBuildRes(index_music_4.c_str());
	EXPECT_EQ(ISS_SUCCESS, err);

	err = ISSSearchCreateInst(&phISSSearch, NULL);
	EXPECT_EQ(ISS_SUCCESS, err);

	const char * search_ret = NULL;
	err = ISSSearchProcess(phISSSearch, (const char *)VA::GBKStrToU8Str("{\"semantic\":{\"slots\":{\"artist\":\"����Ѹ\",\"operation\":\"DIAL\",\"location\":{\"type\":\"LOC_BASIC\"}}},\"service\":\"musicX\"}"), &search_ret);
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
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("��ʿɽ��"), result[0]["songname"].GetString());

	ASSERT_TRUE(result[0].HasMember("albumname"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("What's Going on...?"), result[0]["albumname"].GetString());

	ASSERT_TRUE(result[0].HasMember("singernames"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("����Ѹ"), result[0]["singernames"][0].GetString());

	ASSERT_TRUE(result[0].HasMember("score"));
	EXPECT_TRUE(result[0]["score"].GetDouble() > 0.5);

	ASSERT_TRUE(result[1].IsObject());

	ASSERT_TRUE(result[1].HasMember("songname"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("����ת��"), result[1]["songname"].GetString());

	ASSERT_TRUE(result[1].HasMember("albumname"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("���˰�"), result[1]["albumname"].GetString());

	ASSERT_TRUE(result[1].HasMember("singernames"));
	EXPECT_STREQ((const char *)VA::GBKStrToU8Str("����Ѹ"), result[1]["singernames"][0].GetString());

	ASSERT_TRUE(result[1].HasMember("score"));
	EXPECT_TRUE(result[1]["score"].GetDouble() > 0.5);
}

// ����app
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