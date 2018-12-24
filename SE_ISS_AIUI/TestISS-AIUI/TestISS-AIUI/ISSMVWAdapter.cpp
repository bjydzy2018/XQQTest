#include"ISSMVWAdapter.h"

#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
#include <jni.h>
#include <android/log.h>
#include <unistd.h>

#define LOG_TAG "ISSMVWAdapter"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern JavaVM* gs_jvm;

extern jclass g_NativeHandle_class;
extern jmethodID g_method_NativeHandle_Constructor_init;
extern jmethodID g_method_NativeHandle_getErr_ret;

extern jclass g_libissmvw_class;
extern jmethodID g_method_libissmvw_create;
extern jmethodID g_method_libissmvw_createEx;
extern jmethodID g_method_libissmvw_destroy;
extern jmethodID g_method_libissmvw_setThreshold;
extern jmethodID g_method_libissmvw_setParam;
extern jmethodID g_method_libissmvw_start;
extern jmethodID g_method_libissmvw_addstartscene;
extern jmethodID g_method_libissmvw_appendAudioData;
extern jmethodID g_method_libissmvw_stop;
extern jmethodID g_method_libissmvw_stopscene;
extern jmethodID g_method_libissmvw_setMvwKeyWords;
extern jmethodID g_method_libissmvw_setMvwDefaultKeyWords;
extern jmethodID g_method_libissmvw_setMvwLanguage;
extern jmethodID g_method_libissmvw_isCouldAppendAudioData;

extern jclass g_AiuiToolMainEntry_onIMVWListener_class;
extern jmethodID g_method_AiuiToolMainEntry_onIMVWListener_Constructor_init;
extern jmethodID g_method_AiuiToolMainEntry_onIMVWListener_setNative_point;

class ClibissmvwObj
{
public:
	jobject jobjNativeHandle;

	Proc_ISSMVWOnWakeup  pfnOnWakeup;
	Proc_MVWOnMsgProc pfnMVWOnMsgProc;
	void* pUsrArg;
};

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL Java_com_iflytek_nativetool_AiuiToolMainEntry_onMVWNativeWakeup
(JNIEnv *env, jclass jc, jlong jNative_point, jint nMvwScene, jint nMvwId, jint nMvwScore, jstring jlParam) {
	ClibissmvwObj* pClibissmvwObj = (ClibissmvwObj*)jNative_point;
	const char* lParam = NULL;
	if (jlParam) {
		lParam = (const char*)env->GetStringUTFChars(jlParam, NULL);
	}
	if (pClibissmvwObj && pClibissmvwObj->pfnOnWakeup) {
		pClibissmvwObj->pfnOnWakeup(pClibissmvwObj->pUsrArg, nMvwScene, nMvwId, nMvwScore, lParam);
	}
	if (jlParam) {
		env->ReleaseStringUTFChars(jlParam, lParam);
	}
}
JNIEXPORT void JNICALL Java_com_iflytek_nativetool_AiuiToolMainEntry_onMVWNativeMsgProc
(JNIEnv *env, jclass jc, jlong jNative_point, jlong uMsg, jlong wParam, jstring jlParam) {
	ClibissmvwObj* pClibissmvwObj = (ClibissmvwObj*)jNative_point;
	const char* lParam = NULL;
	if (jlParam) {
		lParam = (const char*)env->GetStringUTFChars(jlParam, NULL);
	}
	if (pClibissmvwObj && pClibissmvwObj->pfnMVWOnMsgProc) {
		pClibissmvwObj->pfnMVWOnMsgProc(pClibissmvwObj->pUsrArg, uMsg, wParam, lParam);
	}
	if (jlParam) {
		env->ReleaseStringUTFChars(jlParam, lParam);
	}
}
#ifdef __cplusplus
}
#endif
#endif

