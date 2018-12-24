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
	const char * resDir; // ��Դ·��
#if VA_VC
	const wchar_t* resDirW;
#endif
	std::string batch_list_path; // �����ϳ��б�

	// �ص���Ϣ
	static int msgOnDataReady;

	DefTTS();
	static void initMsg();
	static void Proc_TTS_Ready(void * pUsrArg);

	void setResLoadType(std::string);
};
#endif
#endif