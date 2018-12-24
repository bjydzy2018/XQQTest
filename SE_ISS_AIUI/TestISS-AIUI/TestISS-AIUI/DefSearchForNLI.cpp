#include "DefSearchForNLI.h"
#include "rapidjson/document.h"

using std::string;
using namespace rapidjson;
using namespace VA;

DefSearchForNLI::DefSearchForNLI() {
#if VA_VC
	resDir = "..\\Active\\SRRes\\SearchRes";
#elif VA_ANDROID
	resDir = "/sdcard/iflytek/res/Active/SRRes/SearchRes";
#endif
}

string DefSearchForNLI::getSearchErrorCode(string search_ret) {
	Document doc;
	doc.Parse(search_ret.c_str());

	if (doc.HasMember("error")) {
		Value & error = doc["error"];

		if (error.HasMember("errcode")) {
			Value & errcode = error["errcode"];

			return string(errcode.GetString());
		}
	}

	//DEBUG_TRACE_ERROR("search ret is invalid: %s", search_ret.c_str());
	printf("ret: %s\n", search_ret.c_str());
	return "";
}
