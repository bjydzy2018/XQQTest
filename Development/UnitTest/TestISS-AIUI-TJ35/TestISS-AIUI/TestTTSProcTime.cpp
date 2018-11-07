#include "TestTTSProcTime.h"
#include "CommonFun.h"
#include "VAInc.h"
#include "test_log.h"
#include "TimerForTest.h"

#include <iostream>
#include <fstream>

using namespace VA;
using std::endl;
using std::string;

TestTTSProcTime::TestTTSProcTime(string proc_time_file_handle_ret_path,
								 string proc_time_mem_ret_path) : proc_time_file_handle_ret_path_(proc_time_file_handle_ret_path),
																  proc_time_mem_ret_path_(proc_time_mem_ret_path){
	init_time_ = 0;
	create_time_ = 0;

	err	= NULL;
	m_phISSTTSRES = NULL;
	m_phISSTTS = NULL;
}

//// 分别使用两种资源加载方式对一个文本列表进行合成
//void TestTTSProcTime::test() {
//	DEBUG_TRACE_INFOR("init tts res mode: 0");
//
//	
//	TimerForTest t;
//	err = ISSTTSInitRes(&m_phISSTTSRES, def.resDir, 0);
//	
//	init_time_ = t.elapsed();
//	DEBUG_TRACE("ISSTTSInitRes return %d" ,err);
//	
//	t.restart();
//	err = ISSTTSCreate(&m_phISSTTS, m_phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL); 
//	
//	create_time_ = t.elapsed();
//	DEBUG_TRACE("ISSTTSCreate return %d" ,err);
//
//	testTTSBatch(proc_time_file_handle_ret_path_);
//
//	err = ISSTTSDestroy(m_phISSTTS);
//	DEBUG_TRACE("ISSTTSDestroy return %d" ,err);
//
//	ISSTTSUnInitRes(m_phISSTTSRES);
//	DEBUG_TRACE("ISSTTSUnInitRes return %d" ,err);
//
//	m_phISSTTSRES = NULL;
//	m_phISSTTS = NULL;
//
//	DEBUG_TRACE_INFOR("sleep 10s");
//	sleep(10000);
//
//	DEBUG_TRACE_INFOR("init tts res mode: 1");
//	t.restart();
//	err = ISSTTSInitRes(&m_phISSTTSRES, def.resDir, 1);
//	
//	init_time_ = t.elapsed();
//	DEBUG_TRACE("ISSTTSInitRes return %d" ,err);
//
//	t.restart();
//	err = ISSTTSCreate(&m_phISSTTS, m_phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL);
//	
//	create_time_ = t.elapsed();
//	DEBUG_TRACE("ISSTTSCreate return %d" ,err);
//
//	testTTSBatch(proc_time_mem_ret_path_);
//
//	err = ISSTTSDestroy(m_phISSTTS);
//	DEBUG_TRACE("ISSTTSDestroy return %d" ,err);
//
//	err = ISSTTSUnInitRes(m_phISSTTSRES);
//	DEBUG_TRACE("ISSTTSUnInitRes return %d" ,err);
//
//	m_phISSTTSRES = NULL;
//	m_phISSTTS = NULL;
//}


void TestTTSProcTime::test(int mode) {
	DEBUG_TRACE_INFOR("init tts res mode:%d",mode);

	TimerForTest t;
	err = ISSTTSInitRes(&m_phISSTTSRES, def.resDir, mode);

	init_time_ = t.elapsed();
	DEBUG_TRACE("ISSTTSInitRes return %d" ,err);

	t.restart();
	err = ISSTTSCreate(&m_phISSTTS, m_phISSTTSRES, DefTTS::Proc_TTS_Ready, NULL, NULL); 

	create_time_ = t.elapsed();
	DEBUG_TRACE("ISSTTSCreate return %d" ,err);

	if(mode == 0){
		testTTSBatch(proc_time_file_handle_ret_path_);
	}else if(mode == 1){
		testTTSBatch(proc_time_mem_ret_path_);
	}else{
		DEBUG_TRACE("mode is wrong!");
		return ;
	}

	err = ISSTTSDestroy(m_phISSTTS);
	DEBUG_TRACE("ISSTTSDestroy return %d" ,err);

	ISSTTSUnInitRes(m_phISSTTSRES);
	DEBUG_TRACE("ISSTTSUnInitRes return %d" ,err);

	m_phISSTTSRES = NULL;
	m_phISSTTS = NULL;

}