ISSErrID ISSMVWAdapter::ISSMVWCreate(HISSMVW* phISSMVW, const char* szResourceDir, Proc_ISSMVWOnWakeup  pfnOnWakeup,
	Proc_OnISSChipCryptError pcbOnISSChipCryptError, void* pUsrArg) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (phISSMVW == NULL) {
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
	jobject jobjNativeHandle = env->NewObject(g_NativeHandle_class, g_method_NativeHandle_Constructor_init);
	jobject jobjonIMVWListener = env->NewObject(g_AiuiToolMainEntry_onIMVWListener_class, g_method_AiuiToolMainEntry_onIMVWListener_Constructor_init);

	ClibissmvwObj* pClibissmvwObj = new ClibissmvwObj();
	pClibissmvwObj->jobjNativeHandle = (jobject)env->NewGlobalRef(jobjNativeHandle);
	pClibissmvwObj->pfnOnWakeup = pfnOnWakeup;
	pClibissmvwObj->pfnMVWOnMsgProc = NULL;
	pClibissmvwObj->pUsrArg = pUsrArg;
	env->CallVoidMethod(jobjonIMVWListener, g_method_AiuiToolMainEntry_onIMVWListener_setNative_point, (jlong)pClibissmvwObj);

	//jobjNativeHandle¡¢jobjonIMVWListener
	int java_ret = env->CallStaticIntMethod(g_libissmvw_class, g_method_libissmvw_create, jobjNativeHandle, jstrResourceDir, jobjonIMVWListener);
	if (0 == java_ret) {
		*phISSMVW = (HISSMVW)pClibissmvwObj;
	}
	else {
		env->DeleteGlobalRef(pClibissmvwObj->jobjNativeHandle);
		delete pClibissmvwObj;
	}

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSMVWCreate(phISSMVW,szResourceDir, pfnOnWakeup,pcbOnISSChipCryptError, pUsrArg);
#endif
}
ISSErrID ISSMVWAdapter::ISSMVWCreateW(HISSMVW* phISSMVW, const wchar_t* szResourceDir, Proc_ISSMVWOnWakeup  pfnOnWakeup,
	Proc_OnISSChipCryptError pcbOnISSChipCryptError, void* pUsrArg) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	return ISS_ERROR_NOT_IMPLEMENT;
#else
	return ::ISSMVWCreateW(phISSMVW, szResourceDir, pfnOnWakeup, pcbOnISSChipCryptError, pUsrArg);
#endif
}
ISSErrID ISSMVWAdapter::ISSMVWCreateEx(HISSMVW* phISSMVW, const char* szResourceDir, Proc_MVWOnMsgProc pfnMVWOnMsgProc, void* pUsrArg) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (phISSMVW == NULL) {
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
	jobject jobjNativeHandle = env->NewObject(g_NativeHandle_class, g_method_NativeHandle_Constructor_init);
	jobject jobjonIMVWListener = env->NewObject(g_AiuiToolMainEntry_onIMVWListener_class, g_method_AiuiToolMainEntry_onIMVWListener_Constructor_init);
	int java_ret = env->CallStaticIntMethod(g_libissmvw_class, g_method_libissmvw_createEx, jobjNativeHandle, jstrResourceDir, jobjonIMVWListener);
	if (0 == java_ret) {
		ClibissmvwObj* pClibissmvwObj = new ClibissmvwObj();
		pClibissmvwObj->jobjNativeHandle = (jobject)env->NewGlobalRef(jobjNativeHandle);
		pClibissmvwObj->pfnOnWakeup = NULL;
		pClibissmvwObj->pfnMVWOnMsgProc = pfnMVWOnMsgProc;
		pClibissmvwObj->pUsrArg = pUsrArg;
		
		env->CallVoidMethod(jobjonIMVWListener, g_method_AiuiToolMainEntry_onIMVWListener_setNative_point, (jlong)pClibissmvwObj);

		*phISSMVW = (HISSMVW)pClibissmvwObj;
	}

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSMVWCreateEx(phISSMVW, szResourceDir, pfnMVWOnMsgProc, pUsrArg);
#endif
}
ISSErrID ISSMVWAdapter::ISSMVWCreateWEx(HISSMVW* phISSMVW, const wchar_t* szResourceDir, Proc_MVWOnMsgProc pfnMVWOnMsgProc, void* pUsrArg) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	return ISS_ERROR_NOT_IMPLEMENT;
#else
	return ::ISSMVWCreateWEx(phISSMVW, szResourceDir, pfnMVWOnMsgProc, pUsrArg);
