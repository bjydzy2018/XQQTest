#include "JobCtrl.h"
#include "VAInc.h"
#include "test_log.h"
#include "gtest.h"
#include "TestTTSProcTime.h"
#include "TestMVWBatch.h"
#include "TestCATATime.h"
#include "TestSRApiRandom.h"
#include "TestMVWApiRandom.h"
#include "TestTTSApiRandom.h"
#include "TestCATAApiRandom.h"
#include "TestTTSText.h"
#include "TestCATAConsistent.h"
#include "TestCATA.h"
#include "TestSR.h"
#include "TestSRBatch.h"
#include "TestSPWordConsistent.h"
#include "TestSRConsistent.h"
#include "TestUpLoadDictBatch.h"
#include "TestSeoptBatch.h"

#include <vector>
#include <string>

#ifndef VA_VC
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <cstdlib>
#endif

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

using namespace VA;

using std::vector;
using std::string;

using boost::lexical_cast;
using namespace boost::algorithm;

// 解析配置文件，将要执行的测试项及其配置存到任务队列中。
JobCtrl::JobCtrl(int argc, char ** argv) : argc_(argc), argv_(argv), run_in_subprocess_(false) {
	CfgParse parse;
	global_cfgs_ = parse.getGlobalCfgs();
	map<string, string>::iterator iter;
	isslog_cfg default_cfg((testRes_path+"log/TestISS-AIUI.log").c_str(), "TestISS-AIUI log");

	// 解析全局配置
	// 配置log
	iter = global_cfgs_.find("log_level");
	if (iter != global_cfgs_.end())
		default_cfg.level(lexical_cast<int>(iter->second));
	else
		default_cfg.level(-1);

	iter = global_cfgs_.find("log_output");
	if (iter != global_cfgs_.end())
		default_cfg.output(lexical_cast<int>(iter->second));
	else
		default_cfg.output(2);

	default_cfg.flush_always(true);
	isslog_open(default_cfg);

	// 配置单独进程执行测试
#ifndef VA_VC
	iter = global_cfgs_.find("run_in_subprocess");
	if (iter != global_cfgs_.end())
		if (lexical_cast<int>(iter->second) == 1)
			run_in_subprocess_ = true;
#endif

	// 解析任务配置
	jobs_ = parse.getJobs();
	temp_jobs_ = parse.getTempJobs();
	all_jobs_ = parse.getAllJobs();
}

// 遍历任务队列，去执行每个测试任务
void JobCtrl::work() {
	if (argc_ > 1) { // 根据命令行参数执行测试
		for (int i = 1; i < argc_; i++) {
			string test_to_run = argv_[i];

			for (int j = 0; j < all_jobs_.size(); j++) {
				if (test_to_run == all_jobs_[j].name) {
					test(all_jobs_[j]);
					break;
				}
			}
		}
	}
	else { // 根据配置文件执行测试
		while (!jobs_.empty()) {
			Job job = jobs_.top();
#if VA_VC
			// 在windows平台使用单进程执行测试任务
			DEBUG_TRACE_INFOR("run tests in main process");
			test(job);
#else
			if (run_in_subprocess_) {
				// 在子进程执行测试任务
				DEBUG_TRACE_INFOR("run tests in child process");
				pid_t c_pid; // 子进程id
				pid_t pid; // wait()返回的id

				c_pid = fork();
				if (c_pid == 0) {
					// 子进程执行测试
					test(job);
					exit(0);
				}
				else if (c_pid > 0) {
					// 主进程等待子进程
					int status;
					if ((pid = wait(&status)) != -1 && pid == c_pid) {}
				}
				else {
					// 进程创建失败，使用单进程执行测试
					DEBUG_TRACE_ERROR("create child process fail, running test in main process");
					test(job);
				}
			}
			else {
				// 单进程执行测试任务
				DEBUG_TRACE_INFOR("run tests in main process");
				test(job);
			}
#endif
			jobs_.pop();
		}

		while (!temp_jobs_.empty()) {
			Job job = temp_jobs_.front();
#if VA_VC
			// 在windows平台使用单进程执行测试任务
			DEBUG_TRACE_INFOR("run temp_tests in main process");
			test(job);
#else
			if (run_in_subprocess_) {
				// 在子进程执行测试任务
				DEBUG_TRACE_INFOR("run temp_tests in child process");
				pid_t c_pid; // 子进程id
				pid_t pid; // wait()返回的id

				c_pid = fork();
				if (c_pid == 0) {
					// 子进程执行测试
					test(job);
					exit(0);
				}
				else if (c_pid > 0) {
					// 主进程等待子进程
					int status;
					if ((pid = wait(&status)) != -1 && pid == c_pid) {}
				}
				else {
					// 进程创建失败，使用单进程执行测试
					DEBUG_TRACE_ERROR("create child process fail, running temp_test in main process");
					test(job);
				}
			}
			else {
				// 单进程执行测试任务
				DEBUG_TRACE_INFOR("run temp_tests in main process");
				test(job);
			}
#endif
			temp_jobs_.pop();
		}
	}
}

