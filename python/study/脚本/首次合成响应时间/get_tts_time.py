#encoding: utf-8

'''
首次合成响应时间的一致性对比。首次合成响应时间，含义为一次合成过程中，第一次“句子开始合成的时间到首次进入音频输出回调的时间”之间的时间差。
'''

import re
import datetime

test_text = '[16/07/13-15:05:40 139][DBG][Px1f8b][Tx4152dd78] prepare to synth text'

tag_pattern = '.*] (.*)'
time_pattern = '\[(.*?)\].*'

def get_time_from_str(time_str):
    return datetime.datetime.strptime('20'+time_str, '%Y/%m/%d-%H:%M:%S %f')

'''
计算时间差。

计算公式为time2-time1。
'''
def get_time_diff(time1, time2):
    ret = (time2 - time1).seconds*1000 + (time2 - time1).microseconds/1000
    return ret

start_time = None
end_time = None

fp_ret = open('tts_time_ret.txt','w')

with open('isstts.log') as fp:
    for line in fp:
        try:
            tag = re.match(tag_pattern, line).group(1)
            time_str = re.match(time_pattern, line).group(1)
        except Exception , e:
            continue
        # print line

        if tag == 'prepare to synth text':
            start_time = get_time_from_str(time_str)

        if tag == 'tts first output':
            end_time = get_time_from_str(time_str)
            time_diff = get_time_diff(start_time, end_time)
            fp_ret.write(str(time_diff)+'\n')
        
fp_ret.close()





