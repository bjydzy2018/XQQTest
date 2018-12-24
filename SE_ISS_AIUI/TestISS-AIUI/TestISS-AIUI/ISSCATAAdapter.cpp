#include"ISSCATAAdapter.h"

#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
#include <jni.h>
#include <android/log.h>
#include <unistd.h>

#define LOG_TAG "ISSCATAAdapter"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern JavaVM* gs_jvm;

extern jclass g_libisscata_class;
extern jmethodID g_method_libisscata_IndexCreate;
extern jmethodID g_method_libisscata_IndexCreateEx;
extern jmethodID g_method_libisscata_IndexDestroy;
extern jmethodID g_method_libisscata_IndexDropRes;
extern jmethodID g_method_libisscata_IndexAddIdxEntity;
extern jmethodID g_method_libisscata_IndexDelIdxEntity;
extern jmethodID g_method_libisscata_IndexEndIdxEntity;
extern jmethodID g_method_libisscata_SearchCreate;
extern jmethodID g_method_libisscata_SearchCreateEx;
extern jmethodID g_method_libisscata_SearchDestroy;
extern jmethodID g_method_libisscata_SearchSync;
extern jmethodID g_method_libisscata_SearchAsync;
extern jmethodID g_method_libisscata_SetParam;

extern jclass g_CataNativeHandle_class;
extern jmethodID g_method_CataNativeHandle_Constructor_init;
extern jmethodID g_method_CataNativeHandle_getErr_ret;

extern jclass g_AiuiToolMainEntry_onICataListener_class;
extern jmethodID g_method_AiuiToolMainEntry_onICataListener_Constructor_init;
extern jmethodID g_method_AiuiToolMainEntry_onICataListener_setNative_point;

class ClibisscataObj
{
public:
	jobject jobjCataNativeHandle;

	Proc_OnCataMsgProc pfnOnCataMsgProc;
	void* pUsrArg;

	std::string searchresult;
};

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL Java_com_iflytek_nativetool_AiuiToolMainEntry_onICataNativeListener
(JNIEnv *env, jclass jc, jlong jNative_point, jint uMsg, jint wParam, jstring jlParam) {
	ClibisscataObj* pClibisscataObj = (ClibisscataObj*)jNative_point;
	const char* lParam = NULL;
	if (jlParam) {
		lParam = (const char*)env->GetStringUTFChars(jlParam, NULL);
	}
	if (pClibisscataObj && pClibisscataObj->pfnOnCataMsgProc) {
		pClibisscataObj->pfnOnCataMsgProc(pClibisscataObj->pUsrArg, uMsg, wParam, lParam);
	}
	if (jlParam) {
		env->ReleaseStringUTFChars(jlParam, lParam);
	}
}
#ifdef __cplusplus
}
#endif

#endif

