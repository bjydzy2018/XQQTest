#include "TestMVWBatch.h"
#include "CommonFun.h"
#include "test_log.h"
#include "VAInc.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdarg>

using namespace VA;
using namespace std;
using std::vector;
using std::string;
using std::endl;


int ParseMvw2List(string Mvw2List,vector<string> &vec,vector<pair<string,int> > &vec_pair)
{
	vec = readFileByLine(Mvw2List,false);
	pair<string,int> mPair;
	string Tempstr;

	for(size_t i=0;i<vec.size();i++){
		Tempstr = vec[i];
		//DEBUG_TRACE(PrtU8(Tempstr.c_str()));
		std::size_t found = Tempstr.find_last_of("/\\");
		std::size_t found2 = Tempstr.find_first_of("-",0);
		//mPair.second = atoi((Tempstr.substr(found-1,found)).c_str());
		mPair.second = atoi((Tempstr.substr(found2+1,found)).c_str());
		/*DEBUG_TRACE("%d",mPair.second);*/

		Tempstr= Tempstr.substr(found+1);
		vector<string> Temp_v = split(Tempstr, '.');
		mPair.first = Temp_v[0];
		//DEBUG_TRACE(PrtU8(mPair.first.c_str()));

		vec_pair.push_back(mPair);
	}
	return 0;

}


TestMVWBatch::TestMVWBatch() : err(-1), phISSMVW(NULL),  scene(1) {
	string scene_str = LongToString(scene);
	audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/SceneId-" + scene_str + ".txt";
	vw_ret_name = "SceneId-" + scene_str + "/SceneId-" + scene_str + "_ret.txt";

	err = ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL);
}

void TestMVWBatch::testMVWBatchByScene(int scenes_count, ...) {
	va_list args;
	va_start(args, scenes_count);

	while (scenes_count--) {
		scene = va_arg(args, int);
		string scene_str = LongToString(scene);

		audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scene_str + "/SceneId-" +
			scene_str + ".txt";
		vw_ret_name = "SceneId-" + scene_str + "/SceneId-" + scene_str + "_ret.txt";

		testMVWBatch("scene(" + scene_str + ")", false);
	}

	ISSMVWDestroy(phISSMVW);
	phISSMVW = NULL;
}

void TestMVWBatch::testMVWBatchByScene(vector<string> & scenes) {
	for(size_t i = 0; i < scenes.size(); ++i){
		scene = atoi(scenes[i].c_str());

		audio_list_path = "TestRes/mvw/batch/list/SceneId-" + scenes[i] + "/SceneId-" +
			scenes[i] + ".txt";
		vw_ret_name = "SceneId-" + scenes[i] + "/SceneId-" + scenes[i] + "_ret.txt";

		testMVWBatch("scene(" + scenes[i] + ")", false);
	}

	ISSMVWDestroy(phISSMVW);
	phISSMVW = NULL;
}

void TestMVWBatch::testMVWBatchByWord() {
	string scene_str = "";

	vector<string> vec;
	vector<pair<string,int> > vec_pair;

	ParseMvw2List("TestRes/mvw/batch/list/mvw_list_list.txt",vec,vec_pair);
	for(size_t i=0;i<vec.size();i++){
		scene = vec_pair[i].second;
		scene_str = LongToString(scene);
		audio_list_path = "TestRes/"+vec[i];
		vw_ret_name = "SceneId-" + scene_str + "/" +"SceneId-" + scene_str + "_" +
			vec_pair[i].first + "_ret.txt";
		testMVWBatch(scene_str + "-" + vec_pair[i].first, false);
	}

	ISSMVWStop(phISSMVW);

	ISSMVWDestroy(phISSMVW);
	phISSMVW = NULL;
}