#endif
}
ISSErrID ISSMVWAdapter::ISSMVWDestroy(HISSMVW  hISSMVW) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSMVW == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissmvwObj* pClibissmvwObj = (ClibissmvwObj*)hISSMVW;
	int java_ret = env->CallStaticIntMethod(g_libissmvw_class, g_method_libissmvw_destroy, pClibissmvwObj->jobjNativeHandle);
	if (0 == java_ret) {
		env->DeleteGlobalRef(pClibissmvwObj->jobjNativeHandle);
		delete pClibissmvwObj;
	}

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSMVWDestroy(hISSMVW);
#endif
}
ISSErrID ISSMVWAdapter::ISSMVWSetParam(HISSMVW hISSMVW, const char* szParam, const char* szParamValue) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSMVW == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissmvwObj* pClibissmvwObj = (ClibissmvwObj*)hISSMVW;
	jstring jstrParam;
	if (szParam) {
		jstrParam = env->NewStringUTF((const char*)szParam);
	} else {
		jstrParam = env->NewStringUTF("");
	}
	jstring jstrParamValue;
	if (szParamValue) {
		jstrParamValue = env->NewStringUTF((const char*)szParamValue);
	} else {
		jstrParamValue = env->NewStringUTF("");
	}
	int java_ret = env->CallStaticIntMethod(g_libissmvw_class, g_method_libissmvw_setParam,
		pClibissmvwObj->jobjNativeHandle, jstrParam, jstrParamValue);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSMVWSetParam(hISSMVW, szParam, szParamValue);
#endif
}
ISSErrID ISSMVWAdapter::ISSMVWSetThreshold(HISSMVW  hISSMVW, unsigned int  nMvwScene, unsigned int nMvwId, int nThreshold) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSMVW == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissmvwObj* pClibissmvwObj = (ClibissmvwObj*)hISSMVW;
	int java_ret = env->CallStaticIntMethod(g_libissmvw_class, g_method_libissmvw_setThreshold,
		pClibissmvwObj->jobjNativeHandle, (jint)nMvwScene, (jint)nMvwId, (jint)nThreshold);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSMVWSetThreshold(hISSMVW, nMvwScene, nMvwId, nThreshold);
#endif
}
ISSErrID ISSMVWAdapter::ISSMVWStart(HISSMVW  hISSMVW, unsigned int  nMvwScene) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSMVW == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissmvwObj* pClibissmvwObj = (ClibissmvwObj*)hISSMVW;
	int java_ret = env->CallStaticIntMethod(g_libissmvw_class, g_method_libissmvw_start,
		pClibissmvwObj->jobjNativeHandle, (jint)nMvwScene);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSMVWStart(hISSMVW, nMvwScene);
#endif
}
ISSErrID ISSMVWAdapter::ISSMVWAddStartScene(HISSMVW  hISSMVW, unsigned int  nMvwScene) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSMVW == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissmvwObj* pClibissmvwObj = (ClibissmvwObj*)hISSMVW;
	int java_ret = env->CallStaticIntMethod(g_libissmvw_class, g_method_libissmvw_addstartscene,
		pClibissmvwObj->jobjNativeHandle, (jint)nMvwScene);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSMVWAddStartScene(hISSMVW, nMvwScene);
