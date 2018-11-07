#ifndef _TTS_INIT_TIME_H_
#define _TTS_INIT_TIME_H_

#include "DefTTS.h"

#include <string>

class TestTTSProcTime
{
private:
	ISSErrID err;
	HISSTTSRES m_phISSTTSRES;
	HISSTTS	 m_phISSTTS;

	int text_num_; // 合成文本的条目数
	double init_time_; // 加载资源时间
	double create_time_; // 初始化时间
	std::string proc_time_file_handle_ret_path_; // 文件句柄加载资源方式初次回调响应时间结果文件路径
	std::string proc_time_mem_ret_path_; // 内存加载资源方式初次回调响应时间结果文件路径

	DefTTS def;

	void testTTSBatch(std::string ret_path);

public:
	TestTTSProcTime(std::string proc_time_file_handle_ret_path = "TestRes/tts/proc_time_ret/file_handle/tts_proc_time_file_handle_ret.txt",
		std::string proc_time_mem_ret_path = "TestRes/tts/proc_time_ret/mem/tts_proc_time_mem_ret.txt");

	void test(int mode); // 测试入口
};

#endif