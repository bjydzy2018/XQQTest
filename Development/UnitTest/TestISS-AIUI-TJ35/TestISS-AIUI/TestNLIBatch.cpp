#include "TestNLIBatch.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"
#include "TimerForTest.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <fstream>

using std::string;
using std::vector;
using namespace rapidjson;
using namespace VA;

typedef struct tagParams {
	string text_ctn;
	string userdata;
} TParams;


#ifdef ISS_SR_3

TestNLIBatch::TestNLIBatch():err(-1), phISSSR(NULL) {

}

TestNLIBatch::~TestNLIBatch() {
	err = ISSSRDestroy(phISSSR);
	phISSSR = NULL;
	DEBUG_TRACE("ISSSRDestroy return %d",err);

	def.initMsg();
}

void TestNLIBatch::test(int lang, int mode, string text_list_path, std::string dict_path,int UploadDictMode){
	if (machineCode_ != "") {
		DEBUG_TRACE("MachineCode:%s", machineCode_.c_str());
		err = ISSSetMachineCode(machineCode_.c_str());
		DEBUG_TRACE(("ISSSetMachineCode return " + LongToString(err)).c_str());
	}
	else {
		DEBUG_TRACE("MachineCode:%s", def.machineCode.c_str());
		err = ISSSetMachineCode(def.machineCode.c_str());
		DEBUG_TRACE(("ISSSetMachineCode return " + LongToString(err)).c_str());
	}

	string lang_str("");
	string mode_str("");

	if (lang == 0)
		lang_str = "chinese";
	else if (lang == 1)
		lang_str = "english";

	if (mode == 0)
		mode_str = "cloud";
	else if (mode == 1)
		mode_str = "local";


	// ISSSRCreate
	err = ISSSRCreateEx(&phISSSR, tagISS_SR_ACOUS_LANG(lang), def.resDir, DefSR::onMsgProc, NULL);
	DEBUG_TRACE("ISSSRCreate return %d",err);
	if (err != ISS_SUCCESS)
		return;
	
	int count = 0;
	while (!def.msgInitStatus&&++count < 3000) {
		sleep(10);
	}

	if (!def.msgInitStatus)
		return;

	ofstream fout((testRes_path + "nli/batch/ret/nli_" +  lang_str + "_" + mode_str + ".csv").c_str());
	if (!fout.is_open()) {
		DEBUG_TRACE_ERROR("create result file failed");
		return;
	}

	fout << "nli_text,mode,nli_out_result\n";

	/*DEBUG_TRACE("after creating, sleep 10s");
	sleep(10000);*/

	if (dict_path != "") {

		// ISSSRUpLoadDict
		err = ISSSRUpLoadDict(phISSSR, readFile(testRes_path + dict_path, false).c_str(), UploadDictMode);
		DEBUG_TRACE("ISSSRUpLoadDict return %d",err);

		DEBUG_TRACE("waiting for uploading dict");
		int count = 0;
		if (UploadDictMode==0)
			while ((!def.msgUpLoadDictToLocalStatus || !def.msgUpLoadDictToCloudStatus)&&count++<3000)
				sleep(10);
		if (UploadDictMode==1)
			while (!def.msgUpLoadDictToCloudStatus &&count++<3000)
				sleep(10);
		if (UploadDictMode==2)
			while (!def.msgUpLoadDictToLocalStatus &&count++<3000)
				sleep(10);

		DEBUG_TRACE("after uploading dict, sleep 10s");
		sleep(10000);
	}

	vector< TParams > vec_pair;
	TParams TempPair;
	// 获取语义输入
	vector<string> nli_ins = readFileByLine(testRes_path + text_list_path, false);
	for(int i=0;i < nli_ins.size();i++){
		Document doc;
		doc.Parse(nli_ins[i].c_str());

		if (doc.HasMember("text")) {
			TempPair.text_ctn = doc["text"].GetString();
		}else{
			TempPair.text_ctn = "";
		}

		if (doc.HasMember("user_data")) {
			Value& valObj = doc["user_data"];
			StringBuffer sbBuf;
			Writer<StringBuffer> jWriter(sbBuf);
			valObj.Accept(jWriter);
			TempPair.userdata = sbBuf.GetString();
		}else{
			TempPair.userdata = "";
		}

		vec_pair.push_back(TempPair);
	}
	
	//return;

	DEBUG_TRACE("sr batch loop num: %d", loop_num_);
	DEBUG_TRACE("sr batch sleep time: %d", sleep_time_);

	if (loop_num_ == -1)
		loop_num_ = 10000000;

	int test_num = 1; // 
	for (int i = 0; i < loop_num_; i++) {
		for(size_t j = 0; j < vec_pair.size(); ++j){


			TParams textanddata = vec_pair[j];
			string text_ctn = textanddata.text_ctn;
			if (textanddata.userdata != ""){
				textanddata.userdata = string("{\"UserData\": ") + textanddata.userdata + string("}");
				DEBUG_TRACE("userdata: %s", PrtU8((textanddata.userdata).c_str()));
				err = ISSSRUpLoadData(phISSSR, (textanddata.userdata).c_str(), UploadDictMode);

				DEBUG_TRACE("ISSSRUpLoadData return %d", err);
				if(err==0){
					int count = 0;
					if (UploadDictMode==0||UploadDictMode==1){
						while (!def.msgUpLoadDataToCloudStatus&&count++<1000)
							sleep(10);
					}

					if (UploadDictMode==0||UploadDictMode==1){
						if (def.msgUpLoadDataToCloudStatus) {
							DEBUG_TRACE("ISSSRUpLoadData success");
						}
						else {
							DEBUG_TRACE("ISSSRUpLoadData failed");
						}		
					}
				}
				
			}
			

			DEBUG_TRACE_INFOR("current test num: %d, text_ctn: %s", test_num++, PrtU8(text_ctn.c_str()));
			const char *nluResult = NULL;
			string strNliResult("");
			if(mode==1){
//#if VA_VC
//				nluResult=ISSSRLocalNli(phISSSR,text_ctn.c_str(),ISS_SR_SCENE_ALL, &err);
//#elif VA_GNUC
//				nluResult=ISSSRLocalNli(phISSSR, text_ctn.c_str(),ISS_SR_SCENE_ALL, &err);
//#endif
				nluResult=ISSSRLocalNli(phISSSR, text_ctn.c_str(),ISS_SR_SCENE_ALL, &err);
				DEBUG_TRACE("ISSSRLocalNli result=%s", PrtU8(nluResult));	
				DEBUG_TRACE("ISSSRLocalNli Err=%d", err);	
			}
			if(mode==0){
//#if VA_VC
//				nluResult=ISSSRMspSearch(phISSSR, text_ctn.c_str(),NULL, &err);
//#elif VA_GNUC
//				nluResult=ISSSRMspSearch(phISSSR, text_ctn.c_str(),NULL, &err);
//#endif
				nluResult=ISSSRMspSearch(phISSSR, text_ctn.c_str(),NULL, &err);
				DEBUG_TRACE("ISSSRMspSearch result=%s", PrtU8(nluResult));	
				DEBUG_TRACE("ISSSRMspSearch Err=%d", err);	
				sleep(sleep_time_);
			}
			strNliResult = string(VA::U8StrToGBKStr(nluResult));
			replace_all(strNliResult, "\n", "");
			replace_all(strNliResult, "\"", "\"\"");

			fout << string(VA::U8StrToGBKStr(text_ctn.c_str())) << ",";
			
			fout << mode_str << ",";
			fout << "\"" << strNliResult << "\"\n";
			fout.flush();

			def.initMsg();
		}
	}

	fout.close();
}

#endif
