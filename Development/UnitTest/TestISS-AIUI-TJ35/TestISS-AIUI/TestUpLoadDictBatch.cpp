#include "TestUpLoadDictBatch.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"
#include "VAString.h"

#include "rapidjson/document.h"

#include <iostream>
#include <fstream>

using std::string;
using std::vector;
using std::pair;
using namespace rapidjson;
using namespace VA;

TestUpLoadDictBatch::TestUpLoadDictBatch() :err(-1), phISSSR(NULL) {
	
}

TestUpLoadDictBatch::~TestUpLoadDictBatch() {
	//err = ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	//DEBUG_TRACE(("ISSSRDestroy return " + lexical_cast<string>(err)).c_str());

	def.initMsg();
}

void TestUpLoadDictBatch::test(int lang, string dict_path, string dict_name,int UploadDictMode,int ifEveryTimeNeedCreate) {
	if (machineCode_ != "") {
		DEBUG_TRACE("MachineCode:%s", machineCode_.c_str());
		err = ISSSetMachineCode(machineCode_.c_str());
		DEBUG_TRACE("ISSSetMachineCode return %d",err);
	}
	else {
		DEBUG_TRACE("MachineCode:%s", def.machineCode.c_str());
		err = ISSSetMachineCode(def.machineCode.c_str());
		DEBUG_TRACE("ISSSetMachineCode return %d",err);
	}
	
	vector<string> dict_names;
	vector<string> dict_full_names;
	string dict_full_name;
	dict_names = split(dict_name, ',');
	for(size_t i = 0; i < dict_names.size(); ++i){
		string dictName = dict_names[i];
		dict_full_name = dict_path+ dictName;
		dict_full_names.push_back(dict_full_name);
		//DEBUG_TRACE("dict_full_names:%s", dict_full_name.c_str());
	}
	
	if (ifEveryTimeNeedCreate==0) {
		err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
		DEBUG_TRACE("ISSSRCreate return %d",err);
		if (err != ISS_SUCCESS)
			return;
		int count = 0;
		while (!def.msgInitStatus&&++count<3000) {
			sleep(10);
		}
		if (!def.msgInitStatus)
			return;
	}
	DEBUG_TRACE("loop_num : %d",loop_num_);
	for (int m=0;m<loop_num_;m++){
		for(size_t i = 0; i < dict_full_names.size(); ++i){
			string dictFullName = dict_full_names[i];
			if (ifEveryTimeNeedCreate) {
				err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
				DEBUG_TRACE("ISSSRCreate return %d",err);
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
			DEBUG_TRACE("before uploading dict, sleep 30s");
			sleep(30000);
			TimerForTest t;
			// ISSSRUpLoadDict
			err = ISSSRUpLoadDict(phISSSR, readFile(testRes_path + dictFullName, false).c_str(), UploadDictMode);
			DEBUG_TRACE("ISSSRUpLoadDict return %d",err);

			DEBUG_TRACE("waiting for uploading dict");
			int count = 0;
			if (UploadDictMode==0){
				//while ((!def.msgUpLoadDictToLocalStatus || !def.msgUpLoadDictToCloudStatus)&&count++<30000)
				while (!def.msgUpLoadDictToLocalStatus && count++<30000)
					sleep(10);
			}
			if (UploadDictMode==1)
				while (!def.msgUpLoadDictToCloudStatus && count++<30000)
					sleep(10);
			if (UploadDictMode==2)
				while (!def.msgUpLoadDictToLocalStatus && count++<30000)
					sleep(10);

			if (def.msgUpLoadDictToLocalStatus) {
				DEBUG_TRACE("upload dict success");
				double uploaddict_time = t.elapsed();
				DEBUG_TRACE("The dict is:%s,upload dict resume:%f ms",dictFullName.c_str(), uploaddict_time);
			}else{
				DEBUG_TRACE("upload dict fail");
			}
			def.initMsg();
			/*DEBUG_TRACE("after uploading dict, sleep 30s");
			sleep(30000);*/

			if (ifEveryTimeNeedCreate) {
				err = ISSSRDestroy(phISSSR);
				DEBUG_TRACE("ISSSRDestroy return %d",err);
			}
			
		}
	}
	

	if (ifEveryTimeNeedCreate == 0) {
		err = ISSSRDestroy(phISSSR);
		DEBUG_TRACE("ISSSRDestroy return %d",err);
	}

}