// 根据测试任务名，执行对应的测试
void JobCtrl::test(Job & job) {
	if (job.name == "tts_api") {
		// 合成接口测试
		DEBUG_TRACE_INFOR("start TTS Api Test");
		testApi(job, "TTS");
	}
	else if (job.name == "mvw_api") {
		// 唤醒接口测试
		DEBUG_TRACE_INFOR("start MVW Api Test");
		testApi(job, "MVW");
	}
	else if (job.name == "cata_api") {
		// CATA接口测试
		DEBUG_TRACE_INFOR("start CATA Api Test");
		testApi(job, "CATA");
	}
	else if (job.name == "tts_proc_time") {
		// 首次合成响应时间测试
		DEBUG_TRACE_INFOR("start TTS proc time Test");
		testTTSProcTime(job);
	}
	else if (job.name == "tts_api_random") {
		// 合成随机调接口测试
		DEBUG_TRACE_INFOR("start TTS Api Random Test");
		testTTSApiRandom(job);
	}
	else if (job.name == "mvw_batch_by_scene") {
		// 按场景测试批量唤醒
		DEBUG_TRACE_INFOR("start MVW Batch Test By Scene");
		testMVWBatchByScene(job);
	}
	else if (job.name == "mvw_batch_by_word") {
		// 按唤醒词测试批量唤醒
		DEBUG_TRACE_INFOR("start MVW Batch Test By Word");
		testMVWBatchByWord(job);
	}
	else if (job.name == "mvw_consistent") {
		// 按唤醒词测试批量唤醒
		DEBUG_TRACE_INFOR("start MVW Consistent Test");
		testMVWConsistent(job);
	}
	else if (job.name == "mvw_api_random") {
		// 唤醒随机调接口测试
		DEBUG_TRACE_INFOR("start MVW Api Random Test");
		testMVWApiRandom(job);
	}
	else if (job.name == "cata_time") {
		// CATA响应时间测试
		DEBUG_TRACE_INFOR("start CATA Time Test");
		testCATATime(job);
	}
	else if (job.name == "cata_consistent") {
		// CATA一致性测试
		DEBUG_TRACE_INFOR("start CATA Consistent Test");
		testCATAConsistent(job);
	}
	else if (job.name == "cata_api_random") {
		// CATA随机调接口测试
		DEBUG_TRACE_INFOR("start CATA Api Random Test");
		testCATAApiRandom(job);
	}
	else if (job.name == "sr_api") {
		// 识别接口测试
		DEBUG_TRACE_INFOR("start SR Api Test");
		testApi(job, "SR");
	}
	else if (job.name == "sr_api_random") {
		// 识别随机调接口测试
		DEBUG_TRACE_INFOR("start SR Api Random Test");
		testSRApiRandom(job);
	}
	else if (job.name == "sr_batch") {
		// 批量识别测试
		testSRBatch(job);
	}
	else if (job.name == "tts_text") {
		// 合成文本测试
		DEBUG_TRACE_INFOR("start tts some text");
		testTTSText(job);
	}
	else if (job.name == "tts_text_by_list") {
		// 合成文本测试
		DEBUG_TRACE_INFOR("start tts some text by list");
		testTTSTextByList(job);
	}
	else if (job.name == "cata_index_search") {
		// cata构建搜索测试
		DEBUG_TRACE_INFOR("start cata index and search");
		testCATAIndexSearch(job);
	}
	else if (job.name == "cata_search") {
		// cata构建搜索测试
		DEBUG_TRACE_INFOR("start cata search");
		testCATASearch(job);
	}
	else if (job.name == "sr_audio") {
		// 识别一条音频
		DEBUG_TRACE_INFOR("start sr an audio");
		testSR(job);
	}
	else if (job.name == "search_api") {
		// 搜索接口测试
		DEBUG_TRACE_INFOR("start Search Api Test");
		testApi(job, "Search");
	}
	else if (job.name == "spword_consistent") {
		// 分词一致性测试
		DEBUG_TRACE_INFOR("start SPWord Consistent Test");
		testSPWordConsistent(job);
	}
	else if (job.name == "sr_consistent") {
		// 识别+语义一致性测试
		DEBUG_TRACE_INFOR("start SR Consistent Test");
		testSRConsistent(job);
	}
	else if (job.name == "uploaddict_batch") {
		// 批量上传词典测试
		DEBUG_TRACE_INFOR("start uploaddict batch Test");
		testUpLoadDictBatch(job);
	}
	else if (job.name == "seopt_batch") {
		// 批量窄波束降噪测试
		DEBUG_TRACE_INFOR("start seopt batch Test");
		testSeoptBatch(job);
	}
	else if (job.name == "seopt_api") {
		// 识别接口测试
		DEBUG_TRACE_INFOR("start Seopt Api Test");
		testApi(job, "Seopt");
	}

}

