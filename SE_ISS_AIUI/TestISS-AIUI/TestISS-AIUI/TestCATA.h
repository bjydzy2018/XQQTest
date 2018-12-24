#ifndef TEST_CATA_H_
#define TEST_CATA_H_

#include "DefCATA.h"

#include <string>

class TestCATA {
private:
	ISSErrID err;
	HISSCATAINDEX phISSCATAINDEX;
	HISSCATASEARCH phISSCATASEARCH;

	DefCATA def;

public:
	TestCATA();

	void testIndexSearch(std::string, std::string, std::string, std::string);
	void testSearch(std::string, std::string);
};

#endif // !TEST_CATA_H_

