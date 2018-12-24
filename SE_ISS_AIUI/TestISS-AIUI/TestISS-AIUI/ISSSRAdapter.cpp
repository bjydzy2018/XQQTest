#include"ISSSRAdapter.h"

#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
#include <jni.h>
#include <android/log.h>
#include <unistd.h>

#define LOG_TAG "ISSSRAdapter"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern JavaVM* gs_jvm;

extern jclass g_libisssr_class;
extern jmethodID g_method_libisssr_setMachineCode;
extern jmethodID g_method_libisssr_setSerialNumber;
extern jmethodID g_method_libisssr_getActiveKey;
extern jmethodID g_method_libisssr_activate;
extern jmethodID g_method_libisssr_create;
extern jmethodID g_method_libisssr_start;
extern jmethodID g_method_libisssr_uploadDict;
extern jmethodID g_method_libisssr_uploadData;
extern jmethodID g_method_libisssr_querySyncStatusResult;
extern jmethodID g_method_libisssr_setParam;
extern jmethodID g_method_libisssr_appendAudioData;
extern jmethodID g_method_libisssr_endAudioData;
extern jmethodID g_method_libisssr_stop;
extern jmethodID g_method_libisssr_destroy;

extern jobject g_joSRMsgListener;

extern Proc_OnMsgProc g_pfnOnSRMsgProc;
extern void* g_pSRUsrArg;
#endif

ISSErrID ISSSRAdapter::ISSSetMachineCode(const char* szMachineCode) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	jstring jstrMachineCode;
	if (szMachineCode) {
		jstrMachineCode = env->NewStringUTF(szMachineCode);
	} else {
		jstrMachineCode = env->NewStringUTF("");
	}
	int java_ret = env->CallStaticIntMethod(g_libisssr_class, g_method_libisssr_setMachineCode, jstrMachineCode);

	gs_jvm->DetachCurrentThread();
	return java_ret;
#else
	return ::ISSSetMachineCode(szMachineCode);
#endif
}

ISSErrID ISSSRAdapter::ISSSRCreate(HISSSR* phISSSRObj,
	ISS_SR_ACOUS_LANG eAcousLang, const char* szResourceDir,
	Proc_OnMsgProc  pfnOnMsgProc, void*  pUsrArg) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (phISSSRObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	g_pfnOnSRMsgProc = pfnOnMsgProc;
	g_pSRUsrArg = pUsrArg;

	jstring jstrResourceDir;
	if (szResourceDir) {
		jstrResourceDir = env->NewStringUTF(szResourceDir);
	}
	else {
		jstrResourceDir = env->NewStringUTF("");
	}

	int java_ret = env->CallStaticIntMethod(g_libisssr_class, g_method_libisssr_create, (jint)eAcousLang, jstrResourceDir, g_joSRMsgListener);
	gs_jvm->DetachCurrentThread();
	//c层认为返回成功，一般句柄也不为空
	if (java_ret == 0) {
		*phISSSRObj = (void*)1;
	}

	return java_ret;
#else
	return ::ISSSRCreate(phISSSRObj, eAcousLang, szResourceDir, pfnOnMsgProc, pUsrArg);
#endif
}

ISSErrID ISSSRAdapter::ISSSRCreateEx(HISSSR*         phISSSRObj,
	ISS_SR_ACOUS_LANG eAcousLang, const char*     szResourceDir,
	unsigned int	nWorkMode, Proc_OnMsgProc  pfnOnMsgProc,
	void* pUsrArg) {

#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	return 0;
#else
	return ::ISSSRCreateEx(phISSSRObj, eAcousLang, szResourceDir, nWorkMode, pfnOnMsgProc, pUsrArg);
#endif

}

ISSErrID ISSSRAdapter::ISSSRDestroy(HISSSR hISSSRObj) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSSRObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	int java_ret = env->CallStaticIntMethod(g_libisssr_class, g_method_libisssr_destroy);

	gs_jvm->DetachCurrentThread();
	return java_ret;
#else
	return ::ISSSRDestroy(hISSSRObj);
#endif
}

ISSErrID ISSSRAdapter::ISSSRStart(HISSSR hISSSRObj, const char* szScene,
	int iMode, const char* szCmd) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSSRObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	jstring jstrScene;
	if (szScene) {
		jstrScene = env->NewStringUTF(szScene);
	}
	else {
		jstrScene = env->NewStringUTF("");
	}

	jstring jstrCmd;
	if (szCmd) {
		jstrCmd = env->NewStringUTF(szCmd);
	}
	else {
		jstrCmd = env->NewStringUTF("");
	}

	int java_ret = env->CallStaticIntMethod(g_libisssr_class, g_method_libisssr_start, jstrScene, (jint)iMode, jstrCmd);

	gs_jvm->DetachCurrentThread();
	return java_ret;
#else
	return ::ISSSRStart(hISSSRObj, szScene, iMode, szCmd);
#endif
}

