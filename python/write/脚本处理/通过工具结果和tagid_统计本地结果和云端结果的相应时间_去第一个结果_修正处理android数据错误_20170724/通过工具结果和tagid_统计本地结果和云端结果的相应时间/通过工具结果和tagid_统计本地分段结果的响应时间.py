  # coding=utf-8
# python3

import sys
import re
import time
import csv
import codecs
import json

# 默认参数，当没有命令行传参时 使用这些参数
strTestIssAIUILogFileName = './speed/cloud/TestISS-AIUI.log'
szIssSrLogFileName = './speed/local/isssr.log'
strVadInfoFileName = 'vad前后端点.txt'
strTestIssAIUILogEffOutputFileName = 'speed_cloud.csv'

# 上一次的音频结果 给了下一次时，配置这个时间间隔
configure_callback_msg_should_not_be_late_then_audio_path_time = 1000

# 读取命令行参数
if(len(sys.argv) != 1 and len(sys.argv) != 5):
    print('Invalid Arg! len(sys.argv)=%d' % (len(sys.argv)))
    print('Uasge: %s <strTestIssAIUILogFileName.log> <szIssSrLogFileName.log> <strVadInfoFileName.txt> <strTestIssAIUILogEffOutputFileName.csv>'
          % (sys.argv[0]))
    sys.exit(1)
if(len(sys.argv) == 5):
    strTestIssAIUILogFileName = sys.argv[1]
    szIssSrLogFileName = sys.argv[2]
    strVadInfoFileName = sys.argv[3]
    strTestIssAIUILogEffOutputFileName = sys.argv[4]

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

# 解析日志文件，读取 audio_path、audioTag和开始送音频的时间 到一个列表中
listAudioPathAndAudioTag = []
dictAudioPathAndAudioTag = {}
for line in listTestISSAIULogLines:
    # 匹配录音文件名
    matchObj = re.search(
        '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*current test num: [\d]+, audio_path: (.*)', line)
    if(matchObj):
        # 记录audio_path
        dictAudioPathAndAudioTag['audio_path'] = matchObj.group(3)

    # 匹配 audioTag
    matchObj = re.search(
        '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*ISSSRSetParam audioTag audioNum ([\d]+) ret 0', line)
    if(matchObj):
        # 记录audioTag
        dictAudioPathAndAudioTag['audioTag'] = matchObj.group(3)

    # 匹配开始送音频的时间
    matchObj = re.search(
        '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*append audio data to sr', line)
    if(matchObj):
        strTime = matchObj.group(1)
        nMsec = int(matchObj.group(2))
        nAppenAudioTime = time.mktime(time.strptime(strTime, '%y/%m/%d-%H:%M:%S')) * 1000 + nMsec
        # 记录开始送音频的时间
        dictAudioPathAndAudioTag['nAppenAudioTime'] = nAppenAudioTime
        # 记录audio_path、audioTag和开始送音频的时间
        listAudioPathAndAudioTag.append(dictAudioPathAndAudioTag.copy())
        dictAudioPathAndAudioTag.clear()

# 解析日志文件，读取结果到一个字典中
dictAudioTag_to_FinalResults = {}
for line in listTestISSAIULogLines:
    # 匹配最终结果
    matchObj = re.search(
    '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*get callback msg: 20009_ISS_SR_MSG_Result.*, lParam: (.*)$', line)
    if(matchObj):
        FinalResult = matchObj.group(3)
        if(len(FinalResult) <= 0):
            print("这一行最终结果为空，请查证：%s" % (line))
            continue
        audioTag = ''
        try:
            jsonObj = json.loads(FinalResult)
            if('intent' in jsonObj
               and 'audioTag' in jsonObj['intent']):
                audioTag = str(jsonObj['intent']['audioTag'])
            else:
                print("格式错误，没有intent.audioTag字段：%s" % (FinalResult))
                sys.exit(1)
        except Exception as e:
            matchObjAudioTag = re.search('"audioTag": *([0-9]+)', line)
            if(matchObjAudioTag):
                audioTag = matchObjAudioTag.group(1)
            else:
                print("格式错误，可能是语义结果太长 被分到两行日志了，在第一行日志 没有发现audioTag: %s %s"
                      % (matchObj.group(1), matchObj.group(2)))
                sys.exit(1)                
        if(not audioTag in dictAudioTag_to_FinalResults):
            # 只取这句话 第一次给结果的时间
            dictAudioTag_to_FinalResults[audioTag] = {}
            strTime = matchObj.group(1)
            nMsec = int(matchObj.group(2))
            nMsgCallBackTime = time.mktime(time.strptime(strTime, '%y/%m/%d-%H:%M:%S')) * 1000 + nMsec
            dictAudioTag_to_FinalResults[audioTag]['nMsgCallBackTime'] = nMsgCallBackTime

