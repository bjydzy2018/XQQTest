/*
 * VASingleton.h
 *
 *  Created on: 2013-3-25
 *      Author: pffang
 */
#pragma once
// #ifndef VASINGLETON_H_
// #define VASINGLETON_H_
#include "VACommon.h"
namespace VA {
#if VA_GNUC && !VA_ANDROID
class CSingleton {
    int lockfile(int fd);
 public:
    //Return: 0:First Instance; 1: Running;
    int GetLock(const char* pidfilepath);
};
#elif VA_VC
class CSingleton {
	HANDLE hMutex;
public:
	//Return: 0:First Instance; 1: Running;
	int GetLock(const wchar_t* MutexName);
	~CSingleton();
};
#endif
} /* namespace VA */

//#endif /* VASINGLETON_H_ */
