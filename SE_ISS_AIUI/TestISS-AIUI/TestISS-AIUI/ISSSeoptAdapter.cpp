#ifndef __x86_64__


#include "ISSSeoptAdapter.h"

#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
#include <jni.h>
#include <android/log.h>
#include <unistd.h>

#define LOG_TAG "ISSSeoptAdapter"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define AUDIO_CHANNELS 4

extern JavaVM* gs_jvm;

extern jclass g_NativeHandle_class;
extern jmethodID g_method_NativeHandle_Constructor_init;
extern jmethodID g_method_NativeHandle_getErr_ret;

extern jclass g_libissseopt_class;
extern jmethodID g_method_libissseopt_create;
extern jmethodID g_method_libissseopt_destroy;
extern jmethodID g_method_libissseopt_process;
extern jmethodID g_method_libissseopt_setParam;
extern jmethodID g_method_libissseopt_getGSCPower;

class ClibissseoptObj
{
public:
	jobject jobjNativeHandle;
	void* pBufOut;
};
#endif


ISSErrID  ISSSeoptAdapter::ISSSeoptCreate(ISSSeoptHandle * pHandle, const char* szResPath) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (pHandle == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	jstring jstrResPath;
	if (szResPath) {
		jstrResPath = env->NewStringUTF(szResPath);
	}
	else {
		jstrResPath = env->NewStringUTF("");
	}
	jobject jobjNativeHandle = env->NewObject(g_NativeHandle_class, g_method_NativeHandle_Constructor_init);

	ClibissseoptObj* pClibissseoptObj = new ClibissseoptObj();
	pClibissseoptObj->jobjNativeHandle = (jobject)env->NewGlobalRef(jobjNativeHandle);
	pClibissseoptObj->pBufOut = NULL;

	int java_ret = env->CallStaticIntMethod(g_libissseopt_class, g_method_libissseopt_create, jobjNativeHandle, jstrResPath);

	if (0 == java_ret) {
		*pHandle = (ISSSeoptHandle)pClibissseoptObj;
	}
	else {
		env->DeleteGlobalRef(pClibissseoptObj->jobjNativeHandle);
		delete pClibissseoptObj;
	}

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSSeoptCreate(pHandle, szResPath);
#endif
}
ISSErrID  ISSSeoptAdapter::ISSSeoptDestroy(ISSSeoptHandle handle) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (handle == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissseoptObj* pClibissseoptObj = (ClibissseoptObj*)handle;
	int java_ret = env->CallStaticIntMethod(g_libissseopt_class, g_method_libissseopt_destroy, pClibissseoptObj->jobjNativeHandle);
	if (0 == java_ret) {
		env->DeleteGlobalRef(pClibissseoptObj->jobjNativeHandle);
		if (pClibissseoptObj->pBufOut) {
			free(pClibissseoptObj->pBufOut);
			pClibissseoptObj->pBufOut = NULL;
		}
		delete pClibissseoptObj;
	}

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSSeoptDestroy(handle);
#endif
}
ISSErrID  ISSSeoptAdapter::ISSSeoptSetParam(ISSSeoptHandle handle, const char* szParam, const char* szParamValue) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (handle == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissseoptObj* pClibissseoptObj = (ClibissseoptObj*)handle;
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
	int java_ret = env->CallStaticIntMethod(g_libissseopt_class, g_method_libissseopt_setParam,
		pClibissseoptObj->jobjNativeHandle, jstrParam, jstrParamValue);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSSeoptSetParam(handle, szParam, szParamValue);
#endif
}
ISSErrID  ISSSeoptAdapter::ISSSeoptProcess(ISSSeoptHandle handle, const void* pBufIn, int nSamplesIn, 
	const void** ppBufOut, int* pnSamplesOut, int* pnChannelsOut) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (handle == NULL || pBufIn==NULL || nSamplesIn==0 || ppBufOut==NULL || pnSamplesOut==NULL || pnChannelsOut==NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissseoptObj* pClibissseoptObj = (ClibissseoptObj*)handle;

	jbyteArray byteArrayIn;
	jbyteArray byteArrayOut;
	jintArray  intArrayOut;
	jint nBytes = nSamplesIn * 2 * AUDIO_CHANNELS;
	if (pBufIn && nSamplesIn) {
		byteArrayIn = env->NewByteArray(nBytes);
		env->SetByteArrayRegion(byteArrayIn, 0, nBytes, (jbyte *)pBufIn);
		byteArrayOut = env->NewByteArray(nBytes);
		intArrayOut = env->NewIntArray(2);
	}

	int java_ret = env->CallStaticIntMethod(g_libissseopt_class, g_method_libissseopt_process,
		pClibissseoptObj->jobjNativeHandle, byteArrayIn, (jint)nSamplesIn, byteArrayOut, intArrayOut);

	jint* arr = env->GetIntArrayElements(intArrayOut, NULL);//得到一个指向原始数据类型内容的指针
	// jint length = env->GetArrayLength(intArrayOut);//得到数组的长度
	*pnSamplesOut = arr[0];
	*pnChannelsOut = arr[1];
	env->ReleaseIntArrayElements(intArrayOut, arr, 0);

	jbyte* byte_arr = env->GetByteArrayElements(byteArrayOut, NULL);
	if (pClibissseoptObj->pBufOut==NULL) {
		pClibissseoptObj->pBufOut = malloc(nBytes);
	}
	if (pClibissseoptObj->pBufOut) {
		memcpy(pClibissseoptObj->pBufOut, byte_arr, nBytes);
		*ppBufOut = pClibissseoptObj->pBufOut;
	} else {
		LOGE("error memory not enough!");
	}
	env->ReleaseByteArrayElements(byteArrayOut, byte_arr, 0);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSSeoptProcess(handle, pBufIn, nSamplesIn, ppBufOut, pnSamplesOut, pnChannelsOut);
#endif
}
ISSErrID  ISSSeoptAdapter::ISSSeoptGetGSCPower(ISSSeoptHandle handle, int nIndex, float* pfPower, float* pfAngle) {
#ifdef AiuiToolMainFuncCallJavaNativeApiMacro
	if (handle == NULL) {
		return ISS_ERROR_INVALID_PARA;
	}

	JNIEnv *env = NULL;
	gs_jvm->AttachCurrentThread(&env, NULL);

	ClibissseoptObj* pClibissseoptObj = (ClibissseoptObj*)handle;
	jfloatArray floatArrayOut = env->NewFloatArray(2);

	int java_ret = env->CallStaticIntMethod(g_libissseopt_class, g_method_libissseopt_getGSCPower,
		pClibissseoptObj->jobjNativeHandle, (jint)nIndex, floatArrayOut);

	jfloat* arr = env->GetFloatArrayElements(floatArrayOut, NULL);//得到一个指向原始数据类型内容的指针
	// jint length = env->GetArrayLength(floatArrayOut);//得到数组的长度
	*pfPower = arr[0];
	*pfAngle = arr[1];
	env->ReleaseFloatArrayElements(floatArrayOut, arr, 0);

	gs_jvm->DetachCurrentThread();

	return java_ret;
#else
	return ::ISSSeoptGetGSCPower( handle,  nIndex, pfPower, pfAngle);
#endif
}

#endif