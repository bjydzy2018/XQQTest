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
strVadInfoFileName = 'vad前后端点.txt'
strTestIssAIUILogEffOutputFileName = 'speed_cloud.csv'

# 上一次的音频结果 给了下一次时，配置这个时间间隔
configure_callback_msg_should_not_be_late_then_audio_path_time = 1000

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
    fhTestISSAIUILog = open(strTestIssAIUILogFileName, mode='r', encoding='gbk', errors="replace")
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
    '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*get callback msg: 20029_ISS_SR_MSG_SRResult.*, lParam: (.*)$', line)
    if(matchObj):
        FinalResult = matchObj.group(3)
        if(len(FinalResult) <= 0):
            print("这一行最终结果为空，请查证：%s" % (line))
            continue
        audioTag = ''
        try:
            jsonObj = json.loads(FinalResult)
            #if('intent' in jsonObj
            #   and 'audioTag' in jsonObj['intent']):
            #    audioTag = str(jsonObj['intent']['audioTag'])
            #else:
            #    print("格式错误，没有intent.audioTag字段：%s" % (FinalResult))
            #    sys.exit(1)
            if('audioTag' in jsonObj):
                audioTag = str(jsonObj['audioTag'])
            else:
                print("格式错误，没有audioTag字段：%s" % (FinalResult))
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
            

# 输出报表 到csv格式的文件中
with open(strTestIssAIUILogEffOutputFileName, 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile)
    spamwriter.writerow(
        ['strAudioPath', 'nAppenAudioTime(UTC ms)', 'nAudioVadEndPoint(ms)', 'nMsgCallBackTime(UTC ms)', 'nFinalResultTime(ms)'])
    for line in listAudioPathAndAudioTag:
        strAudioPath = line['audio_path']
        audioTag = line['audioTag']
        nAppenAudioTime = line['nAppenAudioTime']
        nAudioVadEndPoint = ''
        nMsgCallBackTime = ''
        nFinalResultTime = ''
        if audioTag in dictAudioTag_to_FinalResults:
            nMsgCallBackTime = dictAudioTag_to_FinalResults[audioTag]['nMsgCallBackTime']
            matchObjAudioPath = re.search('.*?([^\\\/]+\.)(wav|pcm)', strAudioPath)
            if(matchObjAudioPath):
                path = matchObjAudioPath.group(1)
                if(path in dictVadInfo):
                    nAudioVadEndPoint = dictVadInfo[path]
                else:
                    print('not found vad end: %s!!' % (path))
                    sys.exit(2)
            else:
                print('pcm path: %s format error333!!' % (strAudioPath))
                sys.exit(2)
        if(nMsgCallBackTime != ''
           and nAppenAudioTime != ''
           and nAudioVadEndPoint != ''
           and (int(nMsgCallBackTime) - int(nAppenAudioTime) - int(nAudioVadEndPoint)) > 0):
            nFinalResultTime = int(nMsgCallBackTime) - int(nAppenAudioTime) - int(nAudioVadEndPoint)
        spamwriter.writerow([strAudioPath, nAppenAudioTime, nAudioVadEndPoint, nMsgCallBackTime, nFinalResultTime])
    print('success :) 输出文件名: %s' % (strTestIssAIUILogEffOutputFileName))
