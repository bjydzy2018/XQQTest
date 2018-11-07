#ifndef CFGPARSE_H_
#define CFGPARSE_H_

#include "Def.h"

#include <string>
#include <queue>
#include <map>

// ��������
struct Job
{
	std::string name; // ����������
	int priority; // ���ȼ�
	std::map<std::string, std::string> cfgs; // ������

	bool operator < (const Job & j) const {
		// �ɴ�С����
		return j.priority > priority;
	}
};

class CfgParse
{
private:
	std::string cfg_path_; // �����ļ�·��

public:
	CfgParse(std::string cfg_path = testRes_path + "TestISS-AIUI.cfg");
	std::queue<Job> getTempJobs(); // ��ȡ��ʱ����������һ������������
	std::priority_queue<Job> getJobs(); // ��һ�����ȼ����д�Ų������񣬰��������ȼ��ɴ�С����
	std::vector<Job> getAllJobs(); // ��ȡȫ���������񣬲����Ƿ�����Ϊִ�С����������в���ִ�в���ʹ��
	std::map<std::string, std::string> getGlobalCfgs(); // ȫ������
};

#endif