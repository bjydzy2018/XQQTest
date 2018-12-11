# coding=utf-8
# python3

import sys
import re
import time
import csv

# 默认参数，当没有命令行传参时 使用这些参数
szTestISSAIUILogFileName = 'TestISS-AIUI_android.log'
szIssSrLogFileName = 'isssr_android.log'
szOutputFileName = 'speed_result_on_android.csv'
szVadInfoFileName = 'vad前后端点.txt'

# 上一次的音频结果 给了下一次时，配置这个时间间隔
configure_callback_msg_should_not_be_late_then_audio_path_time = 1000

# 读取命令行参数
if(len(sys.argv) != 1 and len(sys.argv) != 5):
    print('Invalid Arg! len(sys.argv)=%d' % (len(sys.argv)))
    print('Uasge: %s <TestISS-AIUI.log> <isssr.log> <OutputFileName.cvs> <vad前后端点.txt>' % (sys.argv[0]))
    sys.exit(1)
if(len(sys.argv) == 5):
    szTestISSAIUILogFileName = sys.argv[1]
    szIssSrLogFileName = sys.argv[2]
    szOutputFileName = sys.argv[3]
    szVadInfoFileName = sys.argv[4]

# 读取工具的日志文件
listTestISSAIULogLines = []
try:
    fhTestISSAIUILog = open(szTestISSAIUILogFileName, mode='r', encoding='utf-8')
    for line in fhTestISSAIUILog: listTestISSAIULogLines.append(line.strip())
except Exception as e:
    try:
        fhTestISSAIUILog = open(szTestISSAIUILogFileName, mode='r', encoding='gbk')
        for line in fhTestISSAIUILog: listTestISSAIULogLines.append(line.strip())
    except Exception as e:    
        print('File Error:' + str(e))
        sys.exit(1)

# 读取AIUI套件的的日志文件
listIssSrLogLines = []
try:
    fhIssSrLog = open(szIssSrLogFileName, mode='r', encoding='utf-8')
    for line in fhIssSrLog: listIssSrLogLines.append(line.strip())
except Exception as e:
    try:
        fhIssSrLog = open(szIssSrLogFileName, mode='r', encoding='gbk')
        for line in fhIssSrLog: listIssSrLogLines.append(line.strip())
    except Exception as e:    
        print('File Error:' + str(e))
        sys.exit(1)


# 解析工具的日志数据
listRecords_strAudioPath_nAppenAudioTime_nAudioVadEndPoint_nMsgCallBackTime = []
dictSomeRecord = {}
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
                print('file %s format error111!!' % (szTestISSAIUILogFileName))
                print(dictSomeRecordLastBackup)
                print('strTime: %s %3d' % (strTime, nMsec))
                sys.exit(2)
                # continue
            print('attention found a callback msg that gives slow, strAudioPath: %s' % (dictSomeRecordLastBackup['strAudioPath']))
            listRecords_strAudioPath_nAppenAudioTime_nAudioVadEndPoint_nMsgCallBackTime.append(dictSomeRecordLastBackup.copy())
            dictSomeRecordLastBackup.clear()
        else:
            dictSomeRecord['nMsgCallBackTime'] = nMsgCallBackTime
            if(not('strAudioPath' in dictSomeRecord)
               or not('nAppenAudioTime' in dictSomeRecord)
               or not('nAudioVadEndPoint' in dictSomeRecord)
               or not('nMsgCallBackTime' in dictSomeRecord)):
                print('file %s format error222!!' % (szTestISSAIUILogFileName))
                print(dictSomeRecord)
                print('strTime: %s %3d' % (strTime, nMsec))
                sys.exit(2)
                # continue
            listRecords_strAudioPath_nAppenAudioTime_nAudioVadEndPoint_nMsgCallBackTime.append(dictSomeRecord.copy())
        continue

# 这里打印一下工具中抓取到的有语义结果的记录数目
print('%s Record num: %d.'
      % (szTestISSAIUILogFileName, len(listRecords_strAudioPath_nAppenAudioTime_nAudioVadEndPoint_nMsgCallBackTime)))


