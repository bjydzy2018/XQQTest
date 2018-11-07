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
 * GetCurrentSysTime - ��ȡ��ǰϵͳʱ��(UTC)
 *
 * ��ȡ��ǰϵͳʱ�䣬��UTCʱ��
 * ��ʽ���������ʱ�䣬�����ʽΪ"YYYY-MM-DD WWW hh:mm:ss"
 *
 * @return: std::string
 */
std::string GetCurrentSysTime(void);

/*
 * GetCurrentLocalTime - ��ȡ��ǰ����ʱ��(��ǰ����ʱ��)
 *
 * ��ȡ��ǰ����ʱ�䣬��ϵͳ��ǰ����ʱ����ʱ��
 * ��ʽ���������ʱ�䣬�����ʽΪ"YYYY-MM-DD WWW hh:mm:ss"
 *
 * @return: std::string
 */
std::string GetCurrentLocalTime(void);

/*
 * GetCurrentTickCount - ��ȡ�Ӳ���ϵͳ��������ǰ�������ĺ�����
 *
 * ��ȡ�Ӳ���ϵͳ��������ǰ�������ĺ�����
 * ���Դ洢�����ֵ��(2 ^ 32 - 1)ms��ԼΪ49.71�죬�����ϵͳ����ʱ�䳬��49.71��ʱ��������ͻ��0
 *
 * @return: ����ʵ�ʵĺ�����
 */
unsigned long GetCurrentTickCount(void);

/*
 * GetCurrentSysTimeOffset - ��ȡ��Epoch, 1970-01-01 00:00:00 +0000 (UTC)��ʼ����ǰʱ��ϵͳ��ƫ�Ƶĺ�����
 * 
 * ��ȡ��Epoch, 1970-01-01 00:00:00 +0000 (UTC)��ʼ����ǰʱ��ϵͳ��ƫ�Ƶĺ�����
 *
 * @return: ��Epoch, 1970-01-01 00:00:00 +0000 (UTC)��ʼ����ǰʱ��ϵͳ��ƫ�Ƶĺ�����
 */
uint64 GetCurrentSysTimeOffset(void);

/*
 * GetDiffTime - ��ȡ����ϵͳʱ���
 * 
 * @start: ��ʼʱ�䣬ʱ�䵥λ����endһ��
 * @end: ����ʱ�䣬ʱ�䵥λ����startһ��
 *
 * ��ȡ����ϵͳʱ����GetCurrentSysTime()���ص�ϵͳʱ��
 *
 * @return: ����ϵͳʱ���
 */
uint64 GetDiffTime(uint64 start, uint64 end);

/*
 * SleepSecond - ִ�й���ָ��������ʱ��
 * 
 * @seconds: ָ������
 *
 * ִ�й���ָ��������ʱ��
 *
 * @return: void
 */
void SleepSecond(unsigned int seconds);

/*
 * SleepMilliSecond - ִ�й���ָ����������ʱ��
 * 
 * @milliseconds: ָ������������ֵ��Ӧ����1000����1�룩
 *
 * ִ�й���ָ��������ʱ��
 *
 * @return: void
 */
void SleepMilliSecond(unsigned int milliseconds);

} /* namespace VA */

#endif /* __VATIME_H__ */