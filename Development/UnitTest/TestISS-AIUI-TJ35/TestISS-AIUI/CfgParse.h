#ifndef CFGPARSE_H_
#define CFGPARSE_H_

#include "Def.h"

#include <string>
#include <queue>
#include <map>

// 测试任务
struct Job
{
	std::string name; // 测试任务名
	int priority; // 优先级
	std::map<std::string, std::string> cfgs; // 配置项

	bool operator < (const Job & j) const {
		// 由大到小排列
		return j.priority > priority;
	}
};

class CfgParse
{
private:
	std::string cfg_path_; // 配置文件路径

public:
	CfgParse(std::string cfg_path = testRes_path + "TestISS-AIUI.cfg");
	std::queue<Job> getTempJobs(); // 获取临时测试任务，用一个队列来保存
	std::priority_queue<Job> getJobs(); // 用一个优先级队列存放测试任务，按任务优先级由大到小排序
	std::vector<Job> getAllJobs(); // 获取全部测试任务，不论是否设置为执行。供按命令行参数执行测试使用
	std::map<std::string, std::string> getGlobalCfgs(); // 全局配置
};

#endif