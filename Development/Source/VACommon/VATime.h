/*
 * VATime.h
 *
 */

#ifndef __VATIME_H__
#define __VATIME_H__

#include "VAPlatform.h"
#if VA_VC
#include <BaseTsd.h>
#elif VA_GNUC
#include <stdint.h>
#endif


#include <string>

namespace VA {

#if VA_VC
typedef UINT64 uint64;
#elif VA_GNUC
typedef uint64_t uint64;
#endif

/*
 * GetCurrentSysTime - 获取当前系统时间(UTC)
 *
 * 获取当前系统时间，即UTC时间
 * 格式化输出本地时间，输出格式为"YYYY-MM-DD WWW hh:mm:ss"
 *
 * @return: std::string
 */
std::string GetCurrentSysTime(void);

/*
 * GetCurrentLocalTime - 获取当前本地时间(当前所属时区)
 *
 * 获取当前本地时间，即系统当前所属时区的时间
 * 格式化输出本地时间，输出格式为"YYYY-MM-DD WWW hh:mm:ss"
 *
 * @return: std::string
 */
std::string GetCurrentLocalTime(void);

/*
 * GetCurrentTickCount - 获取从操作系统启动到当前所经过的毫秒数
 *
 * 获取从操作系统启动到当前所经过的毫秒数
 * 可以存储的最大值是(2 ^ 32 - 1)ms，约为49.71天，因此若系统运行时间超过49.71天时，这个数就会归0
 *
 * @return: 返回实际的毫秒数
 */
unsigned long GetCurrentTickCount(void);

/*
 * GetCurrentSysTimeOffset - 获取从Epoch, 1970-01-01 00:00:00 +0000 (UTC)开始到当前时间系统所偏移的毫秒数
 * 
 * 获取从Epoch, 1970-01-01 00:00:00 +0000 (UTC)开始到当前时间系统所偏移的毫秒数
 *
 * @return: 从Epoch, 1970-01-01 00:00:00 +0000 (UTC)开始到当前时间系统所偏移的毫秒数
 */
uint64 GetCurrentSysTimeOffset(void);

/*
 * GetDiffTime - 获取两次系统时间差
 * 
 * @start: 开始时间，时间单位需与end一致
 * @end: 结束时间，时间单位需与start一致
 *
 * 获取两次系统时间差，如GetCurrentSysTime()返回的系统时间
 *
 * @return: 两次系统时间差
 */
uint64 GetDiffTime(uint64 start, uint64 end);

/*
 * SleepSecond - 执行挂起指定秒数的时间
 * 
 * @seconds: 指定秒数
 *
 * 执行挂起指定秒数的时间
 *
 * @return: void
 */
void SleepSecond(unsigned int seconds);

/*
 * SleepMilliSecond - 执行挂起指定毫秒数的时间
 * 
 * @milliseconds: 指定毫秒数，该值不应超过1000（即1秒）
 *
 * 执行挂起指定秒数的时间
 *
 * @return: void
 */
void SleepMilliSecond(unsigned int milliseconds);

} /* namespace VA */

#endif /* __VATIME_H__ */