# coding=utf-8
# python3

import sys
import re
import time
import csv
import codecs

# 默认参数，当没有命令行传参时 使用这些参数
strTestIssAIUILogFileName = 'TestISS-AIUI.log'
strVadInfoFileName = 'vad前后端点.txt'
strTestIssAIUILogEffOutputFileName = 'TestISS-AIUI-eff.csv'

# 上一次的音频结果 给了下一次时，配置这个时间间隔
configure_callback_msg_should_not_be_late_then_audio_path_time = 10

# 读取命令行参数
if(len(sys.argv) != 1 and len(sys.argv) != 4):
    print('Invalid Arg! len(sys.argv)=%d' % (len(sys.argv)))
    print('Uasge: %s <strTestIssAIUILogFileName.log> <strVadInfoFileName.txt> <strTestIssAIUILogEffOutputFileName.csv>'
          % (sys.argv[0]))
    sys.exit(1)
if(len(sys.argv) == 4):
    strTestIssAIUILogFileName = sys.argv[1]
    strVadInfoFileName = sys.argv[2]
    strTestIssAIUILogEffOutputFileName = sys.argv[3]

def handle_encode_error_replace_cb(e):
    return(" ", e.end)

codecs.register_error("replace", handle_encode_error_replace_cb)

# 读取工具的日志文件
listTestISSAIULogLines = []
try:
    fhTestISSAIUILog = open(strTestIssAIUILogFileName, mode='r', encoding='utf-8', errors="replace")
    for line in fhTestISSAIUILog:
        listTestISSAIULogLines.append(line.strip())
except Exception as e:
    print('File Error:' + str(e))
    sys.exit(1)

# 读取如子的VAD的日志文件
listVadInfo = []
dictVadInfo = {}
try:
    fhVadInfo = open(strVadInfoFileName, mode='r', encoding='utf-8')
    for line in fhVadInfo:
        listVadInfo.append(line.strip())
except Exception as e:
    print('File Error:' + str(e))
    sys.exit(1)
# 处理如子的VAD的日志文件
for line in listVadInfo:
    matchObjAudioPath = re.search(
        '[0-9]+, +[0-9]+, +[0-9]+, +[0-9]+, +[0-9]+, +[0-9]+, +([0-9]+), +[0-9]+, +[0-9]+, +[0-9]+, +[0-9]+, +[0-9]+, +[0-9]+, +[0-9]+\.[0-9]+, .*?([^\\\/]+\.)(wav|pcm)', line)
    if(matchObjAudioPath):
        dictVadInfo[matchObjAudioPath.group(2)] = int(matchObjAudioPath.group(1)) * 10


