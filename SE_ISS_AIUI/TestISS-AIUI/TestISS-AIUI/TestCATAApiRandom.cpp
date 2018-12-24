#include "TestCATAApiRandom.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

using namespace VA;
using std::string;
using boost::lexical_cast;
using boost::thread;
using boost::bind;
using boost::timer;

TestCATAApiRandom::TestCATAApiRandom() :err(-1), phISSCATAINDEX(NULL), phISSCATASEARCH(NULL) {}

void TestCATAApiRandom::test() {
	boost::random::mt19937 gen;
	boost::uniform_int<> real6(0, 6);
	boost::uniform_int<> real32768(-32768, 32768);

	err = ISSCATAAdapter::ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, "testApiRandom.bin", 1, DefCATA::onCataMsgProc, NULL);
	DEBUG_TRACE(("ISSCataIndexCreate return " + lexical_cast<string>(err)).c_str());
	if (err != ISS_SUCCESS)
		return;

	err = ISSCATAAdapter::ISSCataSearchCreate(&phISSCATASEARCH, def.resDir, "testApiRandom.bin", DefCATA::onCataMsgProc, NULL);
	DEBUG_TRACE(("ISSCataSearchCreate return " + lexical_cast<string>(err)).c_str());
	if (err != ISS_SUCCESS)
		return;
	timer t;
	double sum_time = 0;
	double expect_time = run_time_*60*60*1000;
	while (sum_time < expect_time) {
		t.restart();
		string index(def.parseSegValues("name|交通台111|1	code|123456789|1	tag|book|2"));

		switch (real6(gen))
		{
		case 0: {
			DEBUG_TRACE("call ISSCataIndexAddIdxEntity");
#if VA_VC
			err = ISSCATAAdapter::ISSCataIndexAddIdxEntity(phISSCATAINDEX, (const char *)GBKStrToU8Str(index.c_str()));
#elif VA_GNUC
			err = ISSCATAAdapter::ISSCataIndexAddIdxEntity(phISSCATAINDEX, index.c_str());
#endif
			break;
		}
		case 1: {
			DEBUG_TRACE("call ISSCataIndexDelIdxEntity");
#if VA_VC
			err = ISSCATAAdapter::ISSCataIndexDelIdxEntity(phISSCATAINDEX, (const char *)GBKStrToU8Str(index.c_str()));
#elif VA_GNUC
			err = ISSCATAAdapter::ISSCataIndexDelIdxEntity(phISSCATAINDEX, index.c_str());
#endif
			break;
		}
		case 2:
			DEBUG_TRACE("call ISSCataIndexEndIdxEntity");
			err = ISSCATAAdapter::ISSCataIndexEndIdxEntity(phISSCATAINDEX);
			break;
		case 3:
			DEBUG_TRACE("call ISSCataIndexDropRes");
			err = ISSCATAAdapter::ISSCataIndexDropRes(phISSCATAINDEX);
			break;
		case 4: {
			string text = getSearchText(real6(gen));
			DEBUG_TRACE(("call ISSCataSearchSync, text: " + text).c_str());
#if VA_VC
			ISSCATAAdapter::ISSCataSearchSync(phISSCATASEARCH, (const char *)GBKStrToU8Str(text.c_str()), &err);
#elif VA_GNUC
			ISSCATAAdapter::ISSCataSearchSync(phISSCATASEARCH, text.c_str(), &err);
#endif
			break;
		}
		case 5: {
			string text = getSearchText(real6(gen));
			DEBUG_TRACE(("call ISSCataSearchAsync, text: " + text).c_str());
#if VA_VC
			err = ISSCATAAdapter::ISSCataSearchAsync(phISSCATASEARCH, (const char *)GBKStrToU8Str(text.c_str()));
#elif VA_GNUC
			err = ISSCATAAdapter::ISSCataSearchAsync(phISSCATASEARCH, text.c_str());
#endif
			break;
		}
		case 6: {
			int param = real32768(gen);
			int paramValue = real32768(gen);
			DEBUG_TRACE(("call ISSCataSetParam, param: " + lexical_cast<string>(param) + ", paramValue: " + lexical_cast<string>(paramValue)).c_str());
			err = ISSCATAAdapter::ISSCataSetParam(phISSCATASEARCH, param, paramValue);
		}
		default:
			break;
		}
		double each_time = t.elapsed() * 1000;
		sum_time += each_time;
	}
	err = ISSCATAAdapter::ISSCataIndexDestroy(phISSCATAINDEX);
	DEBUG_TRACE(("ISSCataIndexDestroy return " + lexical_cast<string>(err)).c_str());
	err = ISSCATAAdapter::ISSCataSearchDestroy(phISSCATASEARCH);
	DEBUG_TRACE(("ISSCataSearchDestroy return " + lexical_cast<string>(err)).c_str());
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