ISSErrID ISSSRAdapter::ISSSRStop(HISSSR hISSSRObj) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSSRObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	int java_ret = env->CallStaticIntMethod(g_libisssr_class, g_method_libisssr_stop);

	gs_jvm->DetachCurrentThread();
	return java_ret;
#else
	return ::ISSSRStop(hISSSRObj);
#endif
}

ISSErrID ISSSRAdapter::ISSSRSetParam(HISSSR hISSSRObj, const char* szParam, 
	                                 const char* szParamValue) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSSRObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	jstring jstrParam;
	if (szParam) {
		jstrParam = env->NewStringUTF(szParam);
	}
	else {
		jstrParam = env->NewStringUTF("");
	}

	jstring jstrParamValue;
	if (szParamValue) {
		jstrParamValue = env->NewStringUTF(szParamValue);
	}
	else {
		jstrParamValue = env->NewStringUTF("");
	}

	int java_ret = env->CallStaticIntMethod(g_libisssr_class, g_method_libisssr_setParam, jstrParam, jstrParamValue);

	gs_jvm->DetachCurrentThread();
	return java_ret;
#else
	return ::ISSSRSetParam(hISSSRObj, szParam, szParamValue);
#endif
}

ISSErrID ISSSRAdapter::ISSSRAppendAudioData(HISSSR hISSSRObj, short* pSamples,
	unsigned int nNumberOfToWrite,
	unsigned int* pNumberOfWritten) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSSRObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	jbyteArray byteArray;
	int size = 0;
	char buf[2] = { 0 };
	if (pSamples && nNumberOfToWrite) {
		byteArray = env->NewByteArray(nNumberOfToWrite * 2);
		env->SetByteArrayRegion(byteArray, 0, nNumberOfToWrite*2, (jbyte *)pSamples);
		size = nNumberOfToWrite * 2;
	} else {
		byteArray = env->NewByteArray(2);
		env->SetByteArrayRegion(byteArray, 0, 2, (jbyte *)buf);
		size = 2;
	}

	int java_ret = env->CallStaticIntMethod(g_libisssr_class, g_method_libisssr_appendAudioData, byteArray, (jint)size);
	if (java_ret == 0 && pNumberOfWritten) {
		*pNumberOfWritten = nNumberOfToWrite;
	}

	gs_jvm->DetachCurrentThread();
	return java_ret;
#else
	return ::ISSSRAppendAudioData(hISSSRObj, pSamples, nNumberOfToWrite, pNumberOfWritten);
#endif
}

ISSErrID ISSSRAdapter::ISSSRUpLoadDict(HISSSR hISSSRObj, const char *szList,
	                                   int iUpLoadMode) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSSRObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	jstring jstrList;
	if (szList) {
		jstrList = env->NewStringUTF(szList);
	}
	else {
		jstrList = env->NewStringUTF("");
	}

	int java_ret = env->CallStaticIntMethod(g_libisssr_class, g_method_libisssr_uploadDict, jstrList, (jint)iUpLoadMode);

	gs_jvm->DetachCurrentThread();
	return java_ret;
#else
	return ::ISSSRUpLoadDict(hISSSRObj, szList, iUpLoadMode);
#endif
}

ISSErrID ISSSRAdapter::ISSSRUpLoadData(HISSSR hISSSRObj, const char *szData, 
	                                   int iUpLoadMode) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSSRObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	jstring jstrData;
	if (szData) {
		jstrData = env->NewStringUTF(szData);
	}
	else {
		jstrData = env->NewStringUTF("");
	}

	int java_ret = env->CallStaticIntMethod(g_libisssr_class, g_method_libisssr_uploadData, jstrData, (jint)iUpLoadMode);

	gs_jvm->DetachCurrentThread();
	return java_ret;
#else
	return ::ISSSRUpLoadData(hISSSRObj, szData, iUpLoadMode);
#endif
}

ISSErrID ISSSRAdapter::ISSSRQuerySyncStatus(HISSSR hISSSRObj, const char *szSid) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSSRObj == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	jstring jstrSid;
	if (szSid) {
		jstrSid = env->NewStringUTF(szSid);
	}
	else {
		jstrSid = env->NewStringUTF("");
	}

	int java_ret = env->CallStaticIntMethod(g_libisssr_class, g_method_libisssr_querySyncStatusResult, jstrSid);

	gs_jvm->DetachCurrentThread();
	return java_ret;
#else
	return ::ISSSRQuerySyncStatus(hISSSRObj,szSid);
#endif
}

const char* ISSSRAdapter::ISSGetRevisionNum(void) {
	return ::ISSGetRevisionNum();
}

ISSErrID ISSSRAdapter::ISSSRSetLogCfgParam(const int iParamID, const char* szParamValue) {
	return ::ISSSRSetLogCfgParam(iParamID, szParamValue);
}

ISSErrID ISSSRAdapter::ISSSRClearResult(HISSSR hISSSRObj) {
	return ::ISSSRClearResult(hISSSRObj);
}