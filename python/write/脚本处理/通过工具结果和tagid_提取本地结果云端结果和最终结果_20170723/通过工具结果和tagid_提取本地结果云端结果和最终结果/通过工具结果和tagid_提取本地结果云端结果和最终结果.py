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
# 工具日志“TestISS-AIUI_xxx.log”所在路径
strTestISSAIUILogDir = './sr/effect/mix/'
# 生成保存了录音和结果的csv报表的文件名
strOutPutResultFileName = './out_mix_result.csv'

# 读取命令行参数
if(len(sys.argv) != 1 and len(sys.argv) != 3):
    print('Invalid Arg! len(sys.argv)=%d' % (len(sys.argv)))
    print('Usage: %s <strTestISSAIUILogDir> <strOutPutResultFileName>' % (sys.argv[0]))
    sys.exit(1)
if(len(sys.argv) == 3):
    # 如果传入了命令行参数，就按照命令行参数为准
    strTestISSAIUILogDir = sys.argv[1]
    strOutPutResultFileName = sys.argv[2]

# 列举strTestISSAIUILogDir目录的所有文件
listTestISSAIUILogDirFiles = os.listdir(strTestISSAIUILogDir)

# 筛选所有TestISS-AIUI_xxx.log日志文件
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
        # 对日志文件，按照UTC时间排序，时间戳约小的 排序越靠前，因为是先写入的
        nTimeX = time.mktime(time.strptime(matchObjX.group(1), '%Y-%m-%d_%H-%M-%S'))
        nTimeY = time.mktime(time.strptime(matchObjY.group(1), '%Y-%m-%d_%H-%M-%S'))
        if(nTimeX < nTimeY) :
            return -1
        elif(nTimeX > nTimeY) :
            return 1
        else:
            return 0
    else:
        # 对TestISS-AIUI.log文件排序，不带时间戳的 排在最后面，因为是最后写入的
        if(not matchObjX and not matchObjY):
            print('Error: Find two TestISS-AIUI.log: %s, %s.' % (x, y))
            sys.exit(1)
        if(not matchObjX):
            return 1
        else:
            return -1
# 调用时间戳排序算法进行排序
listTestISSAIUILogDirLogFileNames.sort(key=cmp_to_key(lambda a, b: comp_by_utc(a, b)))

# 逐个读取每一个日志文件，按行读取到listLog文件
listLog = []
for line in listTestISSAIUILogDirLogFileNames:
    log_file_name = strTestISSAIUILogDir + '/' + line
    log_file_name = log_file_name.replace('\\', '/')
    log_file_name = log_file_name.replace('//', '/')
    print('正在读取套件工具日志: %s' % (log_file_name))
    fhLog = open(log_file_name, mode='r', encoding='gbk')
    for file_line in fhLog: listLog.append(file_line)

# 解析日志文件，读取audio_path和audioTag到一个列表中
listAudioPathAndAudioTag = []
dictAudioPathAndAudioTag = {}
for line in listLog:
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
        # 记录audio_path和audioTag
        listAudioPathAndAudioTag.append(dictAudioPathAndAudioTag.copy())

