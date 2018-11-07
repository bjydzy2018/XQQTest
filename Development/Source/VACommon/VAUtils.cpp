/*
 * VAUtils.cpp
 *
 *  Created on: 2013-8-14
 *      Author: pffang
 */

#include "VACommon.h"
#include "VAInc.h"
// #include "svn_revision.h"
#if VA_GNUC
#include <stdlib.h>
#endif

namespace VA {

#if VA_GNUC
struct timespec diff_timespec(const struct timespec& start,
		const struct timespec& end) {
	struct timespec result;

	if (end.tv_nsec < start.tv_nsec) { // peform carry like in normal subtraction
		//                123456789
		result.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
		result.tv_sec = end.tv_sec - 1 - start.tv_sec;
	} else {
		result.tv_nsec = end.tv_nsec - start.tv_nsec;
		result.tv_sec = end.tv_sec - start.tv_sec;
	}

	return result;
}
struct timeval diff_timespec(const struct timeval& start,
		const struct timeval& end) {
	struct timeval result;

	if (end.tv_usec < start.tv_usec) { // peform carry like in normal subtraction
		//                123456789
		result.tv_usec = 1000000 + end.tv_usec - start.tv_usec;
		result.tv_sec = end.tv_sec - 1 - start.tv_sec;
	} else {
		result.tv_usec = end.tv_usec - start.tv_usec;
		result.tv_sec = end.tv_sec - start.tv_sec;
	}

	return result;
}

long long millisec_elapsed(const struct timespec& diff) {
	//                                123                     123456
	return ((long long) diff.tv_sec * 1000) + (diff.tv_nsec / 1000000);
}

long long microsec_elapsed(const struct timespec& diff) {
	//                                123456                     123
	return ((long long) diff.tv_sec * 1000000) + (diff.tv_nsec / 1000);
}

long long nanosec_elapsed(const struct timespec& diff) {
	//                                123456789
	return ((long long) diff.tv_sec * 1000000000) + diff.tv_nsec;
}

#endif

inline char toHex(const char &x) {
	return x > 9 ? x - 10 + 'A' : x + '0';
}

inline char fromHex(const char &x) {
	return isdigit(x) ? x - '0' : x - 'A' + 10;
}

std::string URLEncode(const std::string &sIn) {
	std::string sOut;
	for (size_t ix = 0; ix < sIn.size(); ix++) {
		char buf[4];
		memset(buf, 0, 4);
		if (isalnum((char) sIn[ix])) {
			buf[0] = sIn[ix];
		}
		//else if ( isspace( (BYTE)sIn[ix] ) ) //貌似把空格编码成%20或者+都可以
		//{
		//    buf[0] = '+';
		//}
		else {
			buf[0] = '%';
			buf[1] = toHex((char) sIn[ix] >> 4);
			buf[2] = toHex((char) sIn[ix] % 16);
		}
		sOut += (char *) buf;
	}
	return sOut;
}

std::string URLDecode(const std::string &sIn) {
	std::string sOut;
	for (size_t ix = 0; ix < sIn.size(); ix++) {
		char ch = 0;
		if (sIn[ix] == '%') {
			ch = (fromHex(sIn[ix + 1]) << 4);
			ch |= fromHex(sIn[ix + 2]);
			ix += 2;
		} else if (sIn[ix] == '+') {
			ch = ' ';
		} else {
			ch = sIn[ix];
		}
		sOut += (char) ch;
	}
	return sOut;
}

//int GetRandom() {
//	int rnum = 0;
//#if VA_VC
//#if VA_VC_WINCE
//	CeGenRandom(sizeof(int), (PBYTE)&rnum);
//#else
//	HCRYPTPROV hProvider = 0;
//	const DWORD dwLength = sizeof(int);
//	BYTE pbBuffer[dwLength] = {};
//	DWORD result =::CryptAcquireContext(&hProvider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT);
//	assert(result);
//	result = ::CryptGenRandom(hProvider, dwLength, pbBuffer);
//	rnum = *(int*)pbBuffer;
//	assert(result);
//	::CryptReleaseContext(hProvider, 0);
//#endif
//#elif VA_GNUC
//	int fd = open("/dev/urandom", O_RDONLY);
//	if (fd != -1) {
//		(void) read(fd, (void *) &rnum, sizeof(int));
//		(void) close(fd);
//	}
//	else
//	{
//		srand(time(NULL));
//		rnum = rand();
//	}
//#endif
//	return rnum;
//}

std::string IntToString(int n)
{
	char buf[21] = {0};

	sprintf(buf, "%d", n);

	return std::string(buf);
}

bool IsNum(const char * szNum)
{
	if ((!szNum) || (!szNum[0]))
	{
		return false;
	}

	std::stringstream sin(szNum);
	double dTmp;
	char cTmp;
	
	if ((sin >> dTmp) && (!(sin >> cTmp)))
	{
		return true;
	}

	return false;
}

bool IsNum(std::string &strNum)
{
	return IsNum(strNum.c_str());
}

} // namespace VA