ISSErrID  ISSCATAAdapter::ISSCataIndexCreate(HISSCATAINDEX*  phISSCATAINDEXObj, const char* szResourceDir,
	const char*  szIndexResName, int  bDropOldResFlag, Proc_OnCataMsgProc  pfnOnCataMsgProc, void* pUsrArg) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (phISSCATAINDEXObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	jstring jstrResourceDir;
	if (szResourceDir) {
		jstrResourceDir = env->NewStringUTF(szResourceDir);
	} else {
		jstrResourceDir = env->NewStringUTF("");
	}
	jstring jstrIndexResName;
	if (szIndexResName) {
		jstrIndexResName = env->NewStringUTF(szIndexResName);
	} else {
		jstrIndexResName = env->NewStringUTF("");
	}
	jobject jobjCataNativeHandle = env->NewObject(g_CataNativeHandle_class, g_method_CataNativeHandle_Constructor_init);
	jobject jobjonICataListener = env->NewObject(g_AiuiToolMainEntry_onICataListener_class, g_method_AiuiToolMainEntry_onICataListener_Constructor_init);
	env->CallStaticVoidMethod(g_libisscata_class, g_method_libisscata_IndexCreate, jobjCataNativeHandle, jstrResourceDir, jstrIndexResName, (jint)bDropOldResFlag, jobjonICataListener);
	int java_ret = env->CallIntMethod(jobjCataNativeHandle, g_method_CataNativeHandle_getErr_ret);
	if (0 == java_ret) {
		ClibisscataObj* pClibisscataObj = new ClibisscataObj();
		pClibisscataObj->jobjCataNativeHandle = (jobject)env->NewGlobalRef(jobjCataNativeHandle);
		pClibisscataObj->pfnOnCataMsgProc = pfnOnCataMsgProc;
		pClibisscataObj->pUsrArg = pUsrArg;

		env->CallVoidMethod(jobjonICataListener, g_method_AiuiToolMainEntry_onICataListener_setNative_point, (jlong)pClibisscataObj);

		*phISSCATAINDEXObj = (HISSCATAINDEX)pClibisscataObj;
	}

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSCataIndexCreate(phISSCATAINDEXObj, szResourceDir, szIndexResName, bDropOldResFlag, pfnOnCataMsgProc, pUsrArg);
#endif
}
ISSErrID  ISSCATAAdapter::ISSCataIndexCreateEx(HISSCATAINDEX* phISSCATAINDEXObj, const char* szResourceDir,
	const char* szIndexResName, int  langType, int bDropOldResFlag, Proc_OnCataMsgProc  pfnOnCataMsgProc,void*  pUsrArg) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (phISSCATAINDEXObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	jstring jstrResourceDir;
	if (szResourceDir) {
		jstrResourceDir = env->NewStringUTF(szResourceDir);
	} else {
		jstrResourceDir = env->NewStringUTF("");
	}
	jstring jstrIndexResName;
	if (szIndexResName) {
		jstrIndexResName = env->NewStringUTF(szIndexResName);
	} else {
		jstrIndexResName = env->NewStringUTF("");
	}
	jobject jobjCataNativeHandle = env->NewObject(g_CataNativeHandle_class, g_method_CataNativeHandle_Constructor_init);
	jobject jobjonICataListener = env->NewObject(g_AiuiToolMainEntry_onICataListener_class, g_method_AiuiToolMainEntry_onICataListener_Constructor_init);
	env->CallStaticVoidMethod(g_libisscata_class, g_method_libisscata_IndexCreateEx, jobjCataNativeHandle, jstrResourceDir, jstrIndexResName, (jint)langType, (jint)bDropOldResFlag, jobjonICataListener);
	int java_ret = env->CallIntMethod(jobjCataNativeHandle, g_method_CataNativeHandle_getErr_ret);
	if (0 == java_ret) {
		ClibisscataObj* pClibisscataObj = new ClibisscataObj();
		pClibisscataObj->jobjCataNativeHandle = (jobject)env->NewGlobalRef(jobjCataNativeHandle);
		pClibisscataObj->pfnOnCataMsgProc = pfnOnCataMsgProc;
		pClibisscataObj->pUsrArg = pUsrArg;

		env->CallVoidMethod(jobjonICataListener, g_method_AiuiToolMainEntry_onICataListener_setNative_point, (jlong)pClibisscataObj);

		*phISSCATAINDEXObj = (HISSCATAINDEX)pClibisscataObj;
	}

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSCataIndexCreateEx(phISSCATAINDEXObj, szResourceDir, szIndexResName, langType, bDropOldResFlag, pfnOnCataMsgProc, pUsrArg);
#endif
}
ISSErrID  ISSCATAAdapter::ISSCataIndexDestroy(HISSCATAINDEX    hISSCATAINDEXObj) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSCATAINDEXObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibisscataObj* pClibisscataObj = (ClibisscataObj*)hISSCATAINDEXObj;
	env->CallStaticVoidMethod(g_libisscata_class, g_method_libisscata_IndexDestroy, pClibisscataObj->jobjCataNativeHandle);
	int java_ret = env->CallIntMethod(pClibisscataObj->jobjCataNativeHandle, g_method_CataNativeHandle_getErr_ret);
	if (0 == java_ret) {
		env->DeleteGlobalRef(pClibisscataObj->jobjCataNativeHandle);
		delete pClibisscataObj;
	}

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSCataIndexDestroy(hISSCATAINDEXObj);
#endif
}
ISSErrID  ISSCATAAdapter::ISSCataIndexDropRes(HISSCATAINDEX    hISSCATAINDEXObj) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSCATAINDEXObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibisscataObj* pClibisscataObj = (ClibisscataObj*)hISSCATAINDEXObj;
	env->CallStaticVoidMethod(g_libisscata_class, g_method_libisscata_IndexDropRes, pClibisscataObj->jobjCataNativeHandle);
	int java_ret = env->CallIntMethod(pClibisscataObj->jobjCataNativeHandle, g_method_CataNativeHandle_getErr_ret);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSCataIndexDropRes(hISSCATAINDEXObj);
