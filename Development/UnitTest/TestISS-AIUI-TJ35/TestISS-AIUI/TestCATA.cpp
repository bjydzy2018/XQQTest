#include "TestCATA.h"
#include "VAInc.h"
#include "test_log.h"

using namespace VA;
using std::string;

TestCATA::TestCATA(): err(-1), phISSCATAINDEX(NULL), phISSCATASEARCH(NULL){}

void TestCATA::testIndexSearch(std::string index_text, std::string index_res,
								std::string srh_text, std::string srh_res) {
	DEBUG_TRACE_INFOR(("adding res: " + string(PrtU8(index_res.c_str()))).c_str());
	err = ISSCataIndexCreate(&phISSCATAINDEX, def.resDir, index_res.c_str(), 1, DefCATA::onCataMsgProc, NULL);
	DEBUG_TRACE("ISSCataIndexCreate return %d" , err);

	DEBUG_TRACE_INFOR(("adding index: " + string(PrtU8(def.parseSegValues(index_text).c_str()))).c_str());
	err = ISSCataIndexAddIdxEntity(phISSCATAINDEX, def.parseSegValues(index_text).c_str());
	DEBUG_TRACE("ISSCataIndexAddIdxEntity return %d" , err);

	err = ISSCataIndexEndIdxEntity(phISSCATAINDEX);
	DEBUG_TRACE("ISSCataIndexEndIdxEntity return %d" , err);

	err = ISSCataIndexDestroy(phISSCATAINDEX);
	DEBUG_TRACE("ISSCataIndexDestroy return %d" , err);
	phISSCATAINDEX = NULL;

	DEBUG_TRACE_INFOR(("searching res: " + string(PrtU8(srh_res.c_str()))).c_str());
	err = ISSCataSearchCreate(&phISSCATASEARCH, def.resDir, srh_res.c_str(), DefCATA::onCataMsgProc, NULL);
	DEBUG_TRACE("ISSCataSearchCreate return %d" , err);

	DEBUG_TRACE_INFOR(("searching text: " + string(PrtU8(srh_text.c_str()))).c_str());
	string search_ret = ISSCataSearchSync(phISSCATASEARCH, srh_text.c_str(), &err);

	DEBUG_TRACE_INFOR("ISSCataSearchSync return %d" , err);
	DEBUG_TRACE_INFOR(("ISSCataSearchSync ret : " + string(PrtU8(search_ret.c_str()))).c_str());

	ISSCataSearchDestroy(phISSCATASEARCH);
	phISSCATASEARCH = NULL;
}

void TestCATA::testSearch(string srh_text, string srh_res) {
	DEBUG_TRACE_INFOR(("searching res: " + string(PrtU8(srh_res.c_str()))).c_str());

	err = ISSCataSearchCreate(&phISSCATASEARCH, def.resDir, srh_res.c_str(), DefCATA::onCataMsgProc, NULL);
	DEBUG_TRACE("ISSCataSearchCreate return %d" , err);

	if (err == 0) {
		DEBUG_TRACE_INFOR(("searching text: " + string(PrtU8(srh_text.c_str()))).c_str());
		string search_ret = ISSCataSearchSync(phISSCATASEARCH, srh_text.c_str(), &err);

		DEBUG_TRACE_INFOR("ISSCataSearchSync return %d" , err);
		DEBUG_TRACE_INFOR(("ISSCataSearchSync ret : " + string(PrtU8(search_ret.c_str()))).c_str());

		ISSCataSearchDestroy(phISSCATASEARCH);
		phISSCATASEARCH = NULL;
	}
}
