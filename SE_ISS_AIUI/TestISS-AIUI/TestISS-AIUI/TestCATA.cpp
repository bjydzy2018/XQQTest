#include "TestCATA.h"
#include "VAInc.h"
#include "test_log.h"

#include <boost/lexical_cast.hpp>

using namespace VA;
using boost::lexical_cast;
using std::string;

TestCATA::TestCATA(): err(-1), phISSCATAINDEX(NULL), phISSCATASEARCH(NULL){}

void TestCATA::testIndexSearch(std::string index_text, std::string index_res,
								std::string srh_text, std::string srh_res) {
	DEBUG_TRACE_INFOR(("adding res: " + string(PrtU8(index_res.c_str()))).c_str());
	err = ISSCATAAdapter::ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, index_res.c_str(), 1, DefCATA::onCataMsgProc, NULL);
	DEBUG_TRACE(("ISSCataIndexCreate return " + lexical_cast<string>(err)).c_str());

	DEBUG_TRACE_INFOR(("adding index: " + string(PrtU8(def.parseSegValues(index_text).c_str()))).c_str());
	err = ISSCATAAdapter::ISSCataIndexAddIdxEntity(phISSCATAINDEX, def.parseSegValues(index_text).c_str());
	DEBUG_TRACE(("ISSCataIndexAddIdxEntity return " + lexical_cast<string>(err)).c_str());

	err = ISSCATAAdapter::ISSCataIndexEndIdxEntity(phISSCATAINDEX);
	DEBUG_TRACE(("ISSCataIndexEndIdxEntity return " + lexical_cast<string>(err)).c_str());

	err = ISSCATAAdapter::ISSCataIndexDestroy(phISSCATAINDEX);
	DEBUG_TRACE(("ISSCataIndexDestroy return " + lexical_cast<string>(err)).c_str());
	phISSCATAINDEX = NULL;

	DEBUG_TRACE_INFOR(("searching res: " + string(PrtU8(srh_res.c_str()))).c_str());
	err = ISSCATAAdapter::ISSCataSearchCreate(&phISSCATASEARCH, def.resDir, srh_res.c_str(), DefCATA::onCataMsgProc, NULL);
	DEBUG_TRACE(("ISSCataSearchCreate return " + lexical_cast<string>(err)).c_str());

	DEBUG_TRACE_INFOR(("searching text: " + string(PrtU8(srh_text.c_str()))).c_str());
	string search_ret = ISSCATAAdapter::ISSCataSearchSync(phISSCATASEARCH, srh_text.c_str(), &err);

	DEBUG_TRACE_INFOR(("ISSCataSearchSync return : " + lexical_cast<string>(err)).c_str());
	DEBUG_TRACE_INFOR(("ISSCataSearchSync ret : " + string(PrtU8(search_ret.c_str()))).c_str());

	ISSCATAAdapter::ISSCataSearchDestroy(phISSCATASEARCH);
	phISSCATASEARCH = NULL;
}

void TestCATA::testSearch(string srh_text, string srh_res) {
	DEBUG_TRACE_INFOR(("searching res: " + string(PrtU8(srh_res.c_str()))).c_str());

	err = ISSCATAAdapter::ISSCataSearchCreate(&phISSCATASEARCH, def.resDir, srh_res.c_str(), DefCATA::onCataMsgProc, NULL);
	DEBUG_TRACE(("ISSCataSearchCreate return " + lexical_cast<string>(err)).c_str());

	if (err == 0) {
		DEBUG_TRACE_INFOR(("searching text: " + string(PrtU8(srh_text.c_str()))).c_str());
		string search_ret = ISSCATAAdapter::ISSCataSearchSync(phISSCATASEARCH, srh_text.c_str(), &err);

		DEBUG_TRACE_INFOR(("ISSCataSearchSync return : " + lexical_cast<string>(err)).c_str());
		DEBUG_TRACE_INFOR(("ISSCataSearchSync ret : " + string(PrtU8(search_ret.c_str()))).c_str());

		ISSCATAAdapter::ISSCataSearchDestroy(phISSCATASEARCH);
		phISSCATASEARCH = NULL;
	}
}