#endif
}
ISSErrID ISSMVWAdapter::ISSMVWAppendAudioData(HISSMVW  hISSMVW, short* pSamples, unsigned int nNumberOfToWrite, unsigned int* pNumberOfWritten) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSMVW == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissmvwObj* pClibissmvwObj = (ClibissmvwObj*)hISSMVW;
	jbyteArray byteArray;
	int size = 0;
	char buf[2] = { 0 };
	if (pSamples && nNumberOfToWrite) {
		byteArray = env->NewByteArray(nNumberOfToWrite * 2);
		env->SetByteArrayRegion(byteArray, 0, nNumberOfToWrite * 2, (jbyte *)pSamples);
		size = nNumberOfToWrite * 2;
	} else {
		byteArray = env->NewByteArray(2);
		env->SetByteArrayRegion(byteArray, 0, 2, (jbyte *)buf);
		size = 2;
	}
	int java_ret = env->CallStaticIntMethod(g_libissmvw_class, g_method_libissmvw_appendAudioData,
		pClibissmvwObj->jobjNativeHandle, byteArray, (jint)size);
	if (java_ret == 0 && pNumberOfWritten) {
		*pNumberOfWritten = nNumberOfToWrite;
	}

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSMVWAppendAudioData(hISSMVW, pSamples, nNumberOfToWrite, pNumberOfWritten);
#endif
}
ISSErrID ISSMVWAdapter::ISSMVWStop(HISSMVW  hISSMVW) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSMVW == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissmvwObj* pClibissmvwObj = (ClibissmvwObj*)hISSMVW;
	int java_ret = env->CallStaticIntMethod(g_libissmvw_class, g_method_libissmvw_stop, pClibissmvwObj->jobjNativeHandle);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSMVWStop(hISSMVW);
#endif
}
ISSErrID ISSMVWAdapter::ISSMVWStopScene(HISSMVW  hISSMVW, unsigned int  nMvwScene) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSMVW == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissmvwObj* pClibissmvwObj = (ClibissmvwObj*)hISSMVW;
	int java_ret = env->CallStaticIntMethod(g_libissmvw_class, g_method_libissmvw_stopscene,
		pClibissmvwObj->jobjNativeHandle, (jint)nMvwScene);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSMVWStopScene(hISSMVW, nMvwScene);
#endif
}
ISSErrID ISSMVWAdapter::ISSMVWSetKeyWords(HISSMVW  hISSMVW, unsigned int nMvwScene, const char* szWords) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSMVW == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissmvwObj* pClibissmvwObj = (ClibissmvwObj*)hISSMVW;
	jstring jstrWords;
	if (szWords) {
		jstrWords = env->NewStringUTF((const char*)szWords);
	}
	else {
		jstrWords = env->NewStringUTF("");
	}
	int java_ret = env->CallStaticIntMethod(g_libissmvw_class, g_method_libissmvw_setMvwKeyWords,
		pClibissmvwObj->jobjNativeHandle, (jint)nMvwScene, jstrWords);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSMVWSetKeyWords(hISSMVW, nMvwScene, szWords);
#endif
}
ISSErrID ISSMVWAdapter::ISSMVWSetDefaultKeyWords(HISSMVW  hISSMVW, unsigned int nMvwScene) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSMVW == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissmvwObj* pClibissmvwObj = (ClibissmvwObj*)hISSMVW;
	int java_ret = env->CallStaticIntMethod(g_libissmvw_class, g_method_libissmvw_setMvwDefaultKeyWords,
		pClibissmvwObj->jobjNativeHandle, (jint)nMvwScene);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSMVWSetDefaultKeyWords(hISSMVW, nMvwScene);
#endif
}
ISSErrID ISSMVWAdapter::ISSMVWSetMvwLanguage(unsigned int nLangType) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);
	int java_ret = env->CallStaticIntMethod(g_libissmvw_class, g_method_libissmvw_setMvwLanguage, (jint)nLangType);
	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSMVWSetMvwLanguage(nLangType);
#endif
}
bool ISSMVWAdapter::ISSMVWIsCouldAppendAudioData() {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);
	bool java_ret = env->CallStaticBooleanMethod(g_libissmvw_class, g_method_libissmvw_isCouldAppendAudioData);
	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSMVWIsCouldAppendAudioData();
#endif
}
ISSErrID ISSMVWAdapter::ISSMVWSetLogCfgParam(const int iParamID, const char* szParamValue) {
	return ISS_ERROR_NOT_IMPLEMENT;
}
