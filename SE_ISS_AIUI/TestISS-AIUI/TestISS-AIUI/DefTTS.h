#ifdef __ANDROID__

#ifndef _DEFTTS_H_
#define _DEFTTS_H_

#include "iss_tts.h"
#include "VAInc.h"
#include "Def.h"
#include "ISSTTSAdapter.h"
#include <string>

class DefTTS: public Def {
public :
	const char * resDir; // 资源路径
#if VA_VC
	const wchar_t* resDirW;
#endif
	std::string batch_list_path; // 批量合成列表

	// 回调消息
	static int msgOnDataReady;

	DefTTS();
	static void initMsg();
	static void Proc_TTS_Ready(void * pUsrArg);

	void setResLoadType(std::string);
};
#endif
#endif