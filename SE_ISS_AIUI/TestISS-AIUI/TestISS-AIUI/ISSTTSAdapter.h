#pragma once
#include "VAInc.h"
#include "iss_tts.h"

#include <string>
class ISSTTSAdapter {
public:
	static ISSErrID  ISSTTSInitRes(HISSTTSRES* phISSTTSRES, const char* szResource,int iResMode );
	static ISSErrID  ISSTTSInitResW(HISSTTSRES* phISSTTSRES,const wchar_t* szResource,int iResMode );
	static ISSErrID  ISSTTSUnInitRes(HISSTTSRES hISSTTSRES);
	static ISSErrID  ISSTTSCreate(HISSTTS* phISSTTS,HISSTTSRES hISSTTSRES, Proc_OnTTSDataReady pcbOnTTSDataReady,
								  Proc_OnISSChipCryptError pcbOnISSChipCryptError, void* pUsrArg);
	static ISSErrID  ISSTTSDestroy(HISSTTS hISSTTS);
	static ISSErrID  ISSTTSSetParam(HISSTTS hISSTTS,int iParamID,int iParamValue);
	static ISSErrID  ISSTTSSetParamEx(HISSTTS hISSTTS,int iParamID,const char* strParam);
	static ISSErrID  ISSTTSStart(HISSTTS hISSTTS,const void* pText, 
			                     int iSize,ISSTTSCodePage iTTSCodePage);
	static ISSErrID  ISSTTSGetAudioData(HISSTTS hISSTTS,void* pOutBuffer,
		                                unsigned int iBufferFrames, unsigned int* piBufferFramesGot, 
		                                unsigned int* piProgBegin,unsigned int* piProgLen);
	static ISSErrID  ISSTTSStop(HISSTTS hISSTTS);
	static ISSErrID  ISSTTSGetUsrArg(HISSTTS hISSTTS, void** ppUsrArg);
	static ISSErrID  ISSTTSSetLogCfgParam(const int iParamID,const char* szParamValue);

};