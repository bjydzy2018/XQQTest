#ifndef TEST_CATA_CONSISTENT_H_
#define TEST_CATA_CONSISTENT_H_

#include "DefCATA.h"

#include <string>

class TestCATAConsistent {
private:
	ISSErrID err;
	HISSCATAINDEX phISSCATAINDEX;
	HISSCATASEARCH phISSCATASEARCH;

	DefCATA def;

	std::string index_list_dir_;
	std::string search_list_;
	std::string search_ret_;

	void addIndex();
	void search();

public:
	TestCATAConsistent(std::string index_list_dir = testRes_path + "cata/consistent/list/index_in_effect",
		std::string search_list = testRes_path + "cata/consistent/list/srh_all.txt");
	void test(); // ≤‚ ‘»Îø⁄
	void setIndexListDir(std::string index_list_dir) { index_list_dir_ = index_list_dir; };
	void setSearchList(std::string search_list) { search_list_ = search_list; };
};

#endif
