#include "DefTTS.h"
#include "CommonFun.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "VAString.h"

#include <fstream>

using namespace rapidjson;
using namespace VA;
using std::string;
using std::ifstream;

int DefTTS::msgOnDataReady = 0;

void DefTTS::initMsg() {
	msgOnDataReady = 0;
}

void DefTTS::Proc_TTS_Ready(void * pUsrArg) {
	++msgOnDataReady;
}

DefTTS::DefTTS() {
#if VA_VC
	resDir = "..\\TTSRes";
	resDirW = L"..\\TTSRes";
#elif VA_ANDROID
	resDir = "/sdcard/iflytek/res/tts";
#else
	resDir = "TTSRes";
#endif

	batch_list_path = testRes_path + "tts/batch_list.txt";
}

// 设置配置文件中发音人的资源载入方式
// param cfg_str: 配置项
//				  格式为『发音人名:资源载入方式』，设置多个发音人以『,』分隔
//                设置全部发音人格式为『all:资源载入方式』
void DefTTS::setResLoadType(string cfg_str) {
	string cfg_path = string(resDir) + "/isstts.cfg";

	Document doc;
	doc.Parse(readFile(cfg_path, false).c_str());
	Value & resource = doc["resource"];

	vector<string> cfgs = split(cfg_str, ',');

	for(size_t i = 0; i < cfgs.size(); ++i){
		vector<string> cfg_values = split(cfgs[i], ':');

		if (cfg_values[0] == "all") {
			for (SizeType i = 0; i < resource.Size(); i++)
			{
				Value & res = resource[i];
				Value & loadtype = res["loadtype"];
				loadtype.SetInt(atoi(cfg_values[1].c_str()));
			}
		}
		else {
			for (SizeType i = 0; i < resource.Size(); i++)
			{
				Value & res = resource[i];
				Value & loadtype = res["loadtype"];
				Value & name = res["name"];
				if (string(name.GetString()) == cfg_values[0]) {
					loadtype.SetInt(atoi(cfg_values[1].c_str()));
					break;
				}
			}
		}
	}

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);

	ofstream fout(cfg_path.c_str());
	fout << buffer.GetString();
	fout.close();
}
