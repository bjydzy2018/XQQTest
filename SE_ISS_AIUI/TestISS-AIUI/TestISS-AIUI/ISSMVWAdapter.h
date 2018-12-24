#pragma once

#include "VAInc.h"
#include "iss_mvw.h"

#include <string>

class ISSMVWAdapter {
public:
	static ISSErrID  ISSMVWCreate(HISSMVW* phISSMVW,const char* szResourceDir,Proc_ISSMVWOnWakeup  pfnOnWakeup,
		                                Proc_OnISSChipCryptError pcbOnISSChipCryptError, void* pUsrArg);
	static ISSErrID  ISSMVWCreateW(HISSMVW* phISSMVW,const wchar_t* szResourceDir,Proc_ISSMVWOnWakeup  pfnOnWakeup,
		                                 Proc_OnISSChipCryptError pcbOnISSChipCryptError, void* pUsrArg);
	static ISSErrID  ISSMVWCreateEx(HISSMVW* phISSMVW,const char* szResourceDir,Proc_MVWOnMsgProc pfnMVWOnMsgProc,
			                              void* pUsrArg);
	static ISSErrID  ISSMVWCreateWEx(HISSMVW* phISSMVW,const wchar_t* szResourceDir,Proc_MVWOnMsgProc pfnMVWOnMsgProc,
		                                   void* pUsrArg);
	static ISSErrID  ISSMVWDestroy(HISSMVW  hISSMVW);
	static ISSErrID  ISSMVWSetParam(HISSMVW hISSMVW,const char* szParam,const char* szParamValue);
	static ISSErrID  ISSMVWSetThreshold(HISSMVW  hISSMVW,unsigned int  nMvwScene,unsigned int  nMvwId,
		                                      int nThreshold);
	static ISSErrID  ISSMVWStart(HISSMVW  hISSMVW,unsigned int  nMvwScene );
	static ISSErrID  ISSMVWAddStartScene(HISSMVW  hISSMVW,unsigned int  nMvwScene );
	static ISSErrID  ISSMVWAppendAudioData(HISSMVW  hISSMVW,short* pSamples,unsigned int nNumberOfToWrite,
		                                         unsigned int* pNumberOfWritten);
	static ISSErrID  ISSMVWStop(HISSMVW  hISSMVW);
	static ISSErrID  ISSMVWStopScene(HISSMVW  hISSMVW,unsigned int  nMvwScene);
	static ISSErrID  ISSMVWSetKeyWords(HISSMVW  hISSMVW,unsigned int nMvwScene,const char* szWords);
	static ISSErrID  ISSMVWSetDefaultKeyWords(HISSMVW  hISSMVW,unsigned int nMvwScene);
	static ISSErrID  ISSMVWSetMvwLanguage(unsigned int nLangType);
	static bool  ISSMVWIsCouldAppendAudioData();
	static ISSErrID  ISSMVWSetLogCfgParam(const int iParamID,const char* szParamValue);
};