int JobCtrl::testApi(Job & job, string module_name) {
	map<string, string>::iterator iter;

	// 执行哪些测试
	iter = job.cfgs.find("filter");
	string filter_str("");
	if (iter != job.cfgs.end())
		filter_str = iter->second;
	else
		filter_str = "*" + module_name + "*.*";
	DEBUG_TRACE_INFOR((module_name + " api test filter: " + filter_str).c_str());
	testing::GTEST_FLAG(filter) = filter_str;

	// 测试几遍
	iter = job.cfgs.find("repeat");
	int repeat_num = 0;
	if (iter != job.cfgs.end())
		repeat_num = lexical_cast<int>(iter->second);
	else
		repeat_num = 1;
	DEBUG_TRACE_INFOR((module_name + " api test repeat: " + lexical_cast<string>(repeat_num)).c_str());
	testing::GTEST_FLAG(repeat) = repeat_num;

	// break_on_failure
	iter = job.cfgs.find("break_on_failure");
	if (iter->second == "1") {
		DEBUG_TRACE_INFOR((module_name + " api test break_on_failure on").c_str());
		testing::GTEST_FLAG(break_on_failure) = true;
	}
	else {
		DEBUG_TRACE_INFOR((module_name + " api test break_on_failure off").c_str());
	}

	DEBUG_TRACE_INFOR((module_name + " api test log file: TestApi" + module_name + ".xml").c_str());

#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	testing::GTEST_FLAG(output) = "xml:/data/AIUI/armeabi-v7a/TestApi" + module_name + ".xml";
#else
	testing::GTEST_FLAG(output) = "xml:TestApi" + module_name + ".xml";
#endif

	return RUN_ALL_TESTS();
}

void JobCtrl::testTTSProcTime(Job & job) {
#ifdef __ANDROID__

	TestTTSProcTime t;
	map<string, string>::iterator iter;
	int loop_num = 1;
	iter = job.cfgs.find("loop_num");
	if (iter != job.cfgs.end())
		loop_num = lexical_cast<int>(iter->second);
	t.setLoopNum(loop_num);
	t.test();
#endif
}

