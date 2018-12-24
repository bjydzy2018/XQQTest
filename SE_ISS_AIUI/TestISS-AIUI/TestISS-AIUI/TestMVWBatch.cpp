#include "TestMVWBatch.h"
#include "CommonFun.h"
#include "test_log.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdarg>

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/foreach.hpp>

using namespace VA;
using std::vector;
using std::endl;

using boost::lexical_cast;
using boost::regex;
using boost::smatch;

int ParseMvw2List(string Mvw2List, vector<string> &vec, vector<pair<string, int> > &vec_pair)
{
	vec = readFileByLine(Mvw2List, false);
	pair<string, int> mPair;
	string Tempstr;

	for (size_t i = 0;i<vec.size();i++) {
		Tempstr = vec[i];
		//DEBUG_TRACE(PrtU8(Tempstr.c_str()));
		std::size_t found = Tempstr.find_last_of("/\\");
		mPair.second = atoi((Tempstr.substr(found - 1, found)).c_str());
		//DEBUG_TRACE("%d",mPair.second);

		Tempstr = Tempstr.substr(found + 1);
		vector<string> Temp_v = split(Tempstr, '.');
		mPair.first = Temp_v[0];
		//DEBUG_TRACE(PrtU8(mPair.first.c_str()));

		vec_pair.push_back(mPair);
	}
	return 0;

}

TestMVWBatch::TestMVWBatch() : err(-1), phISSMVW(NULL),  scene(1) {
	string scene_str = lexical_cast<string>(scene);
	audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/SceneId-" + scene_str + ".txt";
	vw_ret_name = "SceneId-" + scene_str + "/SceneId-" + scene_str + "_ret.txt";

	err = ISSMVWAdapter::ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL);
	DEBUG_TRACE(("ISSMVWCreate return " + lexical_cast<string>(err)).c_str());
}

void TestMVWBatch::testMVWBatchByScene(int scenes_count, ...) {
	va_list args;
	va_start(args, scenes_count);

	while (scenes_count--) {
		scene = va_arg(args, int);
		string scene_str = lexical_cast<string>(scene);

		audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/SceneId-" +
			scene_str + ".txt";
		vw_ret_name = "SceneId-" + scene_str + "/SceneId-" + scene_str + "_ret.txt";

		testMVWBatch("scene(" + scene_str + ")", false);
	}

	ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	phISSMVW = NULL;
}

void TestMVWBatch::testMVWBatchByScene(vector<string> & scenes) {
	BOOST_FOREACH(string scene_str, scenes) {
		scene = lexical_cast<int>(scene_str);;

		audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/SceneId-" +
			scene_str + ".txt";
		vw_ret_name = "SceneId-" + scene_str + "/SceneId-" + scene_str + "_ret.txt";

		testMVWBatch("scene(" + scene_str + ")", false);
	}

	ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	phISSMVW = NULL;
}