void TestTTSProcTime::testTTSBatch(string ret_path) {
	vector<string> tts_texts = readFileByLine(def.batch_list_path, true);
	ofstream fout(ret_path.c_str(), ios::out);
	double sum_proc_time = 0;

	double sum_get_audio_time = 0;
	double sum_get_audio_speed = 0;
	TimerForTest t;
	TimerForTest t_GetAudio;
	unsigned int m_iBufferFrames = 800;
	unsigned int m_piBufferFramesGot = 0;
	short *pRes;
	pRes = (short *)malloc(1600);

	text_num_ = tts_texts.size();
	DEBUG_TRACE_INFOR(("tts list num: " + LongToString(text_num_)).c_str());

	fout << "Init resume " << init_time_ << " ms\n";
	DEBUG_TRACE_INFOR(("init res time: " + DoubleToString(init_time_) + "ms").c_str());
	fout << "Create resume " << create_time_ << " ms\n\n";
	DEBUG_TRACE_INFOR(("create time: " + DoubleToString(create_time_) + "ms").c_str());

	for(size_t i = 0; i < tts_texts.size(); ++i){
		string line = tts_texts[i];
		fout << line << endl;
#if VA_VC
		err = ISSTTSStart(m_phISSTTS, line.c_str(), strlen(line.c_str()), ISS_TTS_CODEPAGE_GBK);
#elif VA_GNUC
		err = ISSTTSStart(m_phISSTTS, line.c_str(), strlen(line.c_str()), ISS_TTS_CODEPAGE_UTF8);
#endif
		DEBUG_TRACE("ISSTTSStart return %d" ,err);

		t.restart();
		while (0 == DefTTS::msgOnDataReady) {
			sleep(10);
		}
		
		double proc_time = t.elapsed();
		DEBUG_TRACE_INFOR(("tts proc time: " + DoubleToString(proc_time) + "ms").c_str());

		DefTTS::initMsg();
		fout << "tts proc result resume " << proc_time << " ms\n\n";

		sum_proc_time += proc_time;

		unsigned int n1, n2;
		unsigned int sumAudioSize = 0;
		t.restart();
		while (err != ISS_ERROR_TTS_COMPLETED) {						//最后一次为COMPLETED
			m_piBufferFramesGot = 0;
			err = ISSTTSGetAudioData(m_phISSTTS, pRes, m_iBufferFrames, &m_piBufferFramesGot, &n1, &n2);
			sumAudioSize += m_piBufferFramesGot;
			long t_sleep = sumAudioSize * 2 / 32;
			long t_pass = t.elapsed();
			//printf("sleeptime: %d (%d - %d)\n", int(t_sleep - t_pass), t_sleep, t_pass);
			if ((int(t_sleep - t_pass)) > 0) {
				sleep(int(t_sleep - t_pass));
			}
		}
		double get_audio_time = t.elapsed();
		double get_audio_speed = (sumAudioSize*2.0 / 1000) / (get_audio_time / 1000);
		DEBUG_TRACE_INFOR("tts get audio resume: %f ms",get_audio_time);
		DEBUG_TRACE_INFOR("tts get audio speed: %f",get_audio_speed);
		fout << "tts get audio resume " << get_audio_time << " ms\n\n";
		fout << "tts get audio speed " << get_audio_speed << " \n";
		sum_get_audio_time += get_audio_time;
		sum_get_audio_speed += get_audio_speed;
	}

	double avg_proc_time = sum_proc_time / text_num_;
	double avg_get_audio_time = sum_get_audio_time / text_num_;
	double avg_get_audio_speed = sum_get_audio_speed / text_num_;
	fout << "the avg tts proc result resume " << avg_proc_time << endl;
	fout << "the avg tts get audio resume " << avg_get_audio_time << endl;
	fout << "the avg tts get audio speed " << avg_get_audio_speed << endl;
	DEBUG_TRACE_INFOR(("tts proc avg time: " + DoubleToString(avg_proc_time) + "ms").c_str());
	DEBUG_TRACE_INFOR(("tts get audio avg time: " + DoubleToString(avg_get_audio_time) + "ms").c_str());
	DEBUG_TRACE_INFOR(("tts get audio avg speed: " + DoubleToString(avg_get_audio_speed)).c_str());
	free(pRes);
	fout.close();
}