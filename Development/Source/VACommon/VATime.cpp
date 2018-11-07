/*
 * VATime.cpp
 *
 */

#include "VATime.h"
#if VA_VC
#include <iostream>
#include <windows.h>
#elif VA_GNUC
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#endif

namespace VA {

/*
 * GetCurrentSysTime - 获取当前系统时间(UTC)
 *
 * 获取当前系统时间，即UTC时间
 * 格式化输出本地时间，输出格式为"YYYY-MM-DD WWW hh-mm-ss.uuuuuu"
 *
 * @return: std::string
 */
std::string GetCurrentSysTime(void)
{
	char str_time[50] = {0};
#if VA_VC
	SYSTEMTIME sys_time, base_sys_time;
	FILETIME file_time, base_file_time; /* FILETIME以100纳秒为单位 */
	ULARGE_INTEGER uli, base_uli; 
	int usec;
#elif VA_GNUC
	time_t t;
	struct tm *sys_time;
	struct timeval cur_time;
#endif
	const char *week_day[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

#if VA_VC
	GetSystemTime(&sys_time);
	SystemTimeToFileTime(&sys_time, &file_time);
	uli.LowPart = file_time.dwLowDateTime;
	uli.HighPart = file_time.dwHighDateTime;
	memset(&base_sys_time, 0, sizeof(base_sys_time));
	base_sys_time.wYear = 1970;
	base_sys_time.wMonth = 1;
	base_sys_time.wDay = 1;
	base_sys_time.wDayOfWeek = 5;
	SystemTimeToFileTime(&base_sys_time, &base_file_time);
	base_uli.LowPart = base_file_time.dwLowDateTime;
	base_uli.HighPart = base_file_time.dwHighDateTime;
	usec = (int)((uli.QuadPart / 10 - base_uli.QuadPart / 10) % 1000000);
	sprintf_s(str_time, sizeof(str_time), "%04d-%02d-%02d %s %02d-%02d-%02d.%06d", sys_time.wYear, sys_time.wMonth, sys_time.wDay, 
		week_day[sys_time.wDayOfWeek], sys_time.wHour, sys_time.wMinute, sys_time.wSecond, usec);
#elif VA_GNUC
	t = time(NULL);
	gettimeofday(&cur_time, NULL);
	sys_time = gmtime(&t);
	snprintf(str_time, sizeof(str_time), "%04d-%02d-%02d %s %02d-%02d-%02d.%06d", sys_time->tm_year + 1900, sys_time->tm_mon + 1, sys_time->tm_mday,
		week_day[sys_time->tm_wday], sys_time->tm_hour, sys_time->tm_min, sys_time->tm_sec, (int)(cur_time.tv_usec));
#endif

	return std::string(str_time);
}

/*
 * GetCurrentLocalTime - 获取当前本地时间(当前所属时区)
 *
 * 获取当前本地时间，即系统当前所属时区的时间
 * 格式化输出本地时间，输出格式为"YYYY-MM-DD WWW hh-mm-ss.uuuuuu"
 *
 * @return: std::string
 */
std::string GetCurrentLocalTime(void)
{
	char str_time[50] = {0};
#if VA_VC
	SYSTEMTIME local_time, base_local_time;
	FILETIME file_time, base_file_time; /* FILETIME以100纳秒为单位 */
	ULARGE_INTEGER uli, base_uli; 
	int usec;
#elif VA_GNUC
	time_t t;
	struct tm *local_time;
	struct timeval cur_time;
#endif
    const char *week_day[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

#if VA_VC
    GetLocalTime(&local_time);
	SystemTimeToFileTime(&local_time, &file_time);
	uli.LowPart = file_time.dwLowDateTime;
	uli.HighPart = file_time.dwHighDateTime;
	memset(&base_local_time, 0, sizeof(base_local_time));
	base_local_time.wYear = 1970;
	base_local_time.wMonth = 1;
	base_local_time.wDay = 1;
	base_local_time.wDayOfWeek = 5;
	SystemTimeToFileTime(&base_local_time, &base_file_time);
	base_uli.LowPart = base_file_time.dwLowDateTime;
	base_uli.HighPart = base_file_time.dwHighDateTime;
	usec = (int)((uli.QuadPart / 10 - base_uli.QuadPart / 10) % 1000000);
    sprintf_s(str_time, sizeof(str_time), "%04d-%02d-%02d %s %02d-%02d-%02d.%06d", local_time.wYear, local_time.wMonth, local_time.wDay, 
		week_day[local_time.wDayOfWeek], local_time.wHour, local_time.wMinute, local_time.wSecond, usec);
#elif VA_GNUC
	t = time(NULL);
	gettimeofday(&cur_time, NULL);
	local_time = localtime(&t);
	snprintf(str_time, sizeof(str_time), "%04d-%02d-%02d %s %02d-%02d-%02d.%06d", local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
		week_day[local_time->tm_wday], local_time->tm_hour, local_time->tm_min, local_time->tm_sec, (int)(cur_time.tv_usec));
#endif

    return std::string(str_time);
}

/*
 * GetCurrentTickCount - 获取从操作系统启动到当前所经过的毫秒数
 *
 * 获取从操作系统启动到当前所经过的毫秒数
 * 可以存储的最大值是(2 ^ 32 - 1)ms，约为49.71天，因此若系统运行时间超过49.71天时，这个数就会归0
 *
 * @return: 返回实际的毫秒数
 */
unsigned long GetCurrentTickCount(void)
{
#if VA_VC
	return GetTickCount();
#elif VA_GNUC
	struct timespec tp;

	clock_gettime(CLOCK_MONOTONIC, &tp);
	return (tp.tv_sec * 1000 + tp.tv_nsec / 1000000);
#endif
}

/*
 * GetCurrentSysTimeOffset - 获取从Epoch, 1970-01-01 00:00:00 +0000 (UTC)开始到当前时间系统所偏移的毫秒数
 * 
 * 获取从Epoch, 1970-01-01 00:00:00 +0000 (UTC)开始到当前时间系统所偏移的毫秒数
 *
 * @return: 从Epoch, 1970-01-01 00:00:00 +0000 (UTC)开始到当前时间系统所偏移的毫秒数
 */
uint64 GetCurrentSysTimeOffset(void)
{
#if VA_VC
    SYSTEMTIME sys_time, base_sys_time;
    FILETIME file_time, base_file_time; /* FILETIME以100纳秒为单位 */
    ULARGE_INTEGER uli, base_uli; 
#elif VA_GNUC
    struct timeval cur_time;
#endif
    uint64 milli_second;

#if VA_VC
    GetSystemTime(&sys_time);
	/* 获取当前系统时间的秒数 */
    SystemTimeToFileTime(&sys_time, &file_time);
    uli.LowPart = file_time.dwLowDateTime;
    uli.HighPart = file_time.dwHighDateTime;
	/* 获取基准系统时间(1970-01-01 00:00:00)的秒数 */
	memset(&base_sys_time, 0, sizeof(base_sys_time));
	base_sys_time.wYear = 1970;
	base_sys_time.wMonth = 1;
	base_sys_time.wDay = 1;
	base_sys_time.wDayOfWeek = 5;
	SystemTimeToFileTime(&base_sys_time, &base_file_time);
	base_uli.LowPart = base_file_time.dwLowDateTime;
	base_uli.HighPart = base_file_time.dwHighDateTime;
	/* 获取当前系统时间所偏移的毫秒数 */
    milli_second = (uli.QuadPart / 10000 - base_uli.QuadPart / 10000) + sys_time.wMilliseconds;
#elif VA_GNUC
    gettimeofday(&cur_time, NULL);
    milli_second = cur_time.tv_sec * 1000 + cur_time.tv_usec / 1000;
#endif

    return milli_second;
}

/*
 * GetDiffTime - 获取两次系统时间差
 * 
 * @start: 开始时间，时间单位需与end一致
 * @end: 结束时间，时间单位需与start一致
 *
 * 获取两次系统时间差，如GetCurrentSysTimeOffset()返回的系统时间
 *
 * @return: 两次系统时间差
 */
uint64 GetDiffTime(uint64 start, uint64 end)
{
	return (start <= end) ? (end - start) : (start - end);
}

/*
 * SleepSecond - 执行挂起指定秒数的时间
 * 
 * @seconds: 指定秒数
 *
 * 执行挂起指定秒数的时间
 *
 * @return: void
 */
void SleepSecond(unsigned int seconds)
{
#if VA_VC
	Sleep(seconds * 1000);
#elif VA_GNUC
	sleep(seconds);
#endif

    return;
}

/*
 * SleepMilliSecond - 执行挂起指定毫秒数的时间
 * 
 * @milliseconds: 指定毫秒数，该值不应超过1000（即1秒）
 *
 * 执行挂起指定秒数的时间
 *
 * @return: void
 */
void SleepMilliSecond(unsigned int milliseconds)
{
#if VA_VC
	Sleep(milliseconds);
#elif VA_GNUC
	usleep(milliseconds * 1000);
#endif

    return;
}

} /* namespace VA */