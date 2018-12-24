#include "TestUpLoadDictBatch.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

#include "rapidjson/document.h"

#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/timer.hpp>
#include <boost/filesystem.hpp>

using std::string;
using std::vector;
using std::pair;
using boost::lexical_cast;
using boost::timer;
using namespace rapidjson;
using namespace boost::algorithm;
using namespace VA;


#ifndef ISS_SR_3

TestUpLoadDictBatch::TestUpLoadDictBatch() :err(-1), phISSSR(NULL) {
	
}

TestUpLoadDictBatch::~TestUpLoadDictBatch() {
	//err = ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	//DEBUG_TRACE(("ISSSRDestroy return " + lexical_cast<string>(err)).c_str());

	def.initMsg();
}

void TestUpLoadDictBatch::test(int lang, string dict_path, string dict_name,int ifOnlyUploadToCloud,int ifEveryTimeNeedCreate) {
	if (machineCode_ != "") {
		DEBUG_TRACE("MachineCode:%s", machineCode_.c_str());
		err = ISSSRAdapter::ISSSetMachineCode(machineCode_.c_str());
		DEBUG_TRACE(("ISSSetMachineCode return " + lexical_cast<string>(err)).c_str());
	}
	else {
		DEBUG_TRACE("MachineCode:%s", def.machineCode.c_str());
		err = ISSSRAdapter::ISSSetMachineCode(def.machineCode.c_str());
		DEBUG_TRACE(("ISSSetMachineCode return " + lexical_cast<string>(err)).c_str());
	}
	
	vector<string> dict_names;
	vector<string> dict_full_names;
	string dict_full_name;
	split(dict_names, dict_name, is_any_of(","));
	BOOST_FOREACH(string dictName, dict_names) {
		dict_full_name = dict_path+ dictName;
		dict_full_names.push_back(dict_full_name);
		//DEBUG_TRACE("dict_full_names:%s", dict_full_name.c_str());
	}
	
	if (ifEveryTimeNeedCreate==0) {
		err = ISSSRAdapter::ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
		DEBUG_TRACE(("ISSSRCreate return " + lexical_cast<string>(err)).c_str());
		if (err != ISS_SUCCESS)
			return;
		int count = 0;
		while (!def.msgInitStatus&&++count<3000) {
			sleep(10);
		}
		if (!def.msgInitStatus)
			return;
	}
	BOOST_FOREACH(string dictFullName, dict_full_names) {
		if (ifEveryTimeNeedCreate) {
			err = ISSSRAdapter::ISSSRCreate(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
			DEBUG_TRACE(("ISSSRCreate return " + lexical_cast<string>(err)).c_str());
			if (err != ISS_SUCCESS)
				return;
			int count = 0;
			while (!def.msgInitStatus&&++count<3000) {
				sleep(10);
			}
			if (!def.msgInitStatus)
				return;
		}
		DEBUG_TRACE("dictFullName:%s",dictFullName.c_str());
		timer t;
		// ISSSRUpLoadDict
		err = ISSSRAdapter::ISSSRUpLoadDict(phISSSR, readFile(testRes_path + dictFullName, false).c_str(), ifOnlyUploadToCloud);
		DEBUG_TRACE(("ISSSRUpLoadDict return " + lexical_cast<string>(err)).c_str());

		DEBUG_TRACE("waiting for uploading dict");
		/*if (!ifOnlyUploadToCloud)
		while (!def.msgUpLoadDictToLocalStatus || !def.msgUpLoadDictToCloudStatus)
		sleep(10);
		else
		while (!def.msgUpLoadDictToLocalStatus && !def.msgUpLoadDictToCloudStatus)
		sleep(10);*/
		//目前只支持本地传个性化，云端个性化不支持
		
		while (!def.msgUpLoadDictToLocalStatus)
			sleep(10);
		if (def.msgUpLoadDictToLocalStatus) {
			DEBUG_TRACE("upload dict success");
			double uploaddict_time = t.elapsed() * 1000;
			DEBUG_TRACE("upload dict resume:%f ms", uploaddict_time);
		}else{
			DEBUG_TRACE("upload dict fail");
		}
		def.initMsg();

		if (ifEveryTimeNeedCreate) {
			err = ISSSRAdapter::ISSSRDestroy(phISSSR);
			DEBUG_TRACE(("ISSSRDestroy return " + lexical_cast<string>(err)).c_str());
		}
		DEBUG_TRACE("after uploading dict, sleep 5s");
		sleep(5000);

	}

	if (ifEveryTimeNeedCreate == 0) {
		err = ISSSRAdapter::ISSSRDestroy(phISSSR);
		DEBUG_TRACE(("ISSSRDestroy return " + lexical_cast<string>(err)).c_str());
	}

}

#endif