int TestMVWBatch::testMVWConsistent() {
	scene = 8;
	audio_list_path = "\\\\10.5.22.77\\套件测试集\\唤醒\\Daily-Consistent\\一致性测试集\\小艾你好\\xanh_iss.txt";
	vw_ret_name = "小艾你好_ret.txt";
	testMVWBatch("1-小艾你好", true);

	scene = 1;
	audio_list_path = "\\\\10.5.22.77\\套件测试集\\唤醒\\Daily-Consistent\\一致性测试集\\你好语音助理\\nhyyzl_iss.txt";
	vw_ret_name = "你好语音助理_ret.txt";
	testMVWBatch("1-你好语音助理", true);

	scene = 2;
	audio_list_path = "\\\\10.5.22.77\\套件测试集\\唤醒\\Daily-Consistent\\一致性测试集\\取消\\取消_iss.lst";
	vw_ret_name = "取消_ret.txt";
	testMVWBatch("2-取消", true);

	scene = 4;
	ISSMVWDestroy(phISSMVW);
	phISSMVW = NULL;
	err = ISSMVWSetMvwLanguage(ISS_MVW_LANG_ENG);
	isslog_func_return(err, ("ISSMVWSetMvwLanguage return %d", err));

	err = ISSMVWCreate(&phISSMVW, def.resDir, DefMVW::onWakeup, NULL, NULL);
	isslog_func_return(err, ("ISSMVWCreate return %d", err));
	audio_list_path = "\\\\10.5.22.77\\套件测试集\\唤醒\\Daily-Consistent\\一致性测试集\\cancel\\cancel_iss.lst";
	vw_ret_name = "cancel_ret.txt";
	testMVWBatch("2-cancel", true);

	ISSMVWDestroy(phISSMVW);
	phISSMVW = NULL;

	return 0;
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

	vector<string> audio_list = readFileByLine(string(PrtU8(audio_list_path.c_str())), false);
	ofstream fout(PrtU8(vw_ret_path.c_str()), ios::out);

	if (!is_test_consistent) {
		DEBUG_TRACE("test mvw batch effect, start only once");

		err = ISSMVWStart(phISSMVW, scene);
		DEBUG_TRACE("ISSMVWStart return %d",err);
		DEBUG_TRACE("MVWscene is %d",scene);
	} else
		DEBUG_TRACE("test mvw batch consistent, start before appending audio data each time");

	for(size_t i = 0; i < audio_list.size(); ++i){
		string audio = audio_list[i];
		if (is_test_consistent) {
			err = ISSMVWStart(phISSMVW, scene);
			DEBUG_TRACE("ISSMVWStart return %d",err);
		}
		if(!is_test_consistent&&use_path_pre_ != ""){
				DEBUG_TRACE_INFOR("use_path_pre_:%s",use_path_pre_.c_str());
				audio = use_path_pre_ + audio;
		}
		vw_num++;

		DEBUG_TRACE_INFOR(("test number:" + LongToString(vw_num) + ", success rate: " + 
			DoubleToString(vw_num == 1 ? 0 : ((double)vw_success_num) / (vw_num - 1) * 100) + "%%").c_str());

		//fout << string(PrtU8(audio.c_str())) ;

		fout << audio ;

		DEBUG_TRACE("audio:%s",PrtU8(audio.c_str()));

		vad_end_time = getVadEndTime(audio);
		DEBUG_TRACE_INFOR("getVadEndTime return %d", vad_end_time);

		appendAudio(phISSMVW, "mvw", string(PrtU8(audio.c_str())), def);

		if (def.msgWakeup) {
			vw_time = def.vw_time - vad_end_time;
			vw_success_num++;
			string vw_ret = "\t";
			vw_ret += "nMvwScene:" + LongToString(def.wakeupInfo.getnMvwScene()) + ",";
			vw_ret += "nMvwId:" + LongToString(def.wakeupInfo.getnMvwId()) + ",";
			vw_ret += "nMvwScore:" + LongToString(def.wakeupInfo.getnMvwScore()) + ",";
#if VA_VC
			vw_ret += "lParam:" + string(GBKStrToU8Str(def.wakeupInfo.getlParam().c_str()));
#else
			vw_ret += "lParam:" + def.wakeupInfo.getlParam();
#endif

			if (vad_end_time > 0) {
				DEBUG_TRACE_INFOR(("get vad end time: " + DoubleToString(vad_end_time)).c_str());

				if (vw_time > 0) {
					vw_vad_success_num++;
					vw_time_sum += vw_time;
					vw_ret += "\t" + DoubleToString(vw_time);
				}
			}

			DEBUG_TRACE("vw ret: %s",vw_ret.c_str());
			fout << vw_ret << endl;

		} else {
			DEBUG_TRACE_INFOR("wakeup fail");
			fout << endl;
		}
		def.initMsg();
	}

	string ret("");
	if (vw_time_sum) {
		ret = "vwLabel：" + string(PrtU8(vw_label.c_str())) + ", 尝试唤醒次数：" + LongToString(vw_num) + ", 唤醒成功次数："
			+ LongToString(vw_success_num) + "，成功率：" + DoubleToString(((double)vw_success_num) / vw_num * 100)
			+ "%，平均唤醒响应时间：" + DoubleToString(((double)vw_time_sum) / vw_vad_success_num);
	}
	else {
		ret = "vwLabel：" + string(PrtU8(vw_label.c_str())) + ", 尝试唤醒次数：" + LongToString(vw_num) + ", 唤醒成功次数："
			+ LongToString(vw_success_num) + "，成功率：" + DoubleToString(((double)vw_success_num) / vw_num * 100)
			+ "%";
	}

	DEBUG_TRACE_INFOR("vw batch ret: %s",ret.c_str());

#if VA_VC
	fout << string(GBKStrToU8Str(ret.c_str())) << endl;
#else
	fout << ret << endl;
#endif

	fout.close();

	ISSMVWStop(phISSMVW);
}

double TestMVWBatch::getVadEndTime(string audio_path) {
	double VadEnd = 0;

	replace_all(audio_path, "\\", "/");
	vector<string> path_parts = split( audio_path, '/');
	string audio_name = path_parts[path_parts.size() - 1];

	if((audio_name.find("-")!=string::npos)&&(audio_name.find("s")!=string::npos))
	{
		vector<string> temp = split( audio_name, '-');
		string t = temp[1];
		vector<string> time = split(t, 's');
		VadEnd = atof(time[0].c_str()) * 1000;
		return VadEnd;
	}

	return 0;
}
