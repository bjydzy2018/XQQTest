#include"ISSTTSAdapter.h"

#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
#include <jni.h>
#include <android/log.h>
#include <unistd.h>

#define LOG_TAG "ISSTTSAdapter"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern JavaVM* gs_jvm;

extern jclass g_NativeHandle_class;
extern jmethodID g_method_NativeHandle_Constructor_init;
extern jmethodID g_method_NativeHandle_getErr_ret;

extern jclass g_libisstts_class;
extern jmethodID g_method_libisstts_initRes;
extern jmethodID g_method_libisstts_unInitRes;
extern jmethodID g_method_libisstts_create;
extern jmethodID g_method_libisstts_destroy;
extern jmethodID g_method_libisstts_setParam;
extern jmethodID g_method_libisstts_setParamEx;
extern jmethodID g_method_libisstts_start;
extern jmethodID g_method_libisstts_getAudioData;
extern jmethodID g_method_libisstts_stop;

extern jclass g_AiuiToolMainEntry_onTtsListener_class;
extern jmethodID g_method_AiuiToolMainEntry_onTtsListener_Constructor_init;
extern jmethodID g_method_AiuiToolMainEntry_onTtsListener_setNative_point;

class ClibissttsObj
{
public:
	jobject jobjNativeHandle;

	Proc_OnTTSDataReady pcbOnTTSDataReady;
	void* pUsrArg;
};

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL Java_com_iflytek_nativetool_AiuiToolMainEntry_onNativeDataReady (JNIEnv *env, jclass jc, jlong jNative_point) {
	ClibissttsObj* pClibissttsObj = (ClibissttsObj*)jNative_point;
	if (pClibissttsObj && pClibissttsObj->pcbOnTTSDataReady) {
		pClibissttsObj->pcbOnTTSDataReady(pClibissttsObj->pUsrArg);
	}
}
#ifdef __cplusplus
}
#endif
#endif


ISSErrID  ISSTTSAdapter::ISSTTSInitRes(HISSTTSRES* phISSTTSRES, const char* szResource, int iResMode) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (phISSTTSRES == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	jstring jstrResourceDir;
	if (szResource) {
		jstrResourceDir = env->NewStringUTF(szResource);
	} else {
		jstrResourceDir = env->NewStringUTF("");
	}
	int java_ret = env->CallStaticIntMethod(g_libisstts_class, g_method_libisstts_initRes, jstrResourceDir, (jint)iResMode);
	if (java_ret == 0) {
		*phISSTTSRES = (void*)1;
	}

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSTTSInitRes(phISSTTSRES, szResource, iResMode);
#endif
}
ISSErrID  ISSTTSAdapter::ISSTTSInitResW(HISSTTSRES* phISSTTSRES, const wchar_t* szResource, int iResMode) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	return ISS_ERROR_NOT_IMPLEMENT;
#else
	return ::ISSTTSInitResW(phISSTTSRES, szResource, iResMode);