void JobCtrl::testMVWBatchByScene(Job & job) {
	TestMVWBatch t;
	map<string, string>::iterator iter;

	string use_path_pre("");
	iter = job.cfgs.find("use_path_pre");
	if (iter != job.cfgs.end()) {
		use_path_pre = iter->second;
	}

	t.setUsePathPre(use_path_pre);

	iter = job.cfgs.find("scenes");
	if (iter != job.cfgs.end()) {
		vector<string> scenes;
		split(scenes, iter->second, is_any_of(","));

		string scenes_str(scenes[0]);
		for (int i = 1; i < scenes.size(); i++) {
			scenes_str += ", " + scenes[i];
		}
		DEBUG_TRACE_INFOR(("MVW batch test by scenes: " + scenes_str).c_str());

		t.testMVWBatchByScene(scenes);
	}
	else {
		DEBUG_TRACE_INFOR("MVW batch test by default scenes: 1, 2, 4, 8" );
		t.testMVWBatchByScene(4, 1, 2, 4, 8);
	}
}

void JobCtrl::testMVWBatchByWord(Job & job) {
	TestMVWBatch t;
	map<string, string>::iterator iter;

	string use_path_pre("");
	iter = job.cfgs.find("use_path_pre");
	if (iter != job.cfgs.end()) {
		use_path_pre = iter->second;
	}

	t.setUsePathPre(use_path_pre);
	t.testMVWBatchByWord();
}

void JobCtrl::testMVWConsistent(Job &) {
	TestMVWBatch t;
	t.testMVWConsistent();
}

void JobCtrl::testCATATime(Job & job) {
	TestCATATime t;
	t.test();
}

void JobCtrl::testCATAConsistent(Job & job) {
	TestCATAConsistent t;

	map<string, string>::iterator iter;

	iter = job.cfgs.find("index_list_dir");
	if (iter != job.cfgs.end()) {
		t.setIndexListDir(string(PrtU8((iter->second).c_str())));
	}

	iter = job.cfgs.find("search_list");
	if (iter != job.cfgs.end()) {
		t.setSearchList(string(PrtU8((iter->second).c_str())));
	}

	t.test();
}

void JobCtrl::testSRApiRandom(Job & job) {
	TestSRApiRandom t;
	map<string, string>::iterator iter;

	iter = job.cfgs.find("run_time");
	if (iter != job.cfgs.end())
		t.setRunTime(lexical_cast<double>(iter->second));
	t.test();
}

void JobCtrl::testSRBatch(Job & job) {
	map<string, string>::iterator iter;

	int test_mode = 0;
	int lang = 0;
	int mode = 0;
	int ifOnlyUploadToCloud = 0;
	int loop_num = 0;
	string ch_audio_list_path("");
	string ch_time_audio_list_path("");
	string en_audio_list_path("");
	string ch_dict_path("");
	string en_dict_path("");
	string machineCode("");

	iter = job.cfgs.find("lang");
	lang = lexical_cast<int>(iter->second);

	iter = job.cfgs.find("test_mode");
	test_mode = lexical_cast<int>(iter->second);

	iter = job.cfgs.find("ch_audio_list_path");
	ch_audio_list_path = iter->second;

	iter = job.cfgs.find("ch_time_audio_list_path");
	ch_time_audio_list_path = iter->second;

	iter = job.cfgs.find("ch_dict_path");
	if (iter != job.cfgs.end())
		ch_dict_path = iter->second;

	iter = job.cfgs.find("en_audio_list_path");
	en_audio_list_path = iter->second;

	iter = job.cfgs.find("en_dict_path");
	if (iter != job.cfgs.end())
		en_dict_path = iter->second;

	iter = job.cfgs.find("machineCode");
	if (iter != job.cfgs.end())
		machineCode = iter->second;

	iter = job.cfgs.find("mode");
	mode = lexical_cast<int>(iter->second);

	iter = job.cfgs.find("ifOnlyUploadToCloud");
	ifOnlyUploadToCloud = lexical_cast<int>(iter->second);

	iter = job.cfgs.find("loop_num");
	loop_num = lexical_cast<int>(iter->second);

	DEBUG_TRACE("test_mode:%d", test_mode);

	// 中文识别效果测试
	if (test_mode & (1 << 0)) {
		DEBUG_TRACE_INFOR("start ch sr effect test");
		TestSRBatch t;
		t.setLoopNum(loop_num);
		t.setMachineCode(machineCode);

		if (ch_dict_path == "")
			t.test(lang, mode, ch_audio_list_path, "effect");
		else
			t.test(lang, mode, ch_audio_list_path, "effect", ch_dict_path, ifOnlyUploadToCloud);
	}

	// 英文识别效果测试
	if (test_mode & (1 << 1)) {
		DEBUG_TRACE_INFOR("start en sr effect test");
		TestSRBatch t;
		t.setLoopNum(loop_num);
		t.setMachineCode(machineCode);

		if (ch_dict_path == "")
			t.test(lang, mode, en_audio_list_path, "effect");
		else
			t.test(lang, mode, en_audio_list_path, "effect", en_dict_path, ifOnlyUploadToCloud);
	}

	// 中文识别响应时间测试
	if (test_mode & (1 << 2)) {
		DEBUG_TRACE_INFOR("start ch sr time test");
		TestSRBatch t;
		t.setLoopNum(loop_num);
		t.setMachineCode(machineCode);

		t.test(lang, mode, ch_time_audio_list_path, "time");
	}
}

