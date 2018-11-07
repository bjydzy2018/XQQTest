/*
 * VAInc.h
 *
 *  Created on: 2013-4-10
 *      Author: pffang
 */

#ifndef VAINC_H_
#define VAINC_H_
#include "VACommon.h"
#include "VADebug.h"
#include "VAString.h"
#include "VAList.h"
#include "VARingBuffer.h"
#include "VAFileSystem.h"
#include "MutexSignalCommon.h"
#include "VACityToProvince.h"
#include "VAThread.h"
#if VA_VC

#elif VA_GNUC
#include "VALocalSocketMessage.h"
#include "VAConditionVariable.h"
#if !VA_ANDROID
#include "VASingleton.h"
#include "VAKey.h"
#include "VASemaphore.h"
#include "VAMessageQueue.h"
#include "VAShareMemory.h"
#endif
#endif

namespace VA {

#if defined VA_VC

#elif VA_GNUC 
inline void __PthreadSleep(pthread_cond_t * cond, pthread_mutex_t * mutex, unsigned long int millisecond) {
    struct timespec timeToWait;
    struct timeval now;
    unsigned long int s, us;

    pthread_mutex_lock(mutex);
#if VA_VC
    now.tv_sec = 0;
    now.tv_usec = 0;
#elif VA_GNUC
    gettimeofday(&now, NULL);
#endif
    us = now.tv_usec + 1000UL * millisecond;
    s = us / 1000000UL;
    us = us - s * 1000000UL;

    timeToWait.tv_sec = now.tv_sec + s;
    timeToWait.tv_nsec = us * 1000UL;
    pthread_cond_timedwait(cond, mutex, &timeToWait);
    pthread_mutex_unlock(mutex);
}
inline void __SemSleep(sem_t * sem, unsigned long int millisecond) {
    struct timespec timeToWait;
    struct timeval now;
    // #if VA_VC
    //   now.tv_sec = 0;
    //   now.tv_usec = 0;
    // #elif VA_GNUC
    gettimeofday(&now, NULL);
    // #endif

    unsigned long int s, us;
    us = now.tv_usec + 1000UL * millisecond;
    s = us / 1000000UL;
    us = us - s * 1000000UL;

    timeToWait.tv_sec = now.tv_sec + s;
    timeToWait.tv_nsec = us * 1000UL;
    // wait until timeout or woken up
    errno = 0;
    while ((sem_timedwait(sem, &timeToWait)) == -1 && errno == EINTR) {
        continue;
    }
    //return errno == ETIMEDOUT;  // returns true if a timeout occured
}

struct timespec diff_timespec(const struct timespec& start, const struct timespec& end);
struct timeval diff_timespec(const struct timeval& start, const struct timeval& end);
long long millisec_elapsed(const struct timespec& diff);
long long microsec_elapsed(const struct timespec& diff);
long long nanosec_elapsed(const struct timespec& diff);

#endif

inline char toHex(const char &x);
inline char fromHex(const char &x);
std::string URLEncode(const std::string &sIn);
std::string URLDecode(const std::string &sIn);

//int GetRandom();
std::string IntToString(int n);

bool IsNum(const char * szNum);
bool IsNum(std::string &strNum);

#if VA_GNUC
inline unsigned long GetTickCount(void) {
	struct timespec tp;
	unsigned long t=0;
	int ret = clock_gettime(CLOCK_MONOTONIC, &tp);
	if(ret<0) {
		return -1;
	} else {
		t = tp.tv_sec*1000+tp.tv_nsec/1000000;
		return t;
	}
}
#endif



#if VA_VC
#define PrtU8(szStr)	VA::U8StrToGBKStr(szStr).operator const char*()
#define PrtGBK(szStr)	szStr
#define PrtU16(szU16Str)	VA::U16StrToGBKStr(szU16Str).operator const char*()
#define PrtWStr(szWStr) PrtU16(szWStr)
#else
#define PrtU8(szStr)	szStr
#define PrtGBK(szStr)	VA::GBKStrToU8Str(szStr).operator const U8Char*()
#define PrtU16(szU16Str)	VA::U16StrToU8Str(szU16Str).operator const U8Char*()
#define PrtWStr(szWStr) VA::U32StrToU8Str(szWStr).operator const U8Char*()
#endif

#if VA_VC
#pragma warning(disable:4819)
#include <Windows.h>
#define VA_DLL_MAIN 	BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) { \
	UNREFERENCED_PARAMETER(hModule); \
	UNREFERENCED_PARAMETER(lpReserved); \
	switch (ul_reason_for_call) { \
						case DLL_PROCESS_ATTACH: \
						case DLL_THREAD_ATTACH: \
						case DLL_THREAD_DETACH: \
						case DLL_PROCESS_DETACH: \
						break; \
} \
	return TRUE;}
#elif VA_GNUC
#define VA_DLL_MAIN
#endif

#define VA_TRY try {

//捕捉std抛出的异常，适合返回值为ISSErrID类型的函数
#define VA_CATCH_STD_EXCEPTION	} catch (const std::exception& e) { \
									DEBUG_TRACE_ERROR("%s | exception: %s", __FUNCTION__, e.what()); \
									fprintf(stderr, "%s | exception: %s\n", __FUNCTION__, e.what()); \
									return ISS_ERROR_EXCEPTION; \
								}
//捕捉std抛出的异常，适合返回值为const char*类型的函数
#define VA_CATCH_STD_EXCEPTION_RETURN_CONST_CHAR	} catch (const std::exception& e) { \
													DEBUG_TRACE_ERROR("%s | exception: %s", __FUNCTION__, e.what()); \
													fprintf(stderr, "%s | exception: %s\n", __FUNCTION__, e.what()); \
													return ""; \
													}
//捕捉抛出的ISS_EXCEPTION异常,返回const char*
#define VA_CATCH_RETURN_CONST_CHAR	} catch (ISS_EXCEPTION err) { \
									DEBUG_TRACE_ERROR("%s | ISS_EXCEPTION err: %d", __FUNCTION__, err); \
									return NULL; \
								}
#define VA_CATCH_ISS_EXCEPTION	catch (ISS_EXCEPTION err) { \
	DEBUG_TRACE_ERROR("%s | ISS_EXCEPTION err: %d", __FUNCTION__, err); \
	return err; \
}
#define VA_CATCH_ALL_EXCEPTION catch (...) { \
									DEBUG_TRACE_ERROR("%s | unhandled exception", __FUNCTION__); \
									fprintf(stderr,"%s | unhandled exception", __FUNCTION__); \
									return ISS_ERROR_EXCEPTION; \
								}

}// namespace VA

#endif /* VAINC_H_ */
