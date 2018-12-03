#coding=utf-8

'''
适用套件1.x版本
'''

import re
import datetime
import sys

target_file = 'sr.log'

time_EndAudioData = None
time_SpeechEnd = None
time_MsgResult = None
time_NLU_In = None
time_NLU_Out = None

sum_time_SR = []
sum_time_NLU = []

ret_file = 'ret.csv'
ret_fp = open(ret_file, 'w')
ret_fp.write('num,time_SR,time_NLU\n')
# 当前结果序号
num = 0

test_text = '[2015-01-01 21:51:13.976000][CIssSr]ISS_SR_MSG_Result'
time_pattern = '.*\[(.*?)\]\[.*\].*'

print re.match(time_pattern, test_text).group(1)

def init_msg():
    global time_EndAudioData
    global time_SpeechEnd
    global time_MsgResult
    global time_NLU_In
    global time_NLU_Out

    time_EndAudioData = None
    time_SpeechEnd = None
    time_MsgResult = None
    time_NLU_In = None
    time_NLU_Out = None

def get_time_from_str(time_str):
    return datetime.datetime.strptime(time_str, '%Y-%m-%d %H:%M:%S.%f')

'''
计算时间差。

计算公式为time2-time1。
'''
def get_time_diff(time1, time2):
    ret = (time2 - time1).seconds*1000 + (time2 - time1).microseconds/1000
    return ret

with open(target_file) as fp:
    for line in fp:
        if 'Start Called' in line:
            # 进入一次识别
            num+=1
            init_msg()
        elif 'EndAudioData Called' in line:
            time_str = re.match(time_pattern, line).group(1)
            time_EndAudioData = get_time_from_str(time_str)
        elif 'OnSpeechEnd' in line:
            time_str = re.match(time_pattern, line).group(1)
            time_SpeechEnd = get_time_from_str(time_str)
        elif 'ivMsg_Result.' in line:
            time_str = re.match(time_pattern, line).group(1)
            time_MsgResult = get_time_from_str(time_str)
        elif 'Before NLIInterpret.' in line:
            time_str = re.match(time_pattern, line).group(1)
            time_NLU_In = get_time_from_str(time_str)
        elif 'NLIInterpret ret' in line:
            time_str = re.match(time_pattern, line).group(1)
            time_NLU_Out = get_time_from_str(time_str)
        elif 'ISS_SR_MSG_Result' in line:
            # 一次识别结束
            print 'endAudioData:'+str(time_EndAudioData), 'speechEnd:'+str(time_SpeechEnd), 'result:'+str(time_MsgResult)
            time_SR = get_time_diff(min(time_EndAudioData, time_SpeechEnd) if time_EndAudioData!=None and time_SpeechEnd!=None else time_EndAudioData if time_EndAudioData!=None else time_SpeechEnd, time_MsgResult)
            sum_time_SR.append(time_SR)

            time_NLU = get_time_diff(time_NLU_In, time_NLU_Out)
            sum_time_NLU.append(time_NLU)

            ret_fp.write(str(num)+','+str(time_SR)+','+str(time_NLU)+'\n')

ret_fp.write('\navg,'+str(sum(sum_time_SR)/len(sum_time_SR))+','+str(sum(sum_time_NLU)/len(sum_time_NLU))+'\n')
ret_fp.close()
