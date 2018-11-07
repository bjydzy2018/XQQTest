#include "TestCATAApiRandom.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

using namespace VA;
using std::string;

int cata_real6(){
	return rand() % 6;
}
int cata_real32768(){
	return (rand() % 32768)*2 - 32768;
}

TestCATAApiRandom::TestCATAApiRandom() :err(-1), phISSCATAINDEX(NULL), phISSCATASEARCH(NULL) {}

void TestCATAApiRandom::test() {
	//boost::random::mt19937 gen;
	//boost::uniform_int<> cata_real6(0, 6);
	//boost::uniform_int<> cata_real32768(-32768, 32768);

	srand((unsigned)time(0));

	err = ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, "testApiRandom.bin", 1, DefCATA::onCataMsgProc, NULL);
	DEBUG_TRACE("ISSCataIndexCreate return %d",err);
	if (err != ISS_SUCCESS)
		return;

	err = ISSCataSearchCreate(&phISSCATASEARCH, def.resDir, "testApiRandom.bin", DefCATA::onCataMsgProc, NULL);
	DEBUG_TRACE("ISSCataSearchCreate return %d",err);
	if (err != ISS_SUCCESS)
		return;

	while (true) {
		string index(def.parseSegValues("name|交通台111|1	code|123456789|1	tag|book|2"));

		switch (cata_real6())
		{
		case 0: {
			DEBUG_TRACE("call ISSCataIndexAddIdxEntity");
#if VA_VC
			err = ISSCataIndexAddIdxEntity(phISSCATAINDEX, (const char *)GBKStrToU8Str(index.c_str()));
#elif VA_GNUC
			err = ISSCataIndexAddIdxEntity(phISSCATAINDEX, index.c_str());
#endif
			break;
		}
		case 1: {
			DEBUG_TRACE("call ISSCataIndexDelIdxEntity");
#if VA_VC
			err = ISSCataIndexDelIdxEntity(phISSCATAINDEX, (const char *)GBKStrToU8Str(index.c_str()));
#elif VA_GNUC
			err = ISSCataIndexDelIdxEntity(phISSCATAINDEX, index.c_str());
#endif
			break;
		}
		case 2:
			DEBUG_TRACE("call ISSCataIndexEndIdxEntity");
			err = ISSCataIndexEndIdxEntity(phISSCATAINDEX);
			break;
		case 3:
			DEBUG_TRACE("call ISSCataIndexDropRes");
			err = ISSCataIndexDropRes(phISSCATAINDEX);
			break;
		case 4: {
			string text = getSearchText(cata_real6());
			DEBUG_TRACE(("call ISSCataSearchSync, text: " + text).c_str());
#if VA_VC
			ISSCataSearchSync(phISSCATASEARCH, (const char *)GBKStrToU8Str(text.c_str()), &err);
#elif VA_GNUC
			ISSCataSearchSync(phISSCATASEARCH, text.c_str(), &err);
#endif
			break;
		}
		case 5: {
			string text = getSearchText(cata_real6());
			DEBUG_TRACE(("call ISSCataSearchAsync, text: " + text).c_str());
#if VA_VC
			err = ISSCataSearchAsync(phISSCATASEARCH, (const char *)GBKStrToU8Str(text.c_str()));
#elif VA_GNUC
			err = ISSCataSearchAsync(phISSCATASEARCH, text.c_str());
#endif
			break;
		}
		case 6: {
			int param = cata_real32768();
			int paramValue = cata_real32768();
			DEBUG_TRACE(("call ISSCataSetParam, param: " + LongToString(param) + ", paramValue: " + LongToString(paramValue)).c_str());
			err = ISSCataSetParam(phISSCATASEARCH, param, paramValue);
		}
		default:
			break;
		}
	}
}

string TestCATAApiRandom::getSearchText(int item) {
	switch (item) {
	case 0:
		return "(field=name,query=交通台,fuzzy=1,searchType=SR)";
	case 1:
		return "";
	case 2:
		return "(field=name,query=交通台,fuzzy=1,searchType=SR";
	case 3:
		return "(field=name,query=交通台,fuzzy=-3,searchType=SR)";
	case 4:
		return "(field=name,query=交通台,fuzzy=1,searchType=BHIDSFG)";
	case 5:
		return "△╲Ⅸ❽‰にオУ┿夊Óū";
	default:
		return "";
	}
}
