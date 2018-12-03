#coding=utf-8

'''
提取引擎结果到ret.csv

使用前需先把引擎结果中的<s>、</s>、空行去掉。
'''

import re

file_obj = 'list.scp_poi.irf_Call_SendNum_Playlist_Album_Songs_Video_Radio_App_Other_Select_Confirm_Cancel_ContactsC_97735.txt'

file_ret = open('ret.csv','w')
file_ret.write('pcm,nConfidenceScore,result,SpeechStartFrame,SpeechEndFrame,ResultTime\n')

result_start = False
result = ''

with open(file_obj) as fp:
    for line in fp:
        line = line.strip()
        
        if line.startswith('"/sdcard/AitalkOneUse'):
            file_ret.write(re.match('"/sdcard/AitalkOneUse/pcm/wav-code/(.*)"',line).group(1)+',')
        elif line.startswith('nConfidenceScore'):
            result_start = True
            file_ret.write(re.match('nConfidenceScore:(.*)',line).group(1)+',')
            continue
        elif line.startswith('SpeechStartFrame'):
            if result_start:
                file_ret.write(result.decode('utf-8').encode('gbk')+',')
                result=''
                result_start = False
            else:
                file_ret.write(' , ,')

            # 引擎以帧为单位，一帧是10ms
            file_ret.write(re.match('SpeechStartFrame:(.*)',line).group(1)+'0,' if re.match('SpeechStartFrame:(.*)',line).group(1)!='0' else '0,')
        elif line.startswith('SpeechEndFrame'):
            file_ret.write(re.match('SpeechEndFrame:(.*)',line).group(1)+'0,' if re.match('SpeechEndFrame:(.*)',line).group(1)!='0' else '0,')
        elif line.startswith('ResultTime'):
            file_ret.write(re.match('ResultTime:(.*)\\..*',line).group(1)+'\n')

        if result_start:
            result+=line

file_ret.close()