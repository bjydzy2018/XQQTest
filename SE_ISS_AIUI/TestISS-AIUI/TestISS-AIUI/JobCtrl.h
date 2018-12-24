#ifndef JOBCTRL_H_
#define JOBCTRL_H_

#include "CfgParse.h"

class JobCtrl {
private:
	int argc_; // ���������������ڴ��ݸ�gtest���
	char ** argv_; // ���������������ڴ��ݸ�gtest���
	std::priority_queue<Job> jobs_;
	std::queue<Job> temp_jobs_;
	std::vector<Job> all_jobs_;
	std::map<std::string, std::string> global_cfgs_;
	bool run_in_subprocess_;

	void test(Job &); // ִ�в�������
	int	 testApi(Job &, std::string);
	void testTTSProcTime(Job &);
	void testTTSApiRandom(Job &);
	void testMVWBatchByScene(Job &);
	void testMVWBatchByWord(Job &);
	void testMVWConsistent(Job &);
	void testMVWApiRandom(Job &);
	void testCATATime(Job &);
	void testCATAConsistent(Job &);
	void testCATAApiRandom(Job &);
	void testSRApiRandom(Job &);
	void testSRBatch(Job &);
	void testSPWordConsistent(Job &);
	void testSRConsistent(Job &);
	void testUpLoadDictBatch(Job &);
	void testSeoptBatch(Job &);

	void testTTSText(Job &);
	void testTTSTextByList(Job &);
	void testCATAIndexSearch(Job &);
	void testCATASearch(Job &);
	void testSR(Job &);

public:
	JobCtrl(int, char **);
	void work();
};

#endif