void TestMVWBatch::testMVWBatchByWord() {
	//scene = 1;
	//string scene_str = lexical_cast<string>(scene);
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/SceneId-" + scene_str +
	//        ".txt";
	//vw_ret_name = "SceneId-" + scene_str + "/你好语音助理_ret.txt";
	//testMVWBatch("1-你好语音助理", false);
	//
	//scene = 2;
	//scene_str = lexical_cast<string>(scene);
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/确定.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/确定_ret.txt";
	//testMVWBatch("2-确定", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/取消.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/取消_ret.txt";
	//testMVWBatch("2-取消", false);
	//
	//scene = 4;
	//scene_str = lexical_cast<string>(scene);
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/第一个.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/第一个_ret.txt";
	//testMVWBatch("4-第一个", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/第二个.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/第二个_ret.txt";
	//testMVWBatch("4-第二个", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/第三个.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/第三个_ret.txt";
	//testMVWBatch("4-第三个", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/第四个.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/第四个_ret.txt";
	//testMVWBatch("4-第四个", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/第五个.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/第五个_ret.txt";
	//testMVWBatch("4-第五个", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/第六个.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/第六个_ret.txt";
	//testMVWBatch("4-第六个", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/最后一个.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/最后一个_ret.txt";
	//testMVWBatch("4-最后一个", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/取消.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/取消_ret.txt";
	//testMVWBatch("4-取消", false);

	//scene = 8;
	//scene_str = lexical_cast<string>(scene);
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/接听.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/接听_ret.txt";
	//testMVWBatch("8-接听", false);

	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/挂断.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/挂断_ret.txt";
	//testMVWBatch("8-挂断", false);

	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/取消.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/取消_ret.txt";
	//testMVWBatch("8-取消", false);

	string scene_str = "";

	vector<string> vec;
	vector<pair<string, int> > vec_pair;

	ParseMvw2List("TestRes/mvw/batch/list/mvw_list_list.txt", vec, vec_pair);
	for (size_t i = 0;i<vec.size();i++) {
		scene = vec_pair[i].second;
		DEBUG_TRACE(("scene " + lexical_cast<string>(scene)).c_str());
		scene_str =lexical_cast<string>(scene);
		audio_list_path = "TestRes/" + vec[i];
		vw_ret_name = "SceneId-" + scene_str + "/" + "SceneId-" + scene_str + "_" +
			vec_pair[i].first + "_ret.txt";
		testMVWBatch(scene_str + "-" + vec_pair[i].first, false);
	}

	ISSMVWStop(phISSMVW);

	ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	phISSMVW = NULL;
}

int TestMVWBatch::testMVWConsistent() {
	scene = 8;
	audio_list_path = "\\\\192.168.28.101\\套件测试集\\唤醒\\Daily-Consistent\\一致性测试集\\小艾你好\\xanh_iss.txt";
	vw_ret_name = "小艾你好_ret.txt";
	testMVWBatch("1-小艾你好", true);

	scene = 1;
	audio_list_path = "\\\\192.168.28.101\\套件测试集\\唤醒\\Daily-Consistent\\一致性测试集\\你好语音助理\\nhyyzl_iss.txt";
	vw_ret_name = "你好语音助理_ret.txt";
	testMVWBatch("1-你好语音助理", true);

	scene = 2;
	audio_list_path = "\\\\192.168.28.101\\套件测试集\\唤醒\\Daily-Consistent\\一致性测试集\\取消\\取消_iss.lst";
	vw_ret_name = "取消_ret.txt";
	testMVWBatch("2-取消", true);

	scene = 4;
	ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	phISSMVW = NULL;
	err = ISSMVWAdapter::ISSMVWSetMvwLanguage(ISS_MVW_LANG_ENG);
	isslog_func_return(err, ("ISSMVWSetMvwLanguage return %d", err));

	err = ISSMVWAdapter::ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL);
	isslog_func_return(err, ("ISSMVWCreate return %d", err));
	audio_list_path = "\\\\192.168.28.101\\套件测试集\\唤醒\\Daily-Consistent\\一致性测试集\\cancel\\cancel_iss.lst";
	vw_ret_name = "cancel_ret.txt";
	testMVWBatch("2-cancel", true);

	ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	phISSMVW = NULL;
}

