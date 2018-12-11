# coding=utf-8
# python3

import os
import sys
import re
import time
import json
import csv
from functools import cmp_to_key


# 默认参数，当没有命令行传参时 使用这些参数
strTestISSAIUILogDir = './mix/'
strLocalCloudAndFinalResultOutPutFileName = './LocalCloudAndFinalResult.csv'

# 上一次的音频结果 给了下一次时，配置这个时间间隔
configure_callback_msg_should_not_be_late_then_audio_path_time = 1000

# 读取命令行参数
if(len(sys.argv) != 1 and len(sys.argv) != 3):
    print('Invalid Arg! len(sys.argv)=%d' % (len(sys.argv)))
    print('Uasge: %s <strTestISSAIUILogDir> <strLocalCloudAndFinalResultOutPutFileName>' % (sys.argv[0]))
    sys.exit(1)
if(len(sys.argv) == 3):
    strTestISSAIUILogDir = sys.argv[1]
    strLocalCloudAndFinalResultOutPutFileName = sys.argv[2]

# 筛选所有TestISS-AIUI_xxx.log日志文件
listTestISSAIUILogDirFiles = os.listdir(strTestISSAIUILogDir)
listTestISSAIUILogDirLogFileNames = []
for file in listTestISSAIUILogDirFiles:
    if not os.path.isdir(file):
        if(re.search('TestISS-AIUI(_[\d]+-[\d]+-[\d]+_[\d]+-[\d]+-[\d]+){0,1}.log', file)):
            listTestISSAIUILogDirLogFileNames.append(file)
# 对TestISS-AIUI_xxx.log日志文件按照日期排序
def comp_by_utc(x, y):
    matchObjX = re.search('TestISS-AIUI_([\d]+-[\d]+-[\d]+_[\d]+-[\d]+-[\d]+).log', x)
    matchObjY = re.search('TestISS-AIUI_([\d]+-[\d]+-[\d]+_[\d]+-[\d]+-[\d]+).log', y)
    if(matchObjX and matchObjY):
        nTimeX = time.mktime(time.strptime(matchObjX.group(1), '%Y-%m-%d_%H-%M-%S'))
        nTimeY = time.mktime(time.strptime(matchObjY.group(1), '%Y-%m-%d_%H-%M-%S'))
        if(nTimeX < nTimeY) :
            return -1
        elif(nTimeX > nTimeY) :
            return 1
        else:
            return 0
    else:
        if(not matchObjX and not matchObjY):
            print('Error: Find two TestISS-AIUI.log: %s, %s.' % (x, y))
            sys.exit(1)
        if(not matchObjX):
            return 1
        else:
            return -1
listTestISSAIUILogDirLogFileNames.sort(key=cmp_to_key(lambda a, b: comp_by_utc(a, b)))
# 逐个处理每一个日志文件
listLog = []
for line in listTestISSAIUILogDirLogFileNames:
    log_file_name = strTestISSAIUILogDir + '/' + line
    log_file_name = log_file_name.replace('\\', '/')
    log_file_name = log_file_name.replace('//', '/')
    print('Handling TestISS-AIUI filenames: %s' % (log_file_name))
    try:
        fhLog = open(log_file_name, mode='r', encoding='utf-8')
        for file_line in fhLog: listLog.append(file_line)
    except Exception as e:
        try:
            fhLog = open(log_file_name, mode='r', encoding='gbk')
            for file_line in fhLog: listLog.append(file_line)
        except Exception as e:
            print('File Error:' + str(e))
            sys.exit(1)

