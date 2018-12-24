#include "TestCATAConsistent.h"
#include "CommonFun.h"
#include "VAInc.h"
#include "test_log.h"

#include <vector>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

using namespace VA;
using std::vector;
using std::string;
using std::endl;
using namespace boost::algorithm;

TestCATAConsistent::TestCATAConsistent(string index_list_dir, string search_list): index_list_dir_(index_list_dir),
						search_list_(search_list), search_ret_(testRes_path + "cata/consistent/srh_ret.txt") {}

void TestCATAConsistent::test() {
	// 构建索引
	addIndex();

	// 搜索
	search();
}

//void TestCATAConsistent::addIndex() {
//	vector<string> index_list_files = getFilesInDir(index_list_dir_);
//	vector<string> index_res_names; // 存储添加的资源名，若是第一次构建某个资源文件，需要先把历史资源清掉
//
//	DEBUG_TRACE("adding index...");
//	BOOST_FOREACH(string index_list_file, index_list_files) {
//		DEBUG_TRACE(("handling index file: " + index_list_file).c_str());
//		vector<string> index_list = readFileByLine(index_list_file, false);
//
//		BOOST_FOREACH(string segValues, index_list) {
//			string segValues_parsed(def.parseSegValues(segValues));
//			string index_res_name(def.getAddBin(segValues));
//
//			bool first_time_to_add = true; // 是否是第一次添加
//			BOOST_FOREACH(string index_res_name_, index_res_names) {
//				if (index_res_name_ == index_res_name) {
//					first_time_to_add = false;
//					break;
//				}
//			}
//
//			// 创建index实例
//			if (first_time_to_add) {
//				ISSCATAAdapter::ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, index_res_name.c_str(), 1, DefCATA::onCataMsgProc, NULL);
//				index_res_names.push_back(index_res_name);
//			}
//			else {
//				//DEBUG_TRACE("index_res_name:%s repeat ", index_res_name.c_str());
//				ISSCATAAdapter::ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, index_res_name.c_str(), 0, DefCATA::onCataMsgProc, NULL);
//			}
//
//			// 添加索引
//			ISSCATAAdapter::ISSCataIndexAddIdxEntity(phISSCATAINDEX, segValues_parsed.c_str());
//			ISSCATAAdapter::ISSCataIndexEndIdxEntity(phISSCATAINDEX);
//			ISSCATAAdapter::ISSCataIndexDestroy(phISSCATAINDEX);
//			phISSCATAINDEX = NULL;
//		}
//	}
//}

void TestCATAConsistent::addIndex() {
	vector<string> index_list_files = getFilesInDir(index_list_dir_);
	vector<string> index_res_names; // 存储添加的资源名，若是第一次构建某个资源文件，需要先把历史资源清掉

	DEBUG_TRACE("adding index...");
	BOOST_FOREACH(string index_list_file, index_list_files) {
		DEBUG_TRACE(("handling index file: " + index_list_file).c_str());
		vector<string> index_list = readFileByLine(index_list_file, false);

		BOOST_FOREACH(string segValues, index_list) {
			string segValues_parsed(def.parseSegValues(segValues));
			string index_res_name(def.getAddBin(segValues));

			bool first_time_to_add = true; // 是否是第一次添加
			BOOST_FOREACH(string index_res_name_, index_res_names) {
				if (index_res_name_ == index_res_name) {
					first_time_to_add = false;
					break;
				}
			}

			// 创建index实例
			if (first_time_to_add) {
				ISSCATAAdapter::ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, index_res_name.c_str(), 1, DefCATA::onCataMsgProc, NULL);
				index_res_names.push_back(index_res_name);
			}
			//else {
			//	//DEBUG_TRACE("index_res_name:%s repeat ", index_res_name.c_str());
			//	ISSCATAAdapter::ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, index_res_name.c_str(), 0, DefCATA::onCataMsgProc, NULL);
			//}

			// 添加索引
			ISSCATAAdapter::ISSCataIndexAddIdxEntity(phISSCATAINDEX, segValues_parsed.c_str());
			
		}
		ISSCATAAdapter::ISSCataIndexEndIdxEntity(phISSCATAINDEX);
		ISSCATAAdapter::ISSCataIndexDestroy(phISSCATAINDEX);
		phISSCATAINDEX = NULL;
	}
}

void TestCATAConsistent::search() {
	DEBUG_TRACE("searching...");
	ofstream fout(search_ret_.c_str());
	vector<string> search_list = readFileByLine(search_list_, false);

	BOOST_FOREACH(string search, search_list) {
		string srh_text = def.getSrhText(search);
		string srh_bin = def.getSrhBin(search);

		DEBUG_TRACE_DEBUG("query = %s, res = %s", PrtU8(srh_text.c_str()), srh_bin.c_str());
		err = ISSCATAAdapter::ISSCataSearchCreate(&phISSCATASEARCH, def.resDir, srh_bin.c_str(), DefCATA::onCataMsgProc, NULL);

		string search_ret = ISSCATAAdapter::ISSCataSearchSync(phISSCATASEARCH, srh_text.c_str(), &err);
		if (err == 0)
			DEBUG_TRACE_DEBUG("ISSCataSearchSync ret: %s", PrtU8(search_ret.c_str()));
		else
			DEBUG_TRACE_DEBUG("ISSCataSearchSync err: %d", err);

		fout << "query = " << srh_text << ",res = " << srh_bin << endl;
		if (err == 0)
			fout << search_ret << endl;
		else
			fout << "ISSCataSearchSync err=" << err << endl << endl;

		ISSCATAAdapter::ISSCataSearchDestroy(phISSCATASEARCH);
		phISSCATASEARCH = NULL;
	}

	fout.close();
}
