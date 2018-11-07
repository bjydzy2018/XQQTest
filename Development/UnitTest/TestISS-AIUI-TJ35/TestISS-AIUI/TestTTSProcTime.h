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

	int text_num_; // �ϳ��ı�����Ŀ��
	double init_time_; // ������Դʱ��
	double create_time_; // ��ʼ��ʱ��
	std::string proc_time_file_handle_ret_path_; // �ļ����������Դ��ʽ���λص���Ӧʱ�����ļ�·��
	std::string proc_time_mem_ret_path_; // �ڴ������Դ��ʽ���λص���Ӧʱ�����ļ�·��

	DefTTS def;

	void testTTSBatch(std::string ret_path);

public:
	TestTTSProcTime(std::string proc_time_file_handle_ret_path = "TestRes/tts/proc_time_ret/file_handle/tts_proc_time_file_handle_ret.txt",
		std::string proc_time_mem_ret_path = "TestRes/tts/proc_time_ret/mem/tts_proc_time_mem_ret.txt");

	void test(int mode); // �������
};

#endif