#endif
}
ISSErrID  ISSTTSAdapter::ISSTTSUnInitRes(HISSTTSRES hISSTTSRES) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSTTSRES == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	int java_ret = env->CallStaticIntMethod(g_libisstts_class, g_method_libisstts_unInitRes);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSTTSUnInitRes(hISSTTSRES);
#endif
}
ISSErrID  ISSTTSAdapter::ISSTTSCreate(HISSTTS* phISSTTS, HISSTTSRES hISSTTSRES, Proc_OnTTSDataReady pcbOnTTSDataReady,
	Proc_OnISSChipCryptError pcbOnISSChipCryptError, void* pUsrArg) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (phISSTTS==NULL || hISSTTSRES==NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	jobject jobjNativeHandle = env->NewObject(g_NativeHandle_class, g_method_NativeHandle_Constructor_init);
	jobject jobjonTtsListener = env->NewObject(g_AiuiToolMainEntry_onTtsListener_class, g_method_AiuiToolMainEntry_onTtsListener_Constructor_init);
	env->CallStaticVoidMethod(g_libisstts_class, g_method_libisstts_create, jobjNativeHandle, jobjonTtsListener);
	jint err_ret = env->CallIntMethod(jobjNativeHandle, g_method_NativeHandle_getErr_ret);
	if (0 == err_ret) {
		ClibissttsObj* pClibissttsObj = new ClibissttsObj();
		pClibissttsObj->jobjNativeHandle = (jobject)env->NewGlobalRef(jobjNativeHandle);
		pClibissttsObj->pcbOnTTSDataReady = pcbOnTTSDataReady;
		pClibissttsObj->pUsrArg = pUsrArg;

		env->CallVoidMethod(jobjonTtsListener, g_method_AiuiToolMainEntry_onTtsListener_setNative_point, (jlong)pClibissttsObj);

		*phISSTTS = (HISSTTS)pClibissttsObj;
	}

	gs_jvm->DetachCurrentThread();

	return err_ret;
#else
	return ::ISSTTSCreate(phISSTTS, hISSTTSRES, pcbOnTTSDataReady,pcbOnISSChipCryptError, pUsrArg);
#endif
}
ISSErrID  ISSTTSAdapter::ISSTTSDestroy(HISSTTS hISSTTS) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSTTS == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissttsObj* pClibissttsObj = (ClibissttsObj*)hISSTTS;
	env->CallStaticVoidMethod(g_libisstts_class, g_method_libisstts_destroy, pClibissttsObj->jobjNativeHandle);
	jint err_ret = env->CallIntMethod(pClibissttsObj->jobjNativeHandle, g_method_NativeHandle_getErr_ret);
	if (0 == err_ret) {
		env->DeleteGlobalRef(pClibissttsObj->jobjNativeHandle);
		delete pClibissttsObj;
	}

	gs_jvm->DetachCurrentThread();

	return err_ret;
#else
	return ::ISSTTSDestroy(hISSTTS);
#endif
}
ISSErrID  ISSTTSAdapter::ISSTTSSetParam(HISSTTS hISSTTS, int iParamID, int iParamValue) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSTTS == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissttsObj* pClibissttsObj = (ClibissttsObj*)hISSTTS;
	env->CallStaticVoidMethod(g_libisstts_class, g_method_libisstts_setParam, pClibissttsObj->jobjNativeHandle, (jint)iParamID, (jint)iParamValue);
	jint err_ret = env->CallIntMethod(pClibissttsObj->jobjNativeHandle, g_method_NativeHandle_getErr_ret);

	gs_jvm->DetachCurrentThread();

	return err_ret;
#else
	return ::ISSTTSSetParam(hISSTTS, iParamID, iParamValue);
#endif
}
ISSErrID  ISSTTSAdapter::ISSTTSSetParamEx(HISSTTS hISSTTS, int iParamID, const char* strParam) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSTTS == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissttsObj* pClibissttsObj = (ClibissttsObj*)hISSTTS;
	jstring jstrParam;
	if (strParam) {
		jstrParam = env->NewStringUTF(strParam);
	} else {
		jstrParam = env->NewStringUTF("");
	}
	env->CallStaticVoidMethod(g_libisstts_class, g_method_libisstts_setParamEx, pClibissttsObj->jobjNativeHandle, (jint)iParamID, jstrParam);
	jint err_ret = env->CallIntMethod(pClibissttsObj->jobjNativeHandle, g_method_NativeHandle_getErr_ret);

	gs_jvm->DetachCurrentThread();

	return err_ret;
#else
	return ::ISSTTSSetParamEx(hISSTTS, iParamID, strParam);
