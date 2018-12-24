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
	//vw_ret_name = "SceneId-" + scene_str + "/�����������_ret.txt";
	//testMVWBatch("1-�����������", false);
	//
	//scene = 2;
	//scene_str = lexical_cast<string>(scene);
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/ȷ��.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/ȷ��_ret.txt";
	//testMVWBatch("2-ȷ��", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/ȡ��.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/ȡ��_ret.txt";
	//testMVWBatch("2-ȡ��", false);
	//
	//scene = 4;
	//scene_str = lexical_cast<string>(scene);
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/��һ��.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/��һ��_ret.txt";
	//testMVWBatch("4-��һ��", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/�ڶ���.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/�ڶ���_ret.txt";
	//testMVWBatch("4-�ڶ���", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/������.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/������_ret.txt";
	//testMVWBatch("4-������", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/���ĸ�.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/���ĸ�_ret.txt";
	//testMVWBatch("4-���ĸ�", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/�����.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/�����_ret.txt";
	//testMVWBatch("4-�����", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/������.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/������_ret.txt";
	//testMVWBatch("4-������", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/���һ��.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/���һ��_ret.txt";
	//testMVWBatch("4-���һ��", false);
	//
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/ȡ��.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/ȡ��_ret.txt";
	//testMVWBatch("4-ȡ��", false);

	//scene = 8;
	//scene_str = lexical_cast<string>(scene);
	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/����.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/����_ret.txt";
	//testMVWBatch("8-����", false);

	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/�Ҷ�.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/�Ҷ�_ret.txt";
	//testMVWBatch("8-�Ҷ�", false);

	//audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/ȡ��.txt";
	//vw_ret_name = "SceneId-" + scene_str + "/ȡ��_ret.txt";
	//testMVWBatch("8-ȡ��", false);

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
	audio_list_path = "\\\\192.168.28.101\\�׼����Լ�\\����\\Daily-Consistent\\һ���Բ��Լ�\\С�����\\xanh_iss.txt";
	vw_ret_name = "С�����_ret.txt";
	testMVWBatch("1-С�����", true);

	scene = 1;
	audio_list_path = "\\\\192.168.28.101\\�׼����Լ�\\����\\Daily-Consistent\\һ���Բ��Լ�\\�����������\\nhyyzl_iss.txt";
	vw_ret_name = "�����������_ret.txt";
	testMVWBatch("1-�����������", true);

	scene = 2;
	audio_list_path = "\\\\192.168.28.101\\�׼����Լ�\\����\\Daily-Consistent\\һ���Բ��Լ�\\ȡ��\\ȡ��_iss.lst";
	vw_ret_name = "ȡ��_ret.txt";
	testMVWBatch("2-ȡ��", true);

	scene = 4;
	ISSMVWAdapter::ISSMVWDestroy(phISSMVW);
	phISSMVW = NULL;
	err = ISSMVWAdapter::ISSMVWSetMvwLanguage(ISS_MVW_LANG_ENG);
	isslog_func_return(err, ("ISSMVWSetMvwLanguage return %d", err));

	err = ISSMVWAdapter::ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL);
	isslog_func_return(err, ("ISSMVWCreate return %d", err));
	audio_list_path = "\\\\192.168.28.101\\�׼����Լ�\\����\\Daily-Consistent\\һ���Բ��Լ�\\cancel\\cancel_iss.lst";
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
	int vw_num = 0; // ���Ի��Ѵ���
	int vw_success_num = 0; // ���ѳɹ�����
	int vw_vad_success_num = 0; // ��ע�˺�˵����Ƶ�Ļ��ѳɹ�����
	double vad_end_time = 0; // ��˵�
	double vw_time = 0; // ������Ӧʱ��
	double vw_time_sum = 0; // �ܻ�����Ӧʱ��

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
		ret = "vwLabel��" + vw_label + ", ���Ի��Ѵ�����" + lexical_cast<string>(vw_num) + ", ���ѳɹ�������"
			+ lexical_cast<string>(vw_success_num) + "���ɹ��ʣ�" + lexical_cast<string>(((double)vw_success_num) / vw_num * 100)
			+ "%��ƽ��������Ӧʱ�䣺" + lexical_cast<string>(((double)vw_time_sum) / vw_vad_success_num);
	}
	else {
		ret = "vwLabel��" + vw_label + ", ���Ի��Ѵ�����" + lexical_cast<string>(vw_num) + ", ���ѳɹ�������"
			+ lexical_cast<string>(vw_success_num) + "���ɹ��ʣ�" + lexical_cast<string>(((double)vw_success_num) / vw_num * 100)
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