# 解析日志文件，读取本地结果、云端结果、混合结果到一个字典中
dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults = {}
find_local_flag = 0
find_cloud_flag = 0
is_mix = 0
for line in listLog:
    # 匹配本地结果
    matchObj = re.search(
        '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*get callback msg: 20026_ISS_SR_MSG_LocalResult.*, lParam: (.*)$', line)
    if(matchObj):
        find_local_flag = 1
        LocalResult = matchObj.group(3)
        if(len(LocalResult) <= 0):
            print("这一行本地结果为空，请查证：%s" % (line))
            continue
        LocalResultText = ''
        audioTag = ''
        jsonObj = json.loads(LocalResult)
        if('intent' in jsonObj
           and 'text' in jsonObj['intent']):
            LocalResultText = jsonObj['intent']['text']
        else:
            print("格式错误，没有intent.text字段：%s" % (LocalResult))
            sys.exit(1)
        if('intent' in jsonObj
           and 'audioTag' in jsonObj['intent']):
            audioTag = str(jsonObj['intent']['audioTag'])
        else:
            print("格式错误，没有intent.audioTag字段：%s" % (LocalResult))
            sys.exit(1)
        if(not audioTag in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults):
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag] = {}
        if('LocalResult' in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]):
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['LocalResult'].append(LocalResult)
        else:
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['LocalResult'] = [ LocalResult ]
        if('LocalResultText' in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]):
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['LocalResultText'].append(LocalResultText)
        else:
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['LocalResultText'] = [ LocalResultText ]

    # 匹配云端结果
    matchObj = re.search(
        '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*get callback msg: 20025_ISS_SR_MSG_CloudResult.*, lParam: (.*)$', line)
    if(matchObj):
        find_cloud_flag = 1
        CloudResult = matchObj.group(3)
        if(len(CloudResult) <= 0):
            print("这一行云端结果为空，请查证：%s" % (line))
            continue
        CloudResultText = ''
        audioTag = ''
        jsonObj = json.loads(CloudResult)
        if('intent' in jsonObj
           and 'text' in jsonObj['intent']):
            CloudResultText = jsonObj['intent']['text']
        else:
            print("格式错误，没有intent.text字段：%s" % (CloudResult))
            sys.exit(1)
        if('intent' in jsonObj
           and 'audioTag' in jsonObj['intent']):
            audioTag = str(jsonObj['intent']['audioTag'])
        else:
            print("格式错误，没有intent.audioTag字段：%s" % (CloudResult))
            sys.exit(1)
        if(not audioTag in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults):
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag] = {}
        if('CloudResult' in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]):
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['CloudResult'].append(CloudResult)
        else:
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['CloudResult'] = [ CloudResult ]
        if('CloudResultText' in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]):
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['CloudResultText'].append(CloudResultText)
        else:
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['CloudResultText'] = [ CloudResultText ]
        
    # 匹配最终结果
    matchObj = re.search(
    '\[([\d]+/[\d]+/[\d]+-[\d]+:[\d]+:[\d]+) ([\d]+)\].*get callback msg: 20009_ISS_SR_MSG_Result.*, lParam: (.*)$', line)
    if(matchObj):
        FinalResult = matchObj.group(3)
        if(len(FinalResult) <= 0):
            print("这一行最终结果为空，请查证：%s" % (line))
            continue
        FinalResultText = ''
        audioTag = ''
        bislocalresult = ''
        jsonObj = json.loads(FinalResult)
        if('intent' in jsonObj
           and 'text' in jsonObj['intent']):
            FinalResultText = jsonObj['intent']['text']
        else:
            print("格式错误，没有intent.text字段：%s" % (FinalResult))
            sys.exit(1)
        if('intent' in jsonObj
           and 'audioTag' in jsonObj['intent']):
            audioTag = str(jsonObj['intent']['audioTag'])
        else:
            print("格式错误，没有intent.audioTag字段：%s" % (FinalResult))
            sys.exit(1)
        if('intent' in jsonObj
           and 'bislocalresult' in jsonObj['intent']):
            bislocalresult = '1'
        else:
            bislocalresult = '0'
        if(not audioTag in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults):
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag] = {}
        if('FinalResult' in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]):
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResult'].append(FinalResult)
        else:
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResult'] = [ FinalResult ]
        if('FinalResultText' in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]):
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResultText'].append(FinalResultText)
        else:
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResultText'] = [ FinalResultText ]
        if('bislocalresult' in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]):
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['bislocalresult'].append(bislocalresult)
        else:
            dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['bislocalresult'] = [ bislocalresult ]
if(find_local_flag and find_cloud_flag):
    is_mix = 1

