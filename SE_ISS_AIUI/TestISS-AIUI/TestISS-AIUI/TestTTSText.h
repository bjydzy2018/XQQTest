#ifdef __ANDROID__

#ifndef TEST_TTS_TEST_H_
#define TEST_TTS_TEST_H_

#include "DefTTS.h"

#include <string>

class TestTTSText {
private:
	ISSErrID err;
	HISSTTSRES phISSTTSRES;
	HISSTTS	 phISSTTS;

	DefTTS def;

	std::string text_; // �ϳ��ı�������ı���&�ָ�
	std::string tts_audio_path_; // �ϳɳ�����Ƶ���ɵ�·��

	void tts(std::string &); // �ϳ�һ���ı�

public:
	TestTTSText();
	~TestTTSText();
	void test(); // ������ڣ��ϳ������ļ���ָ�����ı�
	void test(std::string &); // ������ڣ��ϳ�һ���б��ļ��е��ı�
	void setText(std::string & text) { text_ = text; };
	void setSpeaker(int);
	void setTTSAudioPath(std::string & tts_audio_path) { tts_audio_path_ = string(PrtU8(tts_audio_path.c_str())); };
};

#endif
#endif
