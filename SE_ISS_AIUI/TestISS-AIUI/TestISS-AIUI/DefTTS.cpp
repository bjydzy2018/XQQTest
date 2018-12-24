#ifdef __ANDROID__

#include "DefTTS.h"
#include "CommonFun.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <fstream>

using namespace rapidjson;
using namespace boost::algorithm;
using boost::lexical_cast;
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

 /*���������ļ��з����˵���Դ���뷽ʽ
 param cfg_str: ������
				  ��ʽΪ����������:��Դ���뷽ʽ�������ö���������ԡ�,���ָ�
                ����ȫ�������˸�ʽΪ��all:��Դ���뷽ʽ��*/
void DefTTS::setResLoadType(string cfg_str) {
	string cfg_path = string(resDir) + "/isstts.cfg";

	Document doc;
	doc.Parse(readFile(cfg_path, false).c_str());
	Value & resource = doc["resource"];

	vector<string> cfgs;
	split(cfgs, cfg_str, is_any_of(","));
	BOOST_FOREACH(string cfg, cfgs) {
		vector<string> cfg_values;
		split(cfg_values, cfg, is_any_of(":"));

		if (cfg_values[0] == "all") {
			for (SizeType i = 0; i < resource.Size(); i++)
			{
				Value & res = resource[i];
				Value & loadtype = res["loadtype"];
				loadtype.SetInt(lexical_cast<int>(cfg_values[1]));
			}
		}
		else {
			for (SizeType i = 0; i < resource.Size(); i++)
			{
				Value & res = resource[i];
				Value & loadtype = res["loadtype"];
				Value & name = res["name"];
				if (string(name.GetString()) == cfg_values[0]) {
					loadtype.SetInt(lexical_cast<int>(cfg_values[1]));
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
#endif