#endif
}
ISSErrID  ISSCATAAdapter::ISSCataIndexAddIdxEntity(HISSCATAINDEX  hISSCATAINDEXObj, const char* pData) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSCATAINDEXObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibisscataObj* pClibisscataObj = (ClibisscataObj*)hISSCATAINDEXObj;
	jstring jstrData;
	if (pData) {
		jstrData = env->NewStringUTF((const char*)pData);
	} else {
		jstrData = env->NewStringUTF("");
	}
	env->CallStaticVoidMethod(g_libisscata_class, g_method_libisscata_IndexAddIdxEntity, pClibisscataObj->jobjCataNativeHandle, jstrData);
	int java_ret = env->CallIntMethod(pClibisscataObj->jobjCataNativeHandle, g_method_CataNativeHandle_getErr_ret);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSCataIndexAddIdxEntity(hISSCATAINDEXObj, pData);
#endif
}
ISSErrID  ISSCATAAdapter::ISSCataIndexDelIdxEntity(HISSCATAINDEX  hISSCATAINDEXObj, const char* pData) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSCATAINDEXObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibisscataObj* pClibisscataObj = (ClibisscataObj*)hISSCATAINDEXObj;
	jstring jstrData;
	if (pData) {
		jstrData = env->NewStringUTF((const char*)pData);
	} else {
		jstrData = env->NewStringUTF("");
	}
	env->CallStaticVoidMethod(g_libisscata_class, g_method_libisscata_IndexDelIdxEntity, pClibisscataObj->jobjCataNativeHandle, jstrData);
	int java_ret = env->CallIntMethod(pClibisscataObj->jobjCataNativeHandle, g_method_CataNativeHandle_getErr_ret);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSCataIndexDelIdxEntity(hISSCATAINDEXObj, pData);
#endif
}
ISSErrID  ISSCATAAdapter::ISSCataIndexEndIdxEntity(HISSCATAINDEX  hISSCATAINDEXObj) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSCATAINDEXObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibisscataObj* pClibisscataObj = (ClibisscataObj*)hISSCATAINDEXObj;
	env->CallStaticVoidMethod(g_libisscata_class, g_method_libisscata_IndexEndIdxEntity, pClibisscataObj->jobjCataNativeHandle);
	int java_ret = env->CallIntMethod(pClibisscataObj->jobjCataNativeHandle, g_method_CataNativeHandle_getErr_ret);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSCataIndexEndIdxEntity(hISSCATAINDEXObj);
