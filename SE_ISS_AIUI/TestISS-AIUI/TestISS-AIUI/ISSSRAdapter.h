#pragma once

#include "iss_auth.h"
#include "VAInc.h"
#include "iss_sr.h"

#include <string>
class ISSSRAdapter {
public:
	static ISSErrID  ISSSetMachineCode(const char* szMachineCode );
	static ISSErrID  ISSSRCreate(HISSSR* phISSSRObj,
						  ISS_SR_ACOUS_LANG eAcousLang,const char* szResourceDir,
		                  Proc_OnMsgProc  pfnOnMsgProc,void*  pUsrArg);
	static ISSErrID  ISSSRCreateEx(HISSSR*         phISSSRObj,
		                  ISS_SR_ACOUS_LANG eAcousLang,const char*     szResourceDir,
		                  unsigned int	nWorkMode,Proc_OnMsgProc  pfnOnMsgProc,
		                  void* pUsrArg);
	static ISSErrID  ISSSRDestroy(HISSSR hISSSRObj );
	static ISSErrID  ISSSRStart(HISSSR hISSSRObj,const char* szScene,
		                 int iMode,       const char* szCmd );
	static ISSErrID  ISSSRStop(HISSSR hISSSRObj );
	static ISSErrID  ISSSRSetParam(HISSSR hISSSRObj,const char* szParam,
			                const char* szParamValue);
	static ISSErrID  ISSSRAppendAudioData(HISSSR hISSSRObj, short* pSamples,
                                   unsigned int nNumberOfToWrite,
								   unsigned int*   pNumberOfWritten );
	static ISSErrID  ISSSRUpLoadDict(HISSSR hISSSRObj, const char *szList, int iUpLoadMode);
	static ISSErrID  ISSSRUpLoadData(HISSSR hISSSRObj, const char *szData, int iUpLoadMode);
	static ISSErrID  ISSSRQuerySyncStatus(HISSSR hISSSRObj, const char *szSid);
	static const char* ISSGetRevisionNum(void);
	static ISSErrID  ISSSRSetLogCfgParam(const int iParamID, const char* szParamValue);
	static ISSErrID  ISSSRClearResult(HISSSR hISSSRObj);
};