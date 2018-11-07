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

	std::string getSearchText(int);

	TestCATAApiRandom();
	void test(); // ≤‚ ‘»Îø⁄
};

#endif