#endif
}
ISSErrID  ISSCATAAdapter::ISSCataSearchCreate(HISSCATASEARCH* phISSCATASEARCHObj, const char* szResourceDir,
	const char* szIndexResNames, Proc_OnCataMsgProc  pfnOnCataMsgProc, void* pUsrArg) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (phISSCATASEARCHObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	jstring jstrResourceDir;
	if (szResourceDir) {
		jstrResourceDir = env->NewStringUTF(szResourceDir);
	}
	else {
		jstrResourceDir = env->NewStringUTF("");
	}
	jstring jstrIndexResNames;
	if (szIndexResNames) {
		jstrIndexResNames = env->NewStringUTF(szIndexResNames);
	}
	else {
		jstrIndexResNames = env->NewStringUTF("");
	}
	jobject jobjCataNativeHandle = env->NewObject(g_CataNativeHandle_class, g_method_CataNativeHandle_Constructor_init);
	jobject jobjonICataListener = env->NewObject(g_AiuiToolMainEntry_onICataListener_class, g_method_AiuiToolMainEntry_onICataListener_Constructor_init);
	env->CallStaticVoidMethod(g_libisscata_class, g_method_libisscata_SearchCreate, jobjCataNativeHandle, jstrResourceDir, jstrIndexResNames, jobjonICataListener);
	int java_ret = env->CallIntMethod(jobjCataNativeHandle, g_method_CataNativeHandle_getErr_ret);
	if (0 == java_ret) {
		ClibisscataObj* pClibisscataObj = new ClibisscataObj();
		pClibisscataObj->jobjCataNativeHandle = (jobject)env->NewGlobalRef(jobjCataNativeHandle);
		pClibisscataObj->pfnOnCataMsgProc = pfnOnCataMsgProc;
		pClibisscataObj->pUsrArg = pUsrArg;

		env->CallVoidMethod(jobjonICataListener, g_method_AiuiToolMainEntry_onICataListener_setNative_point, (jlong)pClibisscataObj);

		*phISSCATASEARCHObj = (HISSCATASEARCH)pClibisscataObj;
	}

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSCataSearchCreate(phISSCATASEARCHObj, szResourceDir, szIndexResNames, pfnOnCataMsgProc, pUsrArg);
#endif
}
ISSErrID  ISSCATAAdapter::ISSCataSearchCreateEx(HISSCATASEARCH* phISSCATASEARCHObj, const char*  szResourceDir,
	const char* szIndexResNames, Proc_OnCataMsgProc  pfnOnCataMsgProc, int nLangType, void* pUsrArg) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (phISSCATASEARCHObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	jstring jstrResourceDir;
	if (szResourceDir) {
		jstrResourceDir = env->NewStringUTF(szResourceDir);
	} else {
		jstrResourceDir = env->NewStringUTF("");
	}
	jstring jstrIndexResNames;
	if (szIndexResNames) {
		jstrIndexResNames = env->NewStringUTF(szIndexResNames);
	} else {
		jstrIndexResNames = env->NewStringUTF("");
	}
	jobject jobjCataNativeHandle = env->NewObject(g_CataNativeHandle_class, g_method_CataNativeHandle_Constructor_init);
	jobject jobjonICataListener = env->NewObject(g_AiuiToolMainEntry_onICataListener_class, g_method_AiuiToolMainEntry_onICataListener_Constructor_init);
	env->CallStaticVoidMethod(g_libisscata_class, g_method_libisscata_SearchCreateEx, jobjCataNativeHandle, jstrResourceDir, jstrIndexResNames, (jint)nLangType, jobjonICataListener);
	int java_ret = env->CallIntMethod(jobjCataNativeHandle, g_method_CataNativeHandle_getErr_ret);
	if (0 == java_ret) {
		ClibisscataObj* pClibisscataObj = new ClibisscataObj();
		pClibisscataObj->jobjCataNativeHandle = (jobject)env->NewGlobalRef(jobjCataNativeHandle);
		pClibisscataObj->pfnOnCataMsgProc = pfnOnCataMsgProc;
		pClibisscataObj->pUsrArg = pUsrArg;

		env->CallVoidMethod(jobjonICataListener, g_method_AiuiToolMainEntry_onICataListener_setNative_point, (jlong)pClibisscataObj);

		*phISSCATASEARCHObj = (HISSCATASEARCH)pClibisscataObj;
	}

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSCataSearchCreateEx(phISSCATASEARCHObj, szResourceDir, szIndexResNames, pfnOnCataMsgProc, nLangType, pUsrArg);
#endif
}
ISSErrID  ISSCATAAdapter::ISSCataSearchDestroy(HISSCATASEARCH    hISSCATASEARCHObj) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSCATASEARCHObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibisscataObj* pClibisscataObj = (ClibisscataObj*)hISSCATASEARCHObj;
	env->CallStaticVoidMethod(g_libisscata_class, g_method_libisscata_SearchDestroy, pClibisscataObj->jobjCataNativeHandle);
	int java_ret = env->CallIntMethod(pClibisscataObj->jobjCataNativeHandle, g_method_CataNativeHandle_getErr_ret);
	if (0 == java_ret) {
		env->DeleteGlobalRef(pClibisscataObj->jobjCataNativeHandle);
		delete pClibisscataObj;
	}

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSCataSearchDestroy(hISSCATASEARCHObj);
#endif
}
ISSErrID  ISSCATAAdapter::ISSCataSearchAsync(HISSCATASEARCH  hISSCATASEARCHObj, const char* szQuery) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSCATASEARCHObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibisscataObj* pClibisscataObj = (ClibisscataObj*)hISSCATASEARCHObj;
	jstring jstrQuery;
	if (szQuery) {
		jstrQuery = env->NewStringUTF(szQuery);
	} else {
		jstrQuery = env->NewStringUTF("");
	}
	env->CallStaticVoidMethod(g_libisscata_class, g_method_libisscata_SearchAsync, pClibisscataObj->jobjCataNativeHandle, jstrQuery);
	int java_ret = env->CallIntMethod(pClibisscataObj->jobjCataNativeHandle, g_method_CataNativeHandle_getErr_ret);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSCataSearchAsync(hISSCATASEARCHObj, szQuery);
