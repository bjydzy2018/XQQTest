# coding=utf-8
# python3

import sys
import re
import csv
import time

# 默认参数，当没有命令行传参时 使用这些参数
strCloudSrResultYizhixingFileName = 'pp_rlt_yasuo.txt'
strCloudSrResultConsistenceFileName = '与云端识别比对一致性.csv'

# 读取命令行参数
if(len(sys.argv) != 1 and len(sys.argv) != 3):
    print('Invalid Arg! len(sys.argv)=%d' % (len(sys.argv)))
    print('Uasge: %s <strCloudSrResultYizhixingFileName.txt> <strCloudSrResultConsistenceFileName.csv>' % (sys.argv[0]))
    sys.exit(1)
if(len(sys.argv) == 3):
    strCloudSrResultYizhixingFileName = sys.argv[1]
    strCloudSrResultConsistenceFileName = sys.argv[2]

# 读取识别的日志文件
listCloudSrResultYizhixing = []
try:
    fp = open(strCloudSrResultYizhixingFileName, mode='r', encoding='utf-8')
    for line in fp: listCloudSrResultYizhixing.append(line.strip())
except Exception as e:
    try:
        fp = open(strCloudSrResultYizhixingFileName, mode='r', encoding='gbk')
        for line in fp: listCloudSrResultYizhixing.append(line.strip())
    except Exception as e:
        print('File Error:' + str(e))
        sys.exit(1)

listOut = []
sr_result_flag = 0
strAudioPath = ''
nAudioPathTime = 0
sr_result = ''
for line in listCloudSrResultYizhixing:
    # 匹配音频名
    matchObjAudioPath = re.search(
        'msc_log_([\d]{4}-[\d]{2}-[\d]{2})[a-zA-Z]+([\d]{2}-[\d]{2}-[\d]{2})\.([\d]{3})[\d]+\.(pcm|wav)', line)
    if(matchObjAudioPath):
        strAudioPath = line
        pcm_time = ("%s %s" % (matchObjAudioPath.group(1), matchObjAudioPath.group(2)))
        nMsec = int(matchObjAudioPath.group(3))
        nAudioPathTime = time.mktime(time.strptime(pcm_time, '%Y-%m-%d %H-%M-%S')) * 1000 + nMsec
        sr_result_flag = 1
        continue
    # 这句话结果结束
    if(line == ''):
        sr_result_flag = 0
        if(len(strAudioPath) > 0):
            listOut.append([strAudioPath, nAudioPathTime, sr_result])
            strAudioPath = ''
            nAudioPathTime = 0
            sr_result = ''
        else:
            print('Invalid Local Sr Result Format!')
            sys.exit(1) 
        continue
    # 拼接结果
    if(sr_result_flag == 1):
        if(line != '<s>' and line != '</s>' and line != '..'
           and line[0:7] != 'NetType' and line[0:16] != 'nConfidenceScore'
           and line[0:6] != 'PinYin'
           and line[0:3] != '==='
           and line[0:8] != 'Log End'):
            if(len(sr_result) == 0):
                sr_result = line
            else:
                sr_result = sr_result + line
if(len(strAudioPath) > 0):
    listOut.append([strAudioPath, nAudioPathTime, sr_result])
    strAudioPath = ''
    nAudioPathTime = 0
    sr_result = ''

# 输出报表 到csv格式的文件中
listOut.sort(key=lambda item : item[1])
with open(strCloudSrResultConsistenceFileName, 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile)
    spamwriter.writerow(['strAudioPath', 'pcm_time(UTC ms)', 'sr_result'])
    for some_row in listOut:
        spamwriter.writerow(some_row)
    print('success :) output filename=%s' % (strCloudSrResultConsistenceFileName))
