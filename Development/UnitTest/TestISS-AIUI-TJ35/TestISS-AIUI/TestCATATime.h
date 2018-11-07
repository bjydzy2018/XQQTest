#ifndef CATATIME_H_
#define CATATIME_H_

#include "DefCATA.h"

#include <string>

class TestCATATime {
private:
	ISSErrID err;
	HISSCATAINDEX phISSCATAINDEX;
	HISSCATASEARCH phISSCATASEARCH;

	DefCATA def;

	std::string index_res_name_;
	std::string index_list_;
	std::string index_time_ret_;
	std::string search_list_;
	std::string search_time_ret_;

public:
	TestCATATime(std::string index_list = testRes_path + "cata/batch/list/index/music10000.txt",
		std::string search_list = testRes_path + "cata/batch/list/search/navi_sr_left.txt");
	void test(); // ≤‚ ‘»Îø⁄
};

#endif // !CATATIME_H_