# 解析AIUI套件的日志数据
dict_audioTag_to_nSrResultTime_nSearchProcessTime_nNLIInterpretTime = {}
dictSomeRecord = {}
for line in listIssSrLogLines:
    # 匹配识别时间
    if(re.search('m_strText', line)):
        matchObj = re.search('\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*Sr Result: .*m_nConfidenceScore', line)
        if(matchObj):
            strTime = matchObj.group(1)
            nMsec = int(matchObj.group(2))
            nSrResultTime = time.mktime(time.strptime(strTime, '%y/%m/%d-%H:%M:%S')) * 1000 + nMsec
            dictSomeRecord['nSrResultTime'] = nSrResultTime
            if('nSearchProcessTime' in dictSomeRecord): del dictSomeRecord['nSearchProcessTime']
            if('nNLIInterpretTime' in dictSomeRecord): del dictSomeRecord['nNLIInterpretTime']
        
    # 匹配cata搜索时间
    matchObj = re.search('SearchProcess ([\d]+.[\d]+) msec.', line)
    if(matchObj):
        nSearchProcessTime = float(matchObj.group(1))
        dictSomeRecord['nSearchProcessTime'] = nSearchProcessTime

    # 匹配语义解析时间
    matchObj = re.search('VA::EMNlu::parse  NLIInterpret ([\d]+.[\d]+) msec.', line)
    if(matchObj):
        nNLIInterpretTime = float(matchObj.group(1))
        dictSomeRecord['nNLIInterpretTime'] = nNLIInterpretTime

    # 匹配audioTag
    matchObj = re.search('\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].* m_strOrgNlpResult: (.+)$', line)
    if(matchObj):
        audioTag = ''
        try:
            jsonObj = json.loads(matchObj.group(3))
            if('intent' in jsonObj
               and 'audioTag' in jsonObj['intent']):
                audioTag = str(jsonObj['intent']['audioTag'])
            else:
                print("格式错误，没有intent.audioTag字段：%s" % (matchObj.group(3)))
                sys.exit(1)
        except Exception as e:
            matchObjAudioTag = re.search('"audioTag": *([0-9]+)', line)
            if(matchObjAudioTag):
                audioTag = matchObjAudioTag.group(1)
            else:
                print("格式错误，可能是语义结果太长 被分到两行日志了，在第一行日志 没有发现audioTag: %s %s"
                      % (matchObj.group(1), matchObj.group(2)))
                sys.exit(1)                
        if(not audioTag in dict_audioTag_to_nSrResultTime_nSearchProcessTime_nNLIInterpretTime):
            if(not('nSrResultTime' in dictSomeRecord)
               or not('nNLIInterpretTime' in dictSomeRecord)):
                print('file %s format error333!! %s' % (szIssSrLogFileName, line))
                print(dictSomeRecord)
                sys.exit(2)
            # 只取这句话 第一次给结果的时间
            dict_audioTag_to_nSrResultTime_nSearchProcessTime_nNLIInterpretTime[audioTag] = dictSomeRecord.copy()

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
for some_record in listAudioPathAndAudioTag:
    strAudioPath = some_record['audio_path']
    nAppenAudioTime = some_record['nAppenAudioTime']
    audioTag = some_record['audioTag']
    nAudioVadEndPoint = ''    
    nMsgCallBackTime = ''
    if audioTag in dictAudioTag_to_FinalResults:
        nMsgCallBackTime = dictAudioTag_to_FinalResults[audioTag]['nMsgCallBackTime']
        matchObj = re.search('.*?([^\\\/]+\.)(wav|pcm)', strAudioPath)
        if(matchObj):
            path = matchObj.group(1)
            if(path in dictVadInfo):
                nAudioVadEndPoint = dictVadInfo[path]
            else:
                print('not found vad end: %s!!' % (path))
                sys.exit(2)
        else:
            print('pcm path: %s format error333!!' % (strAudioPath))
            sys.exit(2)    
        nSrResultTime = dict_audioTag_to_nSrResultTime_nSearchProcessTime_nNLIInterpretTime[audioTag]['nSrResultTime']
        nSearchProcessTime = 0
        if('nSearchProcessTime' in dict_audioTag_to_nSrResultTime_nSearchProcessTime_nNLIInterpretTime[audioTag]):
            nSearchProcessTime = dict_audioTag_to_nSrResultTime_nSearchProcessTime_nNLIInterpretTime[audioTag]['nSearchProcessTime']
        if(nSearchProcessTime <= 0):
            nSearchProcessTime = ''
        else:
            nSearchProcessTimeSum += nSearchProcessTime
            nSearchProcessCount += 1
        nNLIInterpretTime = dict_audioTag_to_nSrResultTime_nSearchProcessTime_nNLIInterpretTime[audioTag]['nNLIInterpretTime']
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
with open(strTestIssAIUILogEffOutputFileName, 'w', newline='') as csvfile:
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
    print('success :) 输出文件：%s' % (strTestIssAIUILogEffOutputFileName))
