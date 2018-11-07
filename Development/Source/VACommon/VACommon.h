/*
 * VACommon.h
 *
 *  Created on: 2013-4-8
 *      Author: pffang
 */
#pragma once
#ifndef VACOMMON_H_
#define VACOMMON_H_
#include "VAPlatform.h"

#if VA_VC
#pragma warning(disable:4819)
#pragma warning(disable:4995)
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#if VA_VC_WINCE
// #include "../pthreads.wce/pthread.h"
// #include "../pthreads.wce/semaphore.h"
#endif
#elif VA_GNUC
#if !VA_ANDROID && !VA_QNX
#include <execinfo.h>
#include <linux/kernel.h>
#include <cxxabi.h>
#endif
#include <locale>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <wchar.h>
#if __GXX_EXPERIMENTAL_CXX0X__ || __cplusplus >= 201103L
#include <cstdint>
#else
#include <stdint.h>
#endif
#include <cerrno>
#endif

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <queue>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <exception>
#include <algorithm>
#include <functional> 
#include <cctype>
#include <cassert>
#include <cwchar>
#include <climits>
#include <cstdarg>
#include <ctime>
#include <cmath>

#ifndef BYTE 
typedef unsigned char BYTE;
#endif
#ifndef WORD 
typedef unsigned short WORD;
#endif
#ifndef DWORD 
typedef unsigned long DWORD;
#endif

#ifndef UINT8 
typedef unsigned char UINT8;
#endif
#ifndef WORD 
typedef unsigned short WORD;
#endif
#ifndef DWORD 
typedef unsigned long DWORD;
#endif

#endif /* VACOMMON_H_ */