# 解析AIUI套件的日志数据
listRecords_nSrResultTime_nSearchProcessTime_nNLIInterpretTime = []
dictSomeRecord.clear()
for line in listIssSrLogLines:
    # 匹配识别时间
    matchObjSrResultTime = re.search(
        '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*Sr Result: .*m_nConfidenceScore', line)
    if(matchObjSrResultTime):
        strTime = matchObjSrResultTime.group(1)
        nMsec = int(matchObjSrResultTime.group(2))
        nSrResultTime = time.mktime(time.strptime(strTime, '%y/%m/%d-%H:%M:%S')) * 1000 + nMsec
        dictSomeRecord['nSrResultTime'] = nSrResultTime
        if('nSearchProcessTime' in dictSomeRecord): del dictSomeRecord['nSearchProcessTime']
        if('nNLIInterpretTime' in dictSomeRecord): del dictSomeRecord['nNLIInterpretTime']
        
    # 匹配cata搜索时间
    matchObjSearchProcessTime = re.search('SearchProcess ([\d]+.[\d]+) msec.', line)
    if(matchObjSearchProcessTime):
        nSearchProcessTime = float(matchObjSearchProcessTime.group(1))
        dictSomeRecord['nSearchProcessTime'] = nSearchProcessTime

    # 匹配语义解析时间
    matchObjNLIInterpretTime = re.search('VA::EMNlu::parse  NLIInterpret ([\d]+.[\d]+) msec.', line)
    if(matchObjNLIInterpretTime):
        nNLIInterpretTime = float(matchObjNLIInterpretTime.group(1))
        dictSomeRecord['nNLIInterpretTime'] = nNLIInterpretTime
        if(not('nSrResultTime' in dictSomeRecord)
           or not('nNLIInterpretTime' in dictSomeRecord)):
            print('file %s format error333!!' % (szIssSrLogFileName))
            print(dictSomeRecord)
            sys.exit(2)
            #continue
        listRecords_nSrResultTime_nSearchProcessTime_nNLIInterpretTime.append(dictSomeRecord.copy())
        del dictSomeRecord['nSrResultTime']
        if('nSearchProcessTime' in dictSomeRecord): del dictSomeRecord['nSearchProcessTime']
        del dictSomeRecord['nNLIInterpretTime']

# 这里打印一下AIUI套件中抓取到的有语义结果的记录数目
print("%s Record num: %d." % (szIssSrLogFileName, len(listRecords_nSrResultTime_nSearchProcessTime_nNLIInterpretTime)))


# 要求工具中抓取到的有语义结果的记录数目 等于AIUI套件中 抓取到的有语义结果的数目，否则报错退出
if(len(listRecords_strAudioPath_nAppenAudioTime_nAudioVadEndPoint_nMsgCallBackTime)
   != len(listRecords_nSrResultTime_nSearchProcessTime_nNLIInterpretTime)):
    print('Error, Records %s,%s num do not match!!' % (szTestISSAIUILogFileName, szIssSrLogFileName))
    sys.exit(3)

# 读取如子的VAD的日志文件
listVadInfo = []
dictVadInfo = {}
try:
    fhVadInfo = open(szVadInfoFileName, mode='r', encoding='utf-8')
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

