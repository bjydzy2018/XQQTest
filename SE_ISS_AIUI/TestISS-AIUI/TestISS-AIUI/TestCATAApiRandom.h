#ifndef TEST_CATA_APIRANDOM_H_
#define TEST_CATA_APIRANDOM_H_

#include "DefCATA.h"

#include <string>

class TestCATAApiRandom {
public:
	ISSErrID err;
	HISSCATAINDEX phISSCATAINDEX;
	HISSCATASEARCH phISSCATASEARCH;

	DefCATA def;
	double run_time_;
	std::string getSearchText(int);
	void setRunTime(double run_time) { run_time_ = run_time; };
	TestCATAApiRandom();
	void test(); // ≤‚ ‘»Îø⁄
};

#endif