# 输出识别结果的报表 到csv格式的文件中
print("找到了总的录音条目数为: %d" % (len(listAudioPathAndAudioTag)))  # 这里输出一下录音文件个数
print("其中有结果的录音条目数为: %d" % (len(dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults)))  # 这里输出有结果的一下录音文件个数
with open(strOutPutResultFileName, 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile)
    if(is_mix):
        spamwriter.writerow(['audio_path', 'audioTag',
                             'LocalResultText0', 'LocalResultText1',
                             'CloudResultText0', 'CloudResultText1',
                             'FinalResultText0', 'isLocal0',
                             'FinalResultText1', 'isLocal1',
                             'LocalResult0', 'LocalResult1',
                             'CloudResult0', 'CloudResult1',
                             'FinalResult0', 'FinalResult1'])
        for line in listAudioPathAndAudioTag:
            audio_path = line['audio_path']
            audioTag = line['audioTag']
            LocalResultText0 = ''
            LocalResultText1 = ''
            CloudResultText0 = ''
            CloudResultText1 = ''
            FinalResultText0 = ''
            isLocal0 = ''
            FinalResultText1 = ''
            isLocal1 = ''
            LocalResult0 = ''
            LocalResult1 = ''
            CloudResult0 = ''
            CloudResult1 = ''
            FinalResult0 = ''
            FinalResult1 = ''
            if(audioTag in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults):
                if('LocalResult' in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]):
                    if(len(dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['LocalResult']) == 1):
                        LocalResult0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['LocalResult'][0]
                        LocalResultText0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['LocalResultText'][0]
                    elif(len(dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['LocalResult']) == 2):
                        LocalResult0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['LocalResult'][0]
                        LocalResultText0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['LocalResultText'][0]
                        LocalResult1 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['LocalResult'][1]
                        LocalResultText1 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['LocalResultText'][1]
                    elif(len(dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['LocalResult']) >= 3):
                        print("本地结果多于2个 , \"audioTag\":%s" % (audioTag))
                if('CloudResult' in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]):
                    if(len(dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['CloudResult']) == 1):
                        CloudResult0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['CloudResult'][0]
                        CloudResultText0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['CloudResultText'][0]
                    elif(len(dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['CloudResult']) == 2):
                        CloudResult0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['CloudResult'][0]
                        CloudResultText0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['CloudResultText'][0]
                        CloudResult1 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['CloudResult'][1]
                        CloudResultText1 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['CloudResultText'][1]
                    elif(len(dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['CloudResult']) >= 3):
                        print("云端结果多于2个, \"audioTag\":%s" % (audioTag))
                if('FinalResult' in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]):
                    if(len(dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResult']) == 1):
                        FinalResult0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResult'][0]
                        FinalResultText0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResultText'][0]
                        isLocal0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['bislocalresult'][0]
                    elif(len(dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResult']) == 2):
                        FinalResult0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResult'][0]
                        FinalResultText0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResultText'][0]
                        isLocal0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['bislocalresult'][0]
                        FinalResult1 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResult'][1]
                        FinalResultText1 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResultText'][1]
                        isLocal1 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['bislocalresult'][1]
                    elif(len(dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResult']) >= 3):
                        print("最终混合结果多于2个, \"audioTag\":%s" % (audioTag))
            spamwriter.writerow([audio_path, audioTag,
                             LocalResultText0, LocalResultText1,
                             CloudResultText0, CloudResultText1,
                             FinalResultText0, isLocal0,
                             FinalResultText1, isLocal1,
                             LocalResult0, LocalResult1,
                             CloudResult0, CloudResult1,
                             FinalResult0, FinalResult1])
    else:
        spamwriter.writerow(['audio_path', 'audioTag',
                             'FinalResultText0', 'FinalResultText1',
                             'FinalResult0', 'FinalResult1'])
        for line in listAudioPathAndAudioTag:
            audio_path = line['audio_path']
            audioTag = line['audioTag']
            FinalResultText0 = ''
            FinalResultText1 = ''
            FinalResult0 = ''
            FinalResult1 = ''
            if(audioTag in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults):
                if('FinalResult' in dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]):
                    if(len(dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResult']) == 1):
                        FinalResult0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResult'][0]
                        FinalResultText0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResultText'][0]
                    elif(len(dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResult']) == 2):
                        FinalResult0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResult'][0]
                        FinalResultText0 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResultText'][0]
                        FinalResult1 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResult'][1]
                        FinalResultText1 = dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResultText'][1]
                    elif(len(dictAudioTag_to_LocalResultsAndCloudResultsAndFinalResults[audioTag]['FinalResult']) >= 3):
                        print("最终结果多于2个, \"audioTag\":%s" % (audioTag))
            spamwriter.writerow([audio_path, audioTag,
                             FinalResultText0, FinalResultText1,
                             FinalResult0, FinalResult1])