# 解析工具的日志数据
listRecords_strAudioPath_nAppenAudioTime_nAudioVadEndPoint_nMsgCallBackTime = []
dictSomeRecord = {}
listAudioPath = []
dictSomeRecordLastBackup = {}
nAudioPathTime = 0
nAppenAudioTime = 0
nMsgCallBackTime = 0
for line in listTestISSAIULogLines:
    # 匹配音频名
    matchObjAudioPath = re.search(
        '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*current test num: [\d]+, audio_path: (.*)', line)
    if(matchObjAudioPath):
        strTime = matchObjAudioPath.group(1)
        nMsec = int(matchObjAudioPath.group(2))
        nAudioPathTime = time.mktime(time.strptime(strTime, '%y/%m/%d-%H:%M:%S')) * 1000 + nMsec
        dictSomeRecordLastBackup = dictSomeRecord.copy()
        dictSomeRecord.clear()
        dictSomeRecord['strAudioPath'] = matchObjAudioPath.group(3)
        listAudioPath.append(matchObjAudioPath.group(3))
        continue
    # 匹配开始送入录音的时间
    matchObjAppenAudioTime = re.search(
        '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*append audio data to sr', line)
    if(matchObjAppenAudioTime):
        strTime = matchObjAppenAudioTime.group(1)
        nMsec = int(matchObjAppenAudioTime.group(2))
        nAppenAudioTime = time.mktime(time.strptime(strTime, '%y/%m/%d-%H:%M:%S')) * 1000 + nMsec
        dictSomeRecord['nAppenAudioTime'] = nAppenAudioTime
        continue
    # 匹配录音后端点的时间标注
    matchObjAudioVadEndPoint = re.search('get audio vad end point: ([\d]+)\.', line)
    if(matchObjAudioVadEndPoint):
        nAudioVadEndPoint = int(matchObjAudioVadEndPoint.group(1))
        dictSomeRecord['nAudioVadEndPoint'] = nAudioVadEndPoint
        continue
    # 匹配给出结果的时间
    matchObjMsgCallBackTime = re.search(
        '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*get callback msg: 20009_ISS_SR_MSG_Result', line)
    if(matchObjMsgCallBackTime):
        strTime = matchObjMsgCallBackTime.group(1)
        nMsec = int(matchObjMsgCallBackTime.group(2))
        nMsgCallBackTime = time.mktime(time.strptime(strTime, '%y/%m/%d-%H:%M:%S')) * 1000 + nMsec
        if(nMsgCallBackTime - nAudioPathTime <= configure_callback_msg_should_not_be_late_then_audio_path_time):
            # 上一次的音频结果 给了下一次
            dictSomeRecordLastBackup['nMsgCallBackTime'] = nMsgCallBackTime
            if(not('strAudioPath' in dictSomeRecordLastBackup)
               or not('nAppenAudioTime' in dictSomeRecordLastBackup)
               or not('nAudioVadEndPoint' in dictSomeRecordLastBackup)
               or not('nMsgCallBackTime' in dictSomeRecordLastBackup)):
                print('file %s format error!!' % (strTestIssAIUILogFileName))
                print(dictSomeRecordLastBackup)
                sys.exit(2)
            print('attention found a callback msg that gives slow, strAudioPath: %s' % (dictSomeRecordLastBackup['strAudioPath']))
            listRecords_strAudioPath_nAppenAudioTime_nAudioVadEndPoint_nMsgCallBackTime.append(dictSomeRecordLastBackup.copy())
        else:
            dictSomeRecord['nMsgCallBackTime'] = nMsgCallBackTime
            if(not('strAudioPath' in dictSomeRecord)
               or not('nAppenAudioTime' in dictSomeRecord)
               or not('nAudioVadEndPoint' in dictSomeRecord)
               or not('nMsgCallBackTime' in dictSomeRecord)):
                print('file %s format error!!' % (strTestIssAIUILogFileName))
                print(dictSomeRecord)
                sys.exit(2)
            listRecords_strAudioPath_nAppenAudioTime_nAudioVadEndPoint_nMsgCallBackTime.append(dictSomeRecord.copy())
            dictSomeRecord.clear()
        continue
# 这里打印一下工具中抓取到的有语义结果的记录数目
print('%s Record num: %d.'
      % (strTestIssAIUILogFileName, len(listAudioPath)))


# 输出报表 到csv格式的文件中
with open(strTestIssAIUILogEffOutputFileName, 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile)
    spamwriter.writerow(
        ['strAudioPath', 'nAppenAudioTime(UTC ms)', 'nAudioVadEndPoint(ms)', 'nMsgCallBackTime(UTC ms)', 'nFinalResultTime(ms)'])
    for line in listAudioPath:
        strAudioPath = line
        nAppenAudioTime = ''
        nAudioVadEndPoint = ''
        nMsgCallBackTime = ''
        nFinalResultTime = ''
        for line2 in listRecords_strAudioPath_nAppenAudioTime_nAudioVadEndPoint_nMsgCallBackTime:
            if(line2['strAudioPath'] == strAudioPath):
                nAppenAudioTime = line2['nAppenAudioTime']
                nMsgCallBackTime = line2['nMsgCallBackTime']
                matchObjAudioPath = re.search('.*?([^\\\/]+\.)(wav|pcm)', strAudioPath)
                if(matchObjAudioPath):
                    path = matchObjAudioPath.group(1)
                    if(path in dictVadInfo):
                        nAudioVadEndPoint = dictVadInfo[path]
                    else:
                        print('not found vad end: %s!!' % (path))
                        sys.exit(2)
                else:
                    print('pcm path: %s format error!!' % (strAudioPath))
                    sys.exit(2)
                break

        if(nMsgCallBackTime != ''
           and nAppenAudioTime != ''
           and nAudioVadEndPoint != ''
           and (int(nMsgCallBackTime) - int(nAppenAudioTime) - int(nAudioVadEndPoint)) > 0):
            nFinalResultTime = int(nMsgCallBackTime) - int(nAppenAudioTime) - int(nAudioVadEndPoint)
        spamwriter.writerow([strAudioPath, nAppenAudioTime, nAudioVadEndPoint, nMsgCallBackTime, nFinalResultTime])
    print('success :) output filename=%s' % (strTestIssAIUILogEffOutputFileName))


