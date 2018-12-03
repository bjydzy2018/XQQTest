#coding=utf-8

'''
使用python3运行
'''

import re
import pprint
import xlrd
import xlwt3

file_obj = 'ret.csv'
file_obj_iss = '1470374714710.csv'

ret_dict = {}
ret_dict_iss = {}

ret_result = 0
ret_vad_in = 0
ret_vad_out = 0
ret_confidence = 0
ret_ret_time = 0

with open(file_obj) as fp:
    for line in fp:
        line = line.strip()

        if line.startswith('pcm'):
            # 第一行表头
            continue
        else:
            content = line.split(',')

            pcm = content[0]
            ret_dict[pcm] = {}
            ret_dict[pcm]['nConfidenceScore']=content[1]
            ret_dict[pcm]['result']=content[2].replace('。','')
            ret_dict[pcm]['SpeechStartFrame']=content[3]
            ret_dict[pcm]['SpeechEndFrame']=content[4]
            ret_dict[pcm]['ResultTime']=content[5]

with open(file_obj_iss) as fp:
    for line in fp:
        line = line.strip()

        if line.startswith('录音'):
            # 第一行表头
            continue
        else:
            content = line.split(',')

            pcm = content[0]
            ret_dict_iss[pcm] = {}
            ret_dict_iss[pcm]['nConfidenceScore']=content[10] if content[10]!='' else ' '
            ret_dict_iss[pcm]['result']=content[9].replace('。','') if content[9]!='' else ' '
            ret_dict_iss[pcm]['SpeechStartFrame']=content[11]
            ret_dict_iss[pcm]['SpeechEndFrame']=content[12]
            ret_dict_iss[pcm]['ResultTime']=content[6]


wbk = xlwt3.Workbook()

sheet_nConfidenceScore = wbk.add_sheet('nConfidenceScore')
sheet_nConfidenceScore.write(0, 0, 'pcm')
sheet_nConfidenceScore.write(0, 1, '引擎')
sheet_nConfidenceScore.write(0, 2, '套件')

sheet_result = wbk.add_sheet('result')
sheet_result.write(0, 0, 'pcm')
sheet_result.write(0, 1, '引擎')
sheet_result.write(0, 2, '套件')

sheet_SpeechStartFrame = wbk.add_sheet('SpeechStartFrame')
sheet_SpeechStartFrame.write(0, 0, 'pcm')
sheet_SpeechStartFrame.write(0, 1, '引擎')
sheet_SpeechStartFrame.write(0, 2, '套件')

sheet_SpeechEndFrame = wbk.add_sheet('SpeechEndFrame')
sheet_SpeechEndFrame.write(0, 0, 'pcm')
sheet_SpeechEndFrame.write(0, 1, '引擎')
sheet_SpeechEndFrame.write(0, 2, '套件')

sheet_ResultTime = wbk.add_sheet('ResultTime')
sheet_ResultTime.write(0, 0, 'pcm')
sheet_ResultTime.write(0, 1, '引擎')
sheet_ResultTime.write(0, 2, '套件')

ptr_confidence = 1
ptr_result = 1
ptr_SpeechStartFrame = 1
ptr_SpeechEndFrame = 1
ptr_ResultTime = 1

vad_in_20 = 0
vad_in_40 = 0
vad_in_60 = 0

vad_out_20 = 0
vad_out_40 = 0
vad_out_60 = 0

for pcm in ret_dict:
    if ret_dict[pcm]['nConfidenceScore'] != ret_dict_iss[pcm]['nConfidenceScore']:
        if ret_dict[pcm]['nConfidenceScore'] != ' ' and  ret_dict_iss[pcm]['nConfidenceScore'] != ' ':
            if abs(int(ret_dict[pcm]['nConfidenceScore']) - int(ret_dict_iss[pcm]['nConfidenceScore'])) > 50:
                sheet_nConfidenceScore.write(ptr_confidence, 0, pcm)
                sheet_nConfidenceScore.write(ptr_confidence, 1, ret_dict[pcm]['nConfidenceScore'])
                sheet_nConfidenceScore.write(ptr_confidence, 2, ret_dict_iss[pcm]['nConfidenceScore'])
                ptr_confidence += 1
                ret_confidence += 1
        else:
            sheet_nConfidenceScore.write(ptr_confidence, 0, pcm)
            sheet_nConfidenceScore.write(ptr_confidence, 1, ret_dict[pcm]['nConfidenceScore'])
            sheet_nConfidenceScore.write(ptr_confidence, 2, ret_dict_iss[pcm]['nConfidenceScore'])
            ptr_confidence += 1
            ret_confidence += 1

    if ret_dict[pcm]['result'] != ret_dict_iss[pcm]['result']:
        sheet_result.write(ptr_result, 0, pcm)
        sheet_result.write(ptr_result, 1, ret_dict[pcm]['result'])
        sheet_result.write(ptr_result, 2, ret_dict_iss[pcm]['result'])
        ptr_result += 1
        ret_result += 1

    if ret_dict[pcm]['SpeechStartFrame'] != ret_dict_iss[pcm]['SpeechStartFrame']:
        sheet_SpeechStartFrame.write(ptr_SpeechStartFrame, 0, pcm)
        sheet_SpeechStartFrame.write(ptr_SpeechStartFrame, 1, ret_dict[pcm]['SpeechStartFrame'])
        sheet_SpeechStartFrame.write(ptr_SpeechStartFrame, 2, ret_dict_iss[pcm]['SpeechStartFrame'])
        ptr_SpeechStartFrame += 1
        ret_vad_in += 1

    if ret_dict[pcm]['SpeechEndFrame'] != ret_dict_iss[pcm]['SpeechEndFrame']:
        sheet_SpeechEndFrame.write(ptr_SpeechEndFrame, 0, pcm)
        sheet_SpeechEndFrame.write(ptr_SpeechEndFrame, 1, ret_dict[pcm]['SpeechEndFrame'])
        sheet_SpeechEndFrame.write(ptr_SpeechEndFrame, 2, ret_dict_iss[pcm]['SpeechEndFrame'])
        ptr_SpeechEndFrame += 1
        ret_vad_out += 1

    if ret_dict[pcm]['ResultTime'] != ret_dict_iss[pcm]['ResultTime']:
        sheet_ResultTime.write(ptr_ResultTime, 0, pcm)
        sheet_ResultTime.write(ptr_ResultTime, 1, ret_dict[pcm]['ResultTime'])
        sheet_ResultTime.write(ptr_ResultTime, 2, ret_dict_iss[pcm]['ResultTime'])
        ptr_ResultTime += 1
        ret_ret_time += 1

sheet_nConfidenceScore.write(0, 3, '一致率')
sheet_nConfidenceScore.write(1, 3, (1000-ret_confidence)/1000)

sheet_result.write(0, 3, '一致率')
sheet_result.write(1, 3, (1000-ret_result)/1000)

sheet_SpeechStartFrame.write(0, 3, '一致率')
sheet_SpeechStartFrame.write(1, 3, (1000-ret_vad_in)/1000)

sheet_SpeechEndFrame.write(0, 3, '一致率')
sheet_SpeechEndFrame.write(1, 3, (1000-ret_vad_out)/1000)

sheet_ResultTime.write(0, 3, '一致率')
sheet_ResultTime.write(1, 3, (1000-ret_ret_time)/1000)

wbk.save('compare_ret.xls')

# print(vad_in_20/ret_vad_in, vad_in_40/ret_vad_in, vad_in_60/ret_vad_in)
# print(vad_out_20/ret_vad_out, vad_out_40/ret_vad_out, vad_out_60/ret_vad_out)