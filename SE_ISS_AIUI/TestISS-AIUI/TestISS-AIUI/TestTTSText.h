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

	std::string text_; // 合成文本，多个文本以&分隔
	std::string tts_audio_path_; // 合成出的音频生成的路径

	void tts(std::string &); // 合成一个文本

public:
	TestTTSText();
	~TestTTSText();
	void test(); // 测试入口，合成配置文件中指定的文本
	void test(std::string &); // 测试入口，合成一个列表文件中的文本
	void setText(std::string & text) { text_ = text; };
	void setSpeaker(int);
	void setTTSAudioPath(std::string & tts_audio_path) { tts_audio_path_ = string(PrtU8(tts_audio_path.c_str())); };
};

#endif
#endif
