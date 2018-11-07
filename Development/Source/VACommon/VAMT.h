#pragma once
#ifndef VACriticalSection_H_INCLUDED
#define VACriticalSection_H_INCLUDED

#if VA_VC
#include <windows.h>
#if !defined UNDER_CE
#include <process.h>
#endif
#elif VA_GNUC
#include <pthread.h>
#endif

namespace VA{

#if VA_VC
typedef CRITICAL_SECTION VACriticalSection;       
#define VA_CRITICALSECTION_INITIALIZE(A) InitializeCriticalSection(A)
#define VA_CRITICALSECTION_DESTROY(A)    DeleteCriticalSection(A)
#define VA_CRITICALSECTION_LOCK(A)       EnterCriticalSection(A)
#define VA_CRITICALSECTION_UNLOCK(A)     LeaveCriticalSection(A)

typedef HANDLE VAThreadHandle;
#elif VA_GNUC
typedef pthread_mutex_t VACriticalSection;
#define VA_CRITICALSECTION_INITIALIZE(A) pthread_mutex_init(A, NULL)
#define VA_CRITICALSECTION_DESTROY(A)    pthread_mutex_destroy(A)
#define VA_CRITICALSECTION_LOCK(A)       pthread_mutex_lock(A)
#define VA_CRITICALSECTION_UNLOCK(A)     pthread_mutex_unlock(A)

typedef pthread_t VAThreadHandle;
#endif

}
#endif // VACriticalSection_H_INCLUDED