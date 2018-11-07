/*
 * VAMsgDefine.h
 *
 *  Created on: 2013年8月27日
 *      Author: pffang
 */

#ifndef VAMSGDEFINE_H_
#define VAMSGDEFINE_H_

#include "VACommon.h"

namespace VA {

enum va_msg_type {
  MSG_NULL = 0,
  MSG_TTS_Call_Start = 1000,
  MSG_TTS_Call_Cancel = 1001,
  MSG_TTS_Call_Exit = 1002,
  MSG_TTS_RET_Completed = 1010,
  MSG_SR_Call_Start = 1100,
  MSG_SR_Call_Cancel = 1101,
  MSG_SR_Call_CommitNetWork = 1102,
  MSG_SR_Call_EndAudioData = 1103,
  MSG_SR_Call_Exit = 1104,
  MSG_SR_RET_Volume = 1110,
  MSG_SR_RET_NoSpeech = 1111,
  MSG_SR_RET_SpeechEnd = 1112,
  MSG_SR_RET_ResponseTimeout = 1113,
  MSG_SR_RET_NoResult = 1114,
  MSG_SR_RET_Result = 1115,
  MSG_END = INT_MAX
};

/* 词典类型： 0：无效； 1：Music； 2：Contact。 */
enum sr_grammar_type {
  SR_GRAM_TYPE_NULL = 0,
  SR_GRAM_TYPE_MUSIC = 1,
  SR_GRAM_TYPE_CONTACT = 2,
  SR_GRAM_TYPE_END = INT_MAX
};

enum sr_result_type {
  SR_RESULT_TYPE_NULL = 0,
  SR_RESULT_TYPE_Radio_Open = 10,
  SR_RESULT_TYPE_Radio_FM = 11,
  SR_RESULT_TYPE_Radio_AM = 12,
  SR_RESULT_TYPE_Radio_Name = 13,
  SR_RESULT_TYPE_Call_Open = 20,
  SR_RESULT_TYPE_Call_Contact = 21,
  SR_RESULT_TYPE_Music_Open = 30,
  SR_RESULT_TYPE_Music_PlayCD = 31,
  SR_RESULT_TYPE_Music_Play = 32,
  SR_RESULT_TYPE_Music_Singer = 33,
  SR_RESULT_TYPE_Navi = 40,
  SR_RESULT_TYPE_Navi_Route = 41,
  SR_RESULT_TYPE_LBS_Query = 42,
  SR_RESULT_TYPE_Navi_Cancel = 43,
  SR_RESULT_TYPE_Select = 50,
  SR_RESULT_TYPE_SelectCancel = 51,
  SR_RESULT_TYPE_Confirm = 60,
  SR_RESULT_TYPE_Help = 70,
  SR_RESULT_TYPE_END = INT_MAX
};

struct va_msg {
  long type;  //消息类型
  long value;  //消息主要值
  long param1;  //额外值1
  long param2;  //额外值2
  long extradata;  //消息包含额外数据，0表示不含额外数据，非零用于区分额外数据
  long datasize;  //额外数据大小
  va_msg(long lType = 0, long lValue = 0, long lParam1 = 0, long lParam2 = 0,
         long lExtraData = 0, long lDataSize = 0) {
    type = lType;
    value = lValue;
    param1 = lParam1;
    param2 = lParam2;
    extradata = lExtraData;
    datasize = lDataSize;
  }
  va_msg(va_msg& s) {
    type = s.type;
    value = s.value;
    param1 = s.param1;
    param2 = s.param2;
    extradata = s.extradata;
    datasize = s.datasize;
  }
  const va_msg& operator=(const char* lpbuff) {
    type = *((long*) (lpbuff));
    value = *((long*) (lpbuff + 4));
    param1 = *((long*) (lpbuff + 8));
    param2 = *((long*) (lpbuff + 12));
    extradata = *((long*) (lpbuff + 16));
    datasize = *((long*) (lpbuff + 20));
    return *this;
  }
  const va_msg& operator=(va_msg& s) {
    type = s.type;
    value = s.value;
    param1 = s.param1;
    param2 = s.param2;
    extradata = s.extradata;
    datasize = s.datasize;
    return *this;
  }
};

}
#endif /* VAMSGDEFINE_H_ */
