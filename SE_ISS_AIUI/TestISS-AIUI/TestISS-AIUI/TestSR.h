#ifndef TEST_SR_H_
#define TEST_SR_H_

#include "DefSR.h"

#include <string>

class TestSR {
private:
	ISSErrID err;
	HISSSR	 phISSSR;

	DefSR def;

public:
	TestSR();
	void test_sr(std::string, int, std::string, int, std::string);
	void test_sr(std::string, int, std::string, int, std::string, std::string, int);
#if VA_VC
	void test_sr_online(int, std::string, int, std::string);
	void test_sr_online(int, std::string, int, std::string, std::string, int);
#endif
};

#endif
