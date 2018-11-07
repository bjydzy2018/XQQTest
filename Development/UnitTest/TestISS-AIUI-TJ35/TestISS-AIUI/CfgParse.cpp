
#include "rapidjson/document.h"

#include "CfgParse.h"
#include "VAInc.h"
#include "test_log.h"
#include "CommonFun.h"

using std::string;
using std::pair;
using std::priority_queue;
using std::queue;
using std::map;
using std::vector;
using namespace rapidjson;
using namespace VA;

CfgParse::CfgParse(string cfg_path) : cfg_path_(cfg_path) {}

priority_queue<Job> CfgParse::getJobs() {
	priority_queue<Job> jobs;
	string cfg_data = readFile(cfg_path_, false);
	Document document;
	document.Parse(cfg_data.c_str());

	DEBUG_TRACE("Parse cfg_data");

	Value & test_to_run = document["test_to_run"];
	Value & tests = document["tests"];

	// 遍历每一个测试项
	for (SizeType i = 0; i < tests.Size(); i++) {
		Value & test = tests[i];

		string test_name;
		int priority = 1;
		map<string, string> cfgs;

		// 标记是否将当前测试任务添加到任务队列，判定的来源是'test_to_run'中该任务的设置值。
		bool is_into_jobs = true;

		// 遍历测试项的每一个配置
		for (Value::ConstMemberIterator itr = test.MemberBegin();
			itr != test.MemberEnd(); ++itr) {
			string key = itr->name.GetString();

			if (key == "name") {
				test_name = itr->value.GetString();

				int is_run = test_to_run[test_name.c_str()].GetInt();
				if (is_run == 0) {
					// 如果设置不执行该测试，则不添加到任务队列中
					is_into_jobs = false;
					break;
				}
			} else if (key == "priority") {
				priority = itr->value.GetInt();
			} else {
				cfgs.insert(pair<string, string>(key, itr->value.GetString()));
			}
		}

		if (is_into_jobs) {
			Job job = {test_name, priority, cfgs};
			jobs.push(job);
		}
	}

	return jobs;
}

vector<Job> CfgParse::getAllJobs() {
	vector<Job> jobs;
	string cfg_data = readFile(cfg_path_, false);
	Document document;
	document.Parse(cfg_data.c_str());

	Value & tests = document["tests"];
	Value & temp_tests = document["temp_tests"];

	// 遍历每一个测试项
	for (SizeType i = 0; i < tests.Size(); i++) {
		Value & test = tests[i];

		string test_name;
		int priority = 1;
		map<string, string> cfgs;

		// 遍历测试项的每一个配置
		for (Value::ConstMemberIterator itr = test.MemberBegin();
			itr != test.MemberEnd(); ++itr) {
			string key = itr->name.GetString();

			if (key == "name") {
				test_name = itr->value.GetString();
			}
			else if (key == "priority") {
				priority = itr->value.GetInt();
			}
			else {
				cfgs.insert(pair<string, string>(key, itr->value.GetString()));
			}
		}

		Job job = { test_name, priority, cfgs };
		jobs.push_back(job);
	}

	// 遍历临时测试项
	for (SizeType i = 0; i < temp_tests.Size(); i++) {
		Value & test = temp_tests[i];

		string test_name;
		map<string, string> cfgs;

		// 遍历测试项的每一个配置
		for (Value::ConstMemberIterator itr = test.MemberBegin();
			itr != test.MemberEnd(); ++itr) {
			string key = itr->name.GetString();

			if (key == "name") {
				test_name = itr->value.GetString();
			}
			else {
				cfgs.insert(pair<string, string>(key, itr->value.GetString()));
			}
		}

		Job job = { test_name, 0, cfgs };
		jobs.push_back(job);
	}

	return jobs;
}

queue<Job> CfgParse::getTempJobs() {
	queue<Job> jobs;
	string cfg_data = readFile(cfg_path_, false);
	Document document;
	document.Parse(cfg_data.c_str());

	Value & temp_test_to_run = document["temp_test_to_run"];
	Value & temp_tests = document["temp_tests"];

	// 遍历每一个测试项
	for (SizeType i = 0; i < temp_tests.Size(); i++) {
		Value & test = temp_tests[i];

		string test_name;
		map<string, string> cfgs;

		// 标记是否将当前测试任务添加到任务队列，判定的来源是'temp_test_to_run'中该任务的设置值。
		bool is_into_jobs = true;

		// 遍历测试项的每一个配置
		for (Value::ConstMemberIterator itr = test.MemberBegin();
			itr != test.MemberEnd(); ++itr) {
			string key = itr->name.GetString();

			if (key == "name") {
				test_name = itr->value.GetString();

				int is_run = temp_test_to_run[test_name.c_str()].GetInt();
				if (is_run == 0) {
					// 如果设置不执行该测试，则不添加到任务队列中
					is_into_jobs = false;
					break;
				}
			}
			else {
				cfgs.insert(pair<string, string>(key, itr->value.GetString()));
			}
		}

		if (is_into_jobs) {
			Job job = { test_name, 0, cfgs };
			jobs.push(job);
		}
	}

	return jobs;
}

map<string, string> CfgParse::getGlobalCfgs() {
	map<string, string> global_cfgs;
	string cfg_data = readFile(cfg_path_, false);
	Document document;
	document.Parse(cfg_data.c_str());

	Value & global_cfgs_tree = document["global_cfg"];

	for (Value::ConstMemberIterator itr = global_cfgs_tree.MemberBegin();
		itr != global_cfgs_tree.MemberEnd(); ++itr) {
		global_cfgs.insert(pair<string, string>(itr->name.GetString(), itr->value.GetString()));
	}

	return global_cfgs;
}
