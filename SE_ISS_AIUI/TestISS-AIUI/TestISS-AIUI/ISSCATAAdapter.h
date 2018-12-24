#pragma once
#include "VAInc.h"
#include "iss_cata.h"

#include <string>
class ISSCATAAdapter {
public:
	static ISSErrID  ISSCataIndexCreate(HISSCATAINDEX*  phISSCATAINDEXObj, const char* szResourceDir,
		                                const char*  szIndexResName, int  bDropOldResFlag,    
		                                Proc_OnCataMsgProc  pfnOnCataMsgProc,  void* pUsrArg);
	static ISSErrID  ISSCataIndexCreateEx(HISSCATAINDEX* phISSCATAINDEXObj,const char* szResourceDir,
		                                  const char* szIndexResName,int  langType,
		                                  int bDropOldResFlag,Proc_OnCataMsgProc  pfnOnCataMsgProc,
	                                      void*  pUsrArg );
	static ISSErrID  ISSCataIndexDestroy(HISSCATAINDEX    hISSCATAINDEXObj);
	static ISSErrID  ISSCataIndexDropRes(HISSCATAINDEX    hISSCATAINDEXObj);
	static ISSErrID  ISSCataIndexAddIdxEntity(HISSCATAINDEX  hISSCATAINDEXObj,const char* pData);
	static ISSErrID  ISSCataIndexDelIdxEntity(HISSCATAINDEX  hISSCATAINDEXObj,const char* pData);
	static ISSErrID  ISSCataIndexEndIdxEntity(HISSCATAINDEX  hISSCATAINDEXObj);
	static ISSErrID  ISSCataSearchCreate(HISSCATASEARCH* phISSCATASEARCHObj,const char* szResourceDir,
		                                 const char* szIndexResNames,Proc_OnCataMsgProc  pfnOnCataMsgProc,
		                                 void* pUsrArg );
	static ISSErrID  ISSCataSearchCreateEx(HISSCATASEARCH* phISSCATASEARCHObj,const char*  szResourceDir,											
		                                   const char* szIndexResNames, Proc_OnCataMsgProc  pfnOnCataMsgProc,       
		                                   int nLangType, void* pUsrArg );
	static ISSErrID  ISSCataSearchDestroy(HISSCATASEARCH    hISSCATASEARCHObj);
	static ISSErrID  ISSCataSearchAsync(HISSCATASEARCH  hISSCATASEARCHObj,const char* szQuery);
	static const char* ISSCataSearchSync(HISSCATASEARCH hISSCATASEARCHObj, const char* szQuery, ISSErrID* pErr);

	static ISSErrID  ISSCataSetParam(HISSCATASEARCH hISSCATASEARCHObj,int pmType,int val);
	static ISSErrID  ISSCataSetLogCfgParam(const int iParamID,const char* szParamValue);

};