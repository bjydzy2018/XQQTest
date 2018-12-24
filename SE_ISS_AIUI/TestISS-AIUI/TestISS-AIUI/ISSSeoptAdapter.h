#pragma once
#include "VAInc.h"
#include "iss_seopt.h"

#include <string>
class ISSSeoptAdapter {
public:
	static ISSErrID  ISSSeoptCreate(ISSSeoptHandle * pHandle, const char* szResPath);
	static ISSErrID  ISSSeoptDestroy(ISSSeoptHandle handle);
	static ISSErrID  ISSSeoptSetParam(ISSSeoptHandle handle, const char* szParam, const char* szParamValue);
	static ISSErrID  ISSSeoptProcess(ISSSeoptHandle handle, const void* pBufIn, int nSamplesIn, 
		const void** ppBufOut, int* pnSamplesOut, int* pnChannelsOut);
	static ISSErrID  ISSSeoptGetGSCPower(ISSSeoptHandle handle, int nIndex, float* pfPower, float* pfAngle);
};