void JobCtrl::testMVWApiRandom(Job & job) {
	TestMVWApiRandom t;
	map<string, string>::iterator iter;

	iter = job.cfgs.find("run_time");
	if (iter != job.cfgs.end())
		t.setRunTime(lexical_cast<double>(iter->second));
	t.test();
}

void JobCtrl::testTTSApiRandom(Job & job) {
#ifdef __ANDROID__
	TestTTSApiRandom t;
	map<string, string>::iterator iter;

	iter = job.cfgs.find("run_time");
	if (iter != job.cfgs.end())
		t.setRunTime(lexical_cast<double>(iter->second));
	t.test();
#endif
}

void JobCtrl::testCATAApiRandom(Job & job) {
	TestCATAApiRandom t;
	map<string, string>::iterator iter;

	iter = job.cfgs.find("run_time");
	if (iter != job.cfgs.end())
		t.setRunTime(lexical_cast<double>(iter->second));
	t.test();
}

void JobCtrl::testTTSText(Job & job) {
#ifdef __ANDROID__
	TestTTSText t;
	map<string, string>::iterator iter;

	iter = job.cfgs.find("text");
	if (iter != job.cfgs.end())
		t.setText(iter->second);

	iter = job.cfgs.find("speaker");
	if (iter != job.cfgs.end())
		t.setSpeaker(lexical_cast<int>(iter->second));

	iter = job.cfgs.find("tts_audio_path");
	if (iter != job.cfgs.end())
		t.setTTSAudioPath(iter->second);

	t.test();
#endif
}

void JobCtrl::testTTSTextByList(Job & job) {
#ifdef __ANDROID__
	TestTTSText t;
	map<string, string>::iterator iter;

	iter = job.cfgs.find("speaker");
	if (iter != job.cfgs.end())
		t.setSpeaker(lexical_cast<int>(iter->second));

	iter = job.cfgs.find("tts_audio_path");
	if (iter != job.cfgs.end())
		t.setTTSAudioPath(iter->second);

	iter = job.cfgs.find("text_list_path");
	if (iter != job.cfgs.end())
		t.test(iter->second);
#endif
}

void JobCtrl::testCATAIndexSearch(Job & job) {
	TestCATA t;
	map<string, string>::iterator iter;

	string index_res;
	string index_text;
	string srh_res;
	string srh_text;

	iter = job.cfgs.find("index_res");
	index_res = iter->second;

	iter = job.cfgs.find("index_text");
	index_text = iter->second;

	iter = job.cfgs.find("srh_res");
	srh_res = iter->second;

	iter = job.cfgs.find("srh_text");
	srh_text = iter->second;

	t.testIndexSearch(index_text, index_res, srh_text, srh_res);
}

