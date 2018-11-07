#ifndef TEST_SPWORD_CONSISTENT_H_
#define TEST_SPWORD_CONSISTENT_H_

#include "DefSR.h"

#include <string>

#ifdef ISS_SR_3

class TestSPWordConsistent {
private:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;

	std::string test_text_list_path_; // 文本列表路径
	int lang_; // 语种

public:
	TestSPWordConsistent();
	void setTestTextListPath(std::string test_text_list_path) { test_text_list_path_ = test_text_list_path; };
	void test();
};

#endif // ISS_SR_3

#endif // !TEST_SR_NAMEXP_CONSISTENT_H_