# 输出分段统计效率的报表
nRecordCnt = 0
listOutputRows = []
nSearchProcessTimeSum = 0
nSearchProcessCount = 0
nNLIInterpretTimeSum = 0
nNLIInterpretCount = 0
nRecTimeSum = 0
nRecCount = 0
nRecTimeBiggerThan3sCount = 0
nFinalResultTimeSum = 0
nFinalResultCount = 0
nFinalResultTimeBiggerThan3sCount = 0
for some_record in listRecords_strAudioPath_nAppenAudioTime_nAudioVadEndPoint_nMsgCallBackTime:
    strAudioPath = some_record['strAudioPath']
    nAppenAudioTime = some_record['nAppenAudioTime']

    nAudioVadEndPoint = some_record['nAudioVadEndPoint']
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

    nMsgCallBackTime = some_record['nMsgCallBackTime']
    nSrResultTime = listRecords_nSrResultTime_nSearchProcessTime_nNLIInterpretTime[nRecordCnt]['nSrResultTime']
    nSearchProcessTime = 0
    if('nSearchProcessTime' in listRecords_nSrResultTime_nSearchProcessTime_nNLIInterpretTime[nRecordCnt]):
        nSearchProcessTime = listRecords_nSrResultTime_nSearchProcessTime_nNLIInterpretTime[nRecordCnt]['nSearchProcessTime']
    if(nSearchProcessTime <= 0):
        nSearchProcessTime = ''
    else:
        nSearchProcessTimeSum += nSearchProcessTime
        nSearchProcessCount += 1
    nNLIInterpretTime = listRecords_nSrResultTime_nSearchProcessTime_nNLIInterpretTime[nRecordCnt]['nNLIInterpretTime']
    nNLIInterpretTimeSum += nNLIInterpretTime
    nNLIInterpretCount += 1
    nRecTime = nSrResultTime - nAppenAudioTime - nAudioVadEndPoint
    if(nRecTime >= 3000):
        nRecTimeBiggerThan3sCount += 1
    if(nRecTime <= 0):
        nRecTime = ''
    else:
        nRecTimeSum += nRecTime
        nRecCount += 1
    nFinalResultTime = nMsgCallBackTime - nAppenAudioTime - nAudioVadEndPoint
    if(nFinalResultTime >= 3000):
        nFinalResultTimeBiggerThan3sCount += 1
    if(nFinalResultTime <= 0):
        nFinalResultTime = ''
    else:
        nFinalResultTimeSum += nFinalResultTime
        nFinalResultCount += 1
    listOutputRows.append([strAudioPath, nAppenAudioTime, nAudioVadEndPoint, nMsgCallBackTime,
                             nSrResultTime, nSearchProcessTime, nNLIInterpretTime, nRecTime, nFinalResultTime])
    nRecordCnt += 1
# 防止除0错误
if(nSearchProcessCount == 0):
    nSearchProcessTimeSum = 0
    nSearchProcessCount = 1
if(nNLIInterpretCount == 0):
    nNLIInterpretTimeSum = 0
    nNLIInterpretCount = 1
if(nRecCount == 0):
    nRecTimeSum = 0
    nRecCount = 1
if(nFinalResultCount == 0):
    nFinalResultTimeSum = 0
    nFinalResultCount = 1

# 输出报表 到csv格式的文件中
with open(szOutputFileName, 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile)
    spamwriter.writerow(
        ['strAudioPath',
         'nAppenAudioTime(UTC ms)',
         'nAudioVadEndPoint(ms)',
         'nMsgCallBackTime(UTC ms)',
         'nSrResultTime(UTC ms)',
         'nSearchProcessTime(ms) %.2f' % (nSearchProcessTimeSum / nSearchProcessCount),
         'nNLIInterpretTime(ms) %.2f' % (nNLIInterpretTimeSum / nNLIInterpretCount),
         'nRecTime(ms) %.2f' % (nRecTimeSum / nRecCount),
         'nFinalResultTime(ms) %.2f' % (nFinalResultTimeSum / nFinalResultCount),
         'RecTimeBiggerThan3s(%%) %d/%d=%.2f%%' % (
             nRecTimeBiggerThan3sCount, nRecCount, nRecTimeBiggerThan3sCount * 100 / nRecCount),
         'FinalResultTimeBiggerThan3s(%%) %d/%d=%.2f%%' % (
             nFinalResultTimeBiggerThan3sCount, nFinalResultCount, nFinalResultTimeBiggerThan3sCount * 100 / nFinalResultCount)])
    nRecordCnt = 0
    for some_row in listOutputRows:
        spamwriter.writerow(some_row)
        nRecordCnt += 1
    print('success :) output filename=%s' % (szOutputFileName))