void JobCtrl::testCATASearch(Job & job) {
	TestCATA t;
	map<string, string>::iterator iter;

	string index_res;
	string index_text;
	string srh_res;
	string srh_text;

	iter = job.cfgs.find("srh_res");
	srh_res = iter->second;

	iter = job.cfgs.find("srh_text");
	srh_text = iter->second;

	t.testSearch(srh_text, srh_res);
}

void JobCtrl::testSR(Job & job) {
	TestSR t;
	map<string, string>::iterator iter;

	int online = 0;
	string audio_path("");
	int lang = 0;
	string scene("");
	int mode = 0;
	string szCmd("");
	string dict_path("");
	int ifOnlyUploadToCloud = 0;

	iter = job.cfgs.find("online");
	online = lexical_cast<int>(iter->second);

	iter = job.cfgs.find("audio_path");
	audio_path = string(PrtU8((iter->second).c_str()));

	iter = job.cfgs.find("lang");
	lang = lexical_cast<int>(iter->second);

	iter = job.cfgs.find("scene");
	scene = iter->second;

	iter = job.cfgs.find("mode");
	mode = lexical_cast<int>(iter->second);

	iter = job.cfgs.find("szCmd");
	szCmd = iter->second;

	iter = job.cfgs.find("dict_path");
	if (iter != job.cfgs.end())
		dict_path = iter->second;

	iter = job.cfgs.find("ifOnlyUploadToCloud");
	ifOnlyUploadToCloud = lexical_cast<int>(iter->second);

	if (!online) {
		if (dict_path == "")
			t.test_sr(audio_path, lang, scene, mode, szCmd);
		else
			t.test_sr(audio_path, lang, scene, mode, szCmd, dict_path, ifOnlyUploadToCloud);
	}
	else {
#if VA_VC
		if (dict_path == "")
			t.test_sr_online(lang, scene, mode, szCmd);
		else
			t.test_sr_online(lang, scene, mode, szCmd, dict_path, ifOnlyUploadToCloud);
#endif
	}
}

void JobCtrl::testSPWordConsistent(Job & job) {
#ifdef ISS_SR_3

	TestSPWordConsistent t;
	map<string, string>::iterator iter;

	iter = job.cfgs.find("test_text_list_path");
	string test_text_list_path = iter->second;

	t.setTestTextListPath(testRes_path + test_text_list_path);

	t.test();

#endif
}

void JobCtrl::testSRConsistent(Job & job) {
#ifndef ISS_SR_3
	map<string, string>::iterator iter;

	int test_mode = 0;
	int lang = 0;
	int mode = 0;
	int ifOnlyUploadToCloud = 0;
	int loop_num = 0;
	string ch_audio_list_path("");
	string en_audio_list_path("");
	string ch_dict_path("");
	string en_dict_path("");
	string ch_user_data_path("");
	string en_user_data_path("");
	string machineCode("");

	iter = job.cfgs.find("lang");
	lang = lexical_cast<int>(iter->second);

	iter = job.cfgs.find("test_mode");
	test_mode = lexical_cast<int>(iter->second);

	iter = job.cfgs.find("ch_audio_list_path");
	ch_audio_list_path = iter->second;

	iter = job.cfgs.find("ch_dict_path");
	if (iter != job.cfgs.end())
		ch_dict_path = iter->second;

	iter = job.cfgs.find("en_audio_list_path");
	en_audio_list_path = iter->second;

	iter = job.cfgs.find("en_dict_path");
	if (iter != job.cfgs.end())
		en_dict_path = iter->second;

	iter = job.cfgs.find("ch_user_data_path");
	if (iter != job.cfgs.end())
		ch_user_data_path = iter->second;

	iter = job.cfgs.find("en_user_data_path");
	if (iter != job.cfgs.end())
		en_user_data_path = iter->second;

	iter = job.cfgs.find("mode");
	mode = lexical_cast<int>(iter->second);

	iter = job.cfgs.find("ifOnlyUploadToCloud");
	ifOnlyUploadToCloud = lexical_cast<int>(iter->second);

	iter = job.cfgs.find("loop_num");
	loop_num = lexical_cast<int>(iter->second);
	DEBUG_TRACE("loop_num:%d", loop_num);

	iter = job.cfgs.find("machineCode");
	if (iter != job.cfgs.end())
		machineCode = iter->second;

	TestSRConsistent t;
	t.setLoopNum(loop_num);
	t.setMachineCode(machineCode);
	// 中文识别效果测试
	if (test_mode & (1 << 0)) {
		t.setUserDataPath(ch_user_data_path);
		DEBUG_TRACE_INFOR("start ch sr effect test");
		if (ch_dict_path == "")
			t.test(lang, mode, ch_audio_list_path, "effect");
		else
			t.test(lang, mode, ch_audio_list_path, "effect", ch_dict_path, ifOnlyUploadToCloud);
	}

	// 英文识别效果测试
	if (test_mode & (1 << 1)) {
		t.setUserDataPath(en_user_data_path);
		DEBUG_TRACE_INFOR("start en sr effect test");
		if (ch_dict_path == "")
			t.test(lang, mode, en_audio_list_path, "effect");
		else
			t.test(lang, mode, en_audio_list_path, "effect", en_dict_path, ifOnlyUploadToCloud);
	}
#endif

}