# 解析日志文件
listRecords = []
dictSomeRecord = {}
nAudioPathTime = 0
nMsgCallBackTime = 0
for line in listLog:
    # 匹配录音文件名
    matchObjAudioPath = re.search(
        '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*current test num: [\d]+, audio_path: (.*)', line)
    if(matchObjAudioPath):
        strTime = matchObjAudioPath.group(1)
        nMsec = int(matchObjAudioPath.group(2))
        nAudioPathTime = time.mktime(time.strptime(strTime, '%y/%m/%d-%H:%M:%S')) * 1000 + nMsec
        if('strAudioPath' in dictSomeRecord):
            listRecords.append(dictSomeRecord.copy())
        dictSomeRecord.clear()
        dictSomeRecord['strAudioPath'] = matchObjAudioPath.group(3)
        continue
    # 匹配最终结果
    matchObjMsgCallBackTime = re.search(
        '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*get callback msg: 20009_ISS_SR_MSG_Result.*, lParam: (.*)$', line)
    if(matchObjMsgCallBackTime):
        strTime = matchObjMsgCallBackTime.group(1)
        nMsec = int(matchObjMsgCallBackTime.group(2))
        nMsgCallBackTime = time.mktime(time.strptime(strTime, '%y/%m/%d-%H:%M:%S')) * 1000 + nMsec
        if(nMsgCallBackTime - nAudioPathTime <= configure_callback_msg_should_not_be_late_then_audio_path_time):
            # 上一次的音频结果 给了下一次
            dictSomeRecordLast = listRecords.pop()
            if('strISS_SR_MSG_Result_0' in dictSomeRecordLast): 
                dictSomeRecordLast['strISS_SR_MSG_Result_1'] = matchObjMsgCallBackTime.group(3)
                jsonObj = json.loads(dictSomeRecordLast['strISS_SR_MSG_Result_1'])
                if('text' in jsonObj['intent']):
                    dictSomeRecordLast['srResult_1'] = jsonObj['intent']['text']
            else:
                dictSomeRecordLast['strISS_SR_MSG_Result_0'] = matchObjMsgCallBackTime.group(3)
                jsonObj = json.loads(dictSomeRecordLast['strISS_SR_MSG_Result_0'])
                if('text' in jsonObj['intent']):
                    dictSomeRecordLast['srResult_0'] = jsonObj['intent']['text']
            listRecords.append(dictSomeRecordLast.copy())
            print('attention found a callback msg that gives slow, strAudioPath: %s'
                  % (dictSomeRecordLast['strAudioPath']))
        else:
            # 可能一个录音 给两个结果
            if('strISS_SR_MSG_Result_0' in dictSomeRecord): 
                dictSomeRecord['strISS_SR_MSG_Result_1'] = matchObjMsgCallBackTime.group(3)
                jsonObj = json.loads(dictSomeRecord['strISS_SR_MSG_Result_1'])
                if('text' in jsonObj['intent']):
                    dictSomeRecord['srResult_1'] = jsonObj['intent']['text']
            else:
                dictSomeRecord['strISS_SR_MSG_Result_0'] = matchObjMsgCallBackTime.group(3)
                jsonObj = json.loads(dictSomeRecord['strISS_SR_MSG_Result_0'])
                if('text' in jsonObj['intent']):
                    dictSomeRecord['srResult_0'] = jsonObj['intent']['text']
        continue
    # 匹配云端结果
    matchObjMsgCloudCallBackTime = re.search(
        '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*get callback msg: 20025_ISS_SR_MSG_CloudResult.*, lParam: (.*)$', line)
    if(matchObjMsgCloudCallBackTime):
        # 可能一个录音 给两个结果
        if('strISS_SR_MSG_CloudResult_0' in dictSomeRecord):
            dictSomeRecord['strISS_SR_MSG_CloudResult_1'] = matchObjMsgCloudCallBackTime.group(3)
        else:
            dictSomeRecord['strISS_SR_MSG_CloudResult_0'] = matchObjMsgCloudCallBackTime.group(3)
        continue
    # 匹配本地结果
    matchObjMsgLocalCallBackTime = re.search(
        '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*get callback msg: 20026_ISS_SR_MSG_LocalResult.*, lParam: (.*)$', line)
    if(matchObjMsgLocalCallBackTime):
        # 可能一个录音 给两个结果
        if('strISS_SR_MSG_LocalResult_0' in dictSomeRecord):
            dictSomeRecord['strISS_SR_MSG_LocalResult_1'] = matchObjMsgLocalCallBackTime.group(3)
        else:
            dictSomeRecord['strISS_SR_MSG_LocalResult_0'] = matchObjMsgLocalCallBackTime.group(3)
        continue
if('strAudioPath' in dictSomeRecord):
    listRecords.append(dictSomeRecord.copy())
    dictSomeRecord.clear()

# 输出识别结果的报表 到csv格式的文件中
print("Found audio_path num: %d" % (len(listRecords)))  # 这里输出一下录音文件个数
with open(strLocalCloudAndFinalResultOutPutFileName, 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile)
    spamwriter.writerow(['strAudioPath',
                         'srResult_0', 'srResult_1',
                         'strISS_SR_MSG_Result_0', 'bislocalresult_0', 'strISS_SR_MSG_Result_1', 'bislocalresult_1',
                         'strISS_SR_MSG_CloudResult_0', 'strISS_SR_MSG_CloudResult_1',
                         'strISS_SR_MSG_LocalResult_0', 'strISS_SR_MSG_LocalResult_1' ])
    for line in listRecords:
        strAudioPath = line['strAudioPath']
        srResult_0 = ''
        srResult_1 = ''
        strISS_SR_MSG_Result_0 = ''
        bislocalresult_0 = ''
        strISS_SR_MSG_Result_1 = ''
        bislocalresult_1 = ''
        strISS_SR_MSG_CloudResult_0 = ''
        strISS_SR_MSG_CloudResult_1 = ''
        strISS_SR_MSG_LocalResult_0 = ''
        strISS_SR_MSG_LocalResult_1 = ''
        if('srResult_0' in line): srResult_0 = line['srResult_0']
        if('srResult_1' in line): srResult_1 = line['srResult_1']
        if('strISS_SR_MSG_Result_0' in line):
            strISS_SR_MSG_Result_0 = line['strISS_SR_MSG_Result_0']
            if('bislocalresult' in strISS_SR_MSG_Result_0):
                bislocalresult_0 = 1
            else:
                bislocalresult_0 = 0
        if('strISS_SR_MSG_Result_1' in line):
            strISS_SR_MSG_Result_1 = line['strISS_SR_MSG_Result_1']
            if('bislocalresult' in strISS_SR_MSG_Result_1):
                bislocalresult_1 = 1
            else:
                bislocalresult_1 = 0
        if('strISS_SR_MSG_CloudResult_0' in line): strISS_SR_MSG_CloudResult_0 = line['strISS_SR_MSG_CloudResult_0']
        if('strISS_SR_MSG_CloudResult_1' in line): strISS_SR_MSG_CloudResult_1 = line['strISS_SR_MSG_CloudResult_1']
        if('strISS_SR_MSG_LocalResult_0' in line): strISS_SR_MSG_LocalResult_0 = line['strISS_SR_MSG_LocalResult_0']
        if('strISS_SR_MSG_LocalResult_1' in line): strISS_SR_MSG_LocalResult_1 = line['strISS_SR_MSG_LocalResult_1']
        spamwriter.writerow([strAudioPath,
                             srResult_0, srResult_1,
                             strISS_SR_MSG_Result_0, bislocalresult_0, strISS_SR_MSG_Result_1, bislocalresult_1,
                             strISS_SR_MSG_CloudResult_0, strISS_SR_MSG_CloudResult_1,
                             strISS_SR_MSG_LocalResult_0, strISS_SR_MSG_LocalResult_1])
    print('Write result to file: %s.' % (strLocalCloudAndFinalResultOutPutFileName))