void TestMVWBatch::testMVWBatch(string vw_label, bool is_test_consistent) {
	string vw_ret_path("");
	if (!is_test_consistent)
		vw_ret_path = "TestRes/mvw/batch/ret/" + vw_ret_name;
	else
		vw_ret_path = "TestRes/mvw/consistent/ret/" + vw_ret_name;
	int vw_num = 0; // 尝试唤醒次数
	int vw_success_num = 0; // 唤醒成功次数
	int vw_vad_success_num = 0; // 标注了后端点的音频的唤醒成功次数
	double vad_end_time = 0; // 后端点
	double vw_time = 0; // 唤醒响应时间
	double vw_time_sum = 0; // 总唤醒响应时间

	DEBUG_TRACE_INFOR(("test mvw batch: " + vw_label + ", audio list path: " + audio_list_path + ", ret path: " + vw_ret_path).c_str());

	vector<string> audio_list = readFileByLine(audio_list_path, true);
	ofstream fout(vw_ret_path.c_str(), ios::out);

	if (!is_test_consistent) {
		DEBUG_TRACE("test mvw batch effect, start only once");

		err = ISSMVWAdapter::ISSMVWStart(phISSMVW, scene);
		DEBUG_TRACE(("ISSMVWStart return " + lexical_cast<string>(err)).c_str());
	} else
		DEBUG_TRACE("test mvw batch consistent, start before appending audio data each time");

	BOOST_FOREACH(string audio, audio_list) {
		if (is_test_consistent) {
			err = ISSMVWAdapter::ISSMVWStart(phISSMVW, scene);
			DEBUG_TRACE(("ISSMVWStart return " + lexical_cast<string>(err)).c_str());
		}

		vw_num++;

		audio = use_path_pre_ + audio;

		DEBUG_TRACE_INFOR(("test number: " + lexical_cast<string>(vw_num) + ", success rate: " + 
			lexical_cast<string>(vw_num == 1 ? 0 : ((double)vw_success_num) / (vw_num - 1) * 100) + "%%").c_str());

		fout << audio;

		vad_end_time = getVadEndTime(audio);
		DEBUG_TRACE_INFOR("getVadEndTime return %d", vad_end_time);

		appendAudio(phISSMVW, "mvw", audio, def);

		if (def.msgWakeup) {
			vw_time = def.vw_time - vad_end_time;

			vw_success_num++;
			string vw_ret = "\t";
			vw_ret += "nMvwScene:" + lexical_cast<string>(def.wakeupInfo.getnMvwScene()) + ",";
			vw_ret += "nMvwId:" + lexical_cast<string>(def.wakeupInfo.getnMvwId()) + ",";
			vw_ret += "nMvwScore:" + lexical_cast<string>(def.wakeupInfo.getnMvwScore()) + ",";
			vw_ret += "lParam:" + def.wakeupInfo.getlParam();

			if (vad_end_time > 0) {
				DEBUG_TRACE_INFOR(("get vad end time: " + lexical_cast<string>(vad_end_time)).c_str());

				if (vw_time > 0) {
					vw_vad_success_num++;
					vw_time_sum += vw_time;
					vw_ret += "\t" + lexical_cast<string>(vw_time);
				}
			}
			fout << vw_ret << endl;
			DEBUG_TRACE_INFOR(("vw ret: " + vw_ret).c_str());
		} else {
			DEBUG_TRACE_INFOR("wakeup fail");
			fout << endl;
		}
		def.initMsg();
	}

	string ret("");
	if (vw_time_sum) {
		ret = "vwLabel：" + vw_label + ", 尝试唤醒次数：" + lexical_cast<string>(vw_num) + ", 唤醒成功次数："
			+ lexical_cast<string>(vw_success_num) + "，成功率：" + lexical_cast<string>(((double)vw_success_num) / vw_num * 100)
			+ "%，平均唤醒响应时间：" + lexical_cast<string>(((double)vw_time_sum) / vw_vad_success_num);
	}
	else {
		ret = "vwLabel：" + vw_label + ", 尝试唤醒次数：" + lexical_cast<string>(vw_num) + ", 唤醒成功次数："
			+ lexical_cast<string>(vw_success_num) + "，成功率：" + lexical_cast<string>(((double)vw_success_num) / vw_num * 100)
			+ "%";
	}
	DEBUG_TRACE_INFOR(("vw batch ret: " + ret).c_str());
	fout << ret << endl;

	fout.close();
	ISSMVWAdapter::ISSMVWStop(phISSMVW);
}

double TestMVWBatch::getVadEndTime(string audio_path) {
	regex pattern(".*-(.*)s.*");
	smatch match;

	regex_match(audio_path, match, pattern);

	try {
		return 1000 * lexical_cast<double>(match[1].str());
	}
	catch (exception e) {
		return 0;
	}
}
