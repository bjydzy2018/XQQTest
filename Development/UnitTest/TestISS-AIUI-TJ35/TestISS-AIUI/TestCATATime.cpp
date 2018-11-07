#include "TestCATATime.h"
#include "CommonFun.h"
#include "test_log.h"
#include "TimerForTest.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace VA;
using std::endl;
using std::vector;
using std::string;

TestCATATime::TestCATATime(string index_list,string search_list):err(-1), phISSCATAINDEX(NULL), phISSCATASEARCH(NULL),
		index_res_name_("test_time.bin"), index_list_(index_list), index_time_ret_("TestRes/cata/batch/time/addindex.csv"),
		search_list_(search_list), search_time_ret_("TestRes/cata/batch/time/search.csv") {
	DEBUG_TRACE_INFOR("create index res name: test_time.bin");

	DEBUG_TRACE_INFOR(("index list path: " + index_list).c_str());
	DEBUG_TRACE_INFOR("index time ret path: TestRes/cata/batch/time/addindex.csv");

	DEBUG_TRACE_INFOR(("search list path: " + search_list).c_str());
	DEBUG_TRACE_INFOR("search time ret path: TestRes/cata/batch/time/search.csv");
}

void TestCATATime::test() {
	ofstream fout_index(index_time_ret_.c_str());
	vector<string> index_list = readFileByLine(index_list_, false);

	fout_index << "index_time,err_add_del,text\n";

	err = ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, index_res_name_.c_str(), 1, DefCATA::onCataMsgProc, NULL);
	DEBUG_TRACE("ISSCataIndexCreate return %d",err);

	int add_num = 1;

	TimerForTest t;
	for(size_t i = 0; i < index_list.size(); ++i){
		string segValues_parsed(def.parseSegValues(index_list[i]));
		DEBUG_TRACE(("adding segValues: " + segValues_parsed).c_str());
		
		const char * segValues_parsed_char = segValues_parsed.c_str();
		t.restart();
		err = ISSCataIndexAddIdxEntity(phISSCATAINDEX, segValues_parsed_char);
		//double add_time = t.elapsed() * 1000;
		double add_time = t.elapsed();
		DEBUG_TRACE("ISSCataIndexAddIdxEntity return %d",err);

		fout_index << add_time << "," << err << endl;
	}

	err = ISSCataIndexEndIdxEntity(phISSCATAINDEX);
	DEBUG_TRACE("ISSCataIndexEndIdxEntity return %d",err);

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	phISSCATAINDEX = NULL;
	DEBUG_TRACE("ISSCataIndexDestroy return %d",err);

	fout_index.close();

	DEBUG_TRACE("sleeping 30000 ms");
	sleep(30000);

	ofstream fout_search(search_time_ret_.c_str());
	vector<string> search_list = readFileByLine(search_list_, false);

	fout_search << "loadRes_time,srh_time,err\n";

	t.restart();
	err = ISSCataSearchCreate(&phISSCATASEARCH, def.resDir, (index_res_name_ + ",alias,yellow_page,pinyin_fuzzy").c_str(), DefCATA::onCataMsgProc, NULL);
	double search_create_time = t.elapsed();
	DEBUG_TRACE("ISSCataSearchCreate return %d",err);

	//for(int j =0;j<150;j++){
		for(size_t i = 0; i < search_list.size(); ++i){
		vector<string> search_v = split(search_list[i], '\t');

		//t.restart();
		//err = ISSCataSearchCreate(&phISSCATASEARCH, def.resDir, (index_res_name_ + ",alias,yellow_page,pinyin_fuzzy").c_str(), DefCATA::onCataMsgProc, NULL);
		//double search_create_time = t.elapsed();
		//DEBUG_TRACE("ISSCataSearchCreate return %d",err);

		const char * search_char = search_v[1].c_str();
		DEBUG_TRACE(("search text: " + search_v[1]).c_str());

		t.restart();
		string search_ret = ISSCataSearchSync(phISSCATASEARCH, search_char, &err);
		double search_time = t.elapsed();
		DEBUG_TRACE("ISSCataSearchSync return %d",err);
		DEBUG_TRACE(("search ret: " + search_ret).c_str());

		fout_search << search_create_time << "," << search_time << "," << err << endl;

		/*err = ISSCataSearchDestroy(phISSCATASEARCH);
		phISSCATASEARCH = NULL;
		DEBUG_TRACE("ISSCataSearchDestroy return %d",err);*/
	}

	//}
	
	err = ISSCataSearchDestroy(phISSCATASEARCH);
	phISSCATASEARCH = NULL;
	DEBUG_TRACE("ISSCataSearchDestroy return %d",err);

	fout_search.close();
}
