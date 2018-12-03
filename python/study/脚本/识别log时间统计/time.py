#encoding:utf-8
'''
运行脚本时可跟一个待处理文件名作为参数。若不加参数则默认处理time.txt。
结果文件命名格式为【源文件名_ret.txt】

author:hcliu
'''

import re
import datetime
import sys

start_time = 0
endAudioData_time = 0
speechEnd_time = 0
EMNlu_in_time = 0
EMNlu_out_time = 0

time_last_line = 0

sr_times = []
nlu_times = []
sum_times = []


if len(sys.argv) > 1:
    src_file = sys.argv[1]
else:
    src_file = 'time.txt'

ret_fp = open(src_file.split('.')[0]+'_ret.txt', 'w')

def initTime():
    start_time = 0
    endAudioData_time = 0
    speechEnd_time = 0
    result_time = 0
    EMNlu_in_time = 0
    EMNlu_out_time = 0

'''
计算时间差。

计算公式为time2-time1。
'''
def get_time(time1, time2):
    ret = (time2 - time1).seconds*1000 + (time2 - time1).microseconds/1000
    return ret

with open(src_file) as fp:
    for line in fp:
        # 提取时间和标签
        time_str = re.match('\[(.*?)\].*', line).group(1)
        tag = re.match('\[.*\] (.*)', line).group(1)
        time = datetime.datetime.strptime('20'+time_str, '%Y/%m/%d-%H:%M:%S %f')
        #print time_str, tag

        if tag.startswith('Start Called'):
            # 一次识别结束
            if time_last_line != 0:
                sum_time = get_time(min(endAudioData_time if endAudioData_time!=0 else 999999,
                speechEnd_time if speechEnd_time!=0 else 999999), time_last_line)
                sum_times.append(sum_time);

                nlu_time = get_time(EMNlu_in_time, EMNlu_out_time)
                nlu_times.append(nlu_time)

                sr_time = get_time(min(endAudioData_time if endAudioData_time!=0 else 999999,
                speechEnd_time if speechEnd_time!=0 else 999999), result_time)
                sr_times.append(sr_time)

                print 'sr_time:', sr_time, 'nlu_time:', nlu_time, 'sum_time:',sum_time
                ret_fp.write('sr_time:' + str(sr_time) + ', nlu_time:' + str(nlu_time) + ', sum_time:'+str(sum_time) + '\n')
                initTime()

            start_time = time
        elif tag.startswith('EndAudioData Called'):
            endAudioData_time = time
        elif tag.startswith('ivMsg_SpeechEnd'):
            speechEnd_time = time 
        elif tag.startswith('ivMsg_Result'):
            result_time = time
        elif tag.startswith('EMNlu::parse In'):
            EMNlu_in_time = time
        elif tag.startswith('EMNlu::parse Out'):
            EMNlu_out_time = time

        time_last_line = time

ret_fp.write('\n\n')
ret_fp.write('avg sr_time:' + str(sum(sr_times)/len(sr_times)) + ', nlu_time:'+str(sum(nlu_times)/len(nlu_times)) + ', sum_time:' + str(sum(sum_times)/len(sum_times)))

ret_fp.close()