#endif
}
ISSErrID  ISSTTSAdapter::ISSTTSStart(HISSTTS hISSTTS, const void* pText, int iSize, ISSTTSCodePage iTTSCodePage) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSTTS == NULL || iTTSCodePage != ISS_TTS_CODEPAGE_UTF8) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	jstring jstrText;
	if (pText) {
		jstrText = env->NewStringUTF((const char*)pText);
	} else {
		jstrText = env->NewStringUTF("");
	}
	ClibissttsObj* pClibissttsObj = (ClibissttsObj*)hISSTTS;
	env->CallStaticVoidMethod(g_libisstts_class, g_method_libisstts_start, pClibissttsObj->jobjNativeHandle, jstrText);
	jint err_ret = env->CallIntMethod(pClibissttsObj->jobjNativeHandle, g_method_NativeHandle_getErr_ret);

	gs_jvm->DetachCurrentThread();

	return err_ret;
#else
	return ::ISSTTSStart(hISSTTS, pText, iSize, iTTSCodePage);
#endif
}
ISSErrID  ISSTTSAdapter::ISSTTSGetAudioData(HISSTTS hISSTTS, void* pOutBuffer,
	unsigned int iBufferFrames, unsigned int* piBufferFramesGot,
	unsigned int* piProgBegin, unsigned int* piProgLen) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSTTS == NULL || pOutBuffer==NULL || iBufferFrames==0 || piBufferFramesGot==NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissttsObj* pClibissttsObj = (ClibissttsObj*)hISSTTS;
	jint nBytes = iBufferFrames * 2;
	jbyteArray audioBuffer = env->NewByteArray(nBytes);
	jintArray outBytes = env->NewIntArray(1);
	env->CallStaticVoidMethod(g_libisstts_class, g_method_libisstts_getAudioData, pClibissttsObj->jobjNativeHandle, audioBuffer, nBytes, outBytes);

	jint err_ret = env->CallIntMethod(pClibissttsObj->jobjNativeHandle, g_method_NativeHandle_getErr_ret);

	jint* pOutBytes = env->GetIntArrayElements(outBytes, NULL);
	*piBufferFramesGot = pOutBytes[0] / 2;
	env->ReleaseIntArrayElements(outBytes, pOutBytes, 0);

	jbyte* p_audioBuffer = env->GetByteArrayElements(audioBuffer, NULL);
	memcpy(pOutBuffer, p_audioBuffer, pOutBytes[0]);
	env->ReleaseByteArrayElements(audioBuffer, p_audioBuffer, 0);

	gs_jvm->DetachCurrentThread();

	return err_ret;
#else
	return ::ISSTTSGetAudioData(hISSTTS, pOutBuffer, iBufferFrames, piBufferFramesGot, piProgBegin, piProgLen);
#endif
}
ISSErrID  ISSTTSAdapter::ISSTTSStop(HISSTTS hISSTTS) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSTTS == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}
	
	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);
	
	ClibissttsObj* pClibissttsObj = (ClibissttsObj*)hISSTTS;
	env->CallStaticVoidMethod(g_libisstts_class, g_method_libisstts_stop, pClibissttsObj->jobjNativeHandle);
	jint err_ret = env->CallIntMethod(pClibissttsObj->jobjNativeHandle, g_method_NativeHandle_getErr_ret);

	gs_jvm->DetachCurrentThread();

	return err_ret;
#else
	return ::ISSTTSStop(hISSTTS);
#endif
}
ISSErrID  ISSTTSAdapter::ISSTTSGetUsrArg(HISSTTS hISSTTS, void** ppUsrArg) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (hISSTTS == NULL || ppUsrArg==NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissttsObj* pClibissttsObj = (ClibissttsObj*)hISSTTS;
	*ppUsrArg = pClibissttsObj->pUsrArg;

	gs_jvm->DetachCurrentThread();

	return ISS_SUCCESS;
#else
	// return ::ISSTTSGetUsrArg(hISSTTS, ppUsrArg);
	return ISS_ERROR_NOT_IMPLEMENT;
#endif
}
ISSErrID  ISSTTSAdapter::ISSTTSSetLogCfgParam(const int iParamID, const char* szParamValue) {
	return ::ISSTTSSetLogCfgParam(iParamID, szParamValue);
}