#endif
}
const char* ISSCATAAdapter::ISSCataSearchSync(HISSCATASEARCH hISSCATASEARCHObj, const char* szQuery, ISSErrID* pErr) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSCATASEARCHObj == NULL) {
		return "";
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);
	
	ClibisscataObj* pClibisscataObj = (ClibisscataObj*)hISSCATASEARCHObj;
	jstring jstrQuery;
	if (szQuery) {
		jstrQuery = env->NewStringUTF(szQuery);
	} else {
		jstrQuery = env->NewStringUTF("");
	}

	jstring returnString = (jstring)env->CallStaticObjectMethod(g_libisscata_class, g_method_libisscata_SearchSync, pClibisscataObj->jobjCataNativeHandle, jstrQuery);

	const char* ret = "";
	if (returnString) {
		ret = (const char*)env->GetStringUTFChars(returnString, NULL);
	}
	pClibisscataObj->searchresult = ret;
	if (returnString) {
		env->ReleaseStringUTFChars(returnString, ret);
	}
	if (pErr) {
		*pErr = ISS_SUCCESS;
	}

	gs_jvm->DetachCurrentThread();

	return pClibisscataObj->searchresult.c_str();
#else
	return ::ISSCataSearchSync(hISSCATASEARCHObj, szQuery, pErr);
#endif
}

ISSErrID  ISSCATAAdapter::ISSCataSetParam(HISSCATASEARCH hISSCATASEARCHObj, int pmType, int val) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSCATASEARCHObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibisscataObj* pClibisscataObj = (ClibisscataObj*)hISSCATASEARCHObj;
	env->CallStaticVoidMethod(g_libisscata_class, g_method_libisscata_SetParam, pClibisscataObj->jobjCataNativeHandle, (jint)pmType, (jint)val);
	int java_ret = env->CallIntMethod(pClibisscataObj->jobjCataNativeHandle, g_method_CataNativeHandle_getErr_ret);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSCataSetParam(hISSCATASEARCHObj, pmType, val);
#endif
}
ISSErrID  ISSCATAAdapter::ISSCataSetLogCfgParam(const int iParamID, const char* szParamValue) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	return ISS_ERROR_NOT_IMPLEMENT;
#else
	return ::ISSCataSetLogCfgParam( iParamID,  szParamValue);
#endif
}