#coding=utf-8

'''
适用套件3.0版本
'''

import re
import datetime
import sys

target_file = 'isssr.log'

time_EndAudioData = None
time_SpeechEnd = None
time_MsgResult = None
time_NLU_In = None
time_NLU_Out = None
time_Contact_Name = None
time_CATA_Search_Result = None

sum_time_SR = []
sum_time_NLU = []
sum_time_CATA = []

ret_file = 'ret.csv'
ret_fp = open(ret_file, 'w')
ret_fp.write('num,time_SR,time_NLU,time_CATA\n')
# 当前结果序号
num = 0
# 有cata结果的数量
cata_num = 0

test_text = '[16/08/01-17:25:59 547][DBG][Px0fff][Tx4002db00] EndAudioData Called.m_iMode=1'
time_pattern = '\[(.*?)\].*'

# print re.match(time_pattern, test_text).group(1)

def init_msg():
    global time_EndAudioData
    global time_SpeechEnd
    global time_MsgResult
    global time_NLU_In
    global time_NLU_Out
    global time_Contact_Name
    global time_CATA_Search_Result

    time_EndAudioData = None
    time_SpeechEnd = None
    time_MsgResult = None
    time_NLU_In = None
    time_NLU_Out = None
    time_Contact_Name = None
    time_CATA_Search_Result = None

def get_time_from_str(time_str):
    return datetime.datetime.strptime('20'+time_str, '%Y/%m/%d-%H:%M:%S %f')

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
        elif 'EndAudioData Called.m_iMode' in line:
            time_str = re.match(time_pattern, line).group(1)
            time_EndAudioData = get_time_from_str(time_str)
        elif 'ivMsg_SpeechEnd' in line:
            time_str = re.match(time_pattern, line).group(1)
            time_SpeechEnd = get_time_from_str(time_str)
        elif 'ivMsg_Result result num is' in line:
            time_str = re.match(time_pattern, line).group(1)
            time_MsgResult = get_time_from_str(time_str)
        elif 'EMNlu::parse In' in line:
            time_str = re.match(time_pattern, line).group(1)
            time_NLU_In = get_time_from_str(time_str)
        elif 'EMNlu::parse Out' in line:
            time_str = re.match(time_pattern, line).group(1)
            time_NLU_Out = get_time_from_str(time_str)
        elif 'Contact Name' in line:
            time_str = re.match(time_pattern, line).group(1)
            time_Contact_Name = get_time_from_str(time_str)
        elif 'CATA SEARCH RESULT' in line:
            time_str = re.match(time_pattern, line).group(1)
            time_CATA_Search_Result = get_time_from_str(time_str)
        elif 'ISS_SR_MSG_Result' in line:
            # 一次识别结束
            time_SR = get_time_diff(min(time_EndAudioData, time_SpeechEnd) if time_EndAudioData!=None and time_SpeechEnd!=None else time_EndAudioData if time_EndAudioData!=None else time_SpeechEnd, time_MsgResult)
            sum_time_SR.append(time_SR)

            time_NLU = get_time_diff(time_NLU_In, time_NLU_Out if time_Contact_Name==None else time_Contact_Name)
            sum_time_NLU.append(time_NLU)

            try:
                time_CATA = get_time_diff(time_Contact_Name, time_CATA_Search_Result) if time_Contact_Name!=None and time_CATA_Search_Result!=None else 0
            except Exception,e:
                print time_EndAudioData,time_Contact_Name, time_CATA_Search_Result
                sys.exit(0)
            sum_time_CATA.append(time_CATA)
            if time_Contact_Name and time_CATA_Search_Result:
                cata_num+=1

            ret_fp.write(str(num)+','+str(time_SR)+','+str(time_NLU)+','+str(time_CATA)+'\n')

ret_fp.write('\navg,'+str(sum(sum_time_SR)/len(sum_time_SR))+','+str(sum(sum_time_NLU)/len(sum_time_NLU))+','+str(sum(sum_time_CATA)/cata_num)+'\n')
ret_fp.close()