void JobCtrl::testUpLoadDictBatch(Job & job) {
	map<string, string>::iterator iter;

	int lang = 0;
	string dict_path("");
	string dict_name("");
	string machineCode("");
	int ifOnlyUploadToCloud = 0;
	int ifEveryTimeNeedCreate = 0;
	int loop_num = 0;

	iter = job.cfgs.find("lang");
	lang = lexical_cast<int>(iter->second);

	iter = job.cfgs.find("dict_path");
	dict_path = iter->second;

	iter = job.cfgs.find("dict_name");
	dict_name = iter->second;

	iter = job.cfgs.find("ifOnlyUploadToCloud");
	ifOnlyUploadToCloud = lexical_cast<int>(iter->second);

	iter = job.cfgs.find("ifEveryTimeNeedCreate");
	ifEveryTimeNeedCreate = lexical_cast<int>(iter->second);

	iter = job.cfgs.find("loop_num");
	loop_num = lexical_cast<int>(iter->second);

	iter = job.cfgs.find("machineCode");
	if (iter != job.cfgs.end())
		machineCode = iter->second;

	TestUpLoadDictBatch t;
	t.setLoopNum(loop_num);
	t.setMachineCode(machineCode);
	t.test(lang,dict_path,dict_name,ifOnlyUploadToCloud,ifEveryTimeNeedCreate);

}

void JobCtrl::testSeoptBatch(Job & job) {

	map<string, string>::iterator iter;
	string audio_list_path("");
	string audio_list_path_0("");
	string audio_out_path("");
	int SeoptMode = 0;
	int IfNeedSleep = 0;
	int loop_num = 1;

	iter = job.cfgs.find("audio_list_path");
	if (iter != job.cfgs.end())
		audio_list_path = iter->second;

	iter = job.cfgs.find("audio_list_path_0");
	if (iter != job.cfgs.end())
		audio_list_path_0 = iter->second;

	iter = job.cfgs.find("audio_out_path");
	if (iter != job.cfgs.end())
		audio_out_path = iter->second;

	iter = job.cfgs.find("SeoptMode");
	if (iter != job.cfgs.end())
		SeoptMode = atoi(iter->second.c_str());

	iter = job.cfgs.find("IfNeedSleep");
	if (iter != job.cfgs.end())
		IfNeedSleep = atoi(iter->second.c_str());
	DEBUG_TRACE("IfNeedSleep:%d", IfNeedSleep);

	iter = job.cfgs.find("loop_num");
	if (iter != job.cfgs.end())
		loop_num = atoi(iter->second.c_str());
	DEBUG_TRACE("loop_num:%d", loop_num);

	TestSeoptBatch t;
	for (int i = 0;i < loop_num;i++) {
		DEBUG_TRACE("test_num:%d", i);
		t.SetParam(SeoptMode);
		if (SeoptMode == 0) {
			t.test(audio_list_path_0, audio_out_path, IfNeedSleep);
		}
		else {
			t.test(audio_list_path, audio_out_path, IfNeedSleep);
		}

	}
	return;


}
