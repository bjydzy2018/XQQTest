# coding=utf-8
# python3

import xlrd
import re
import os
import sys
import time
import json
import csv
from functools import cmp_to_key

# 默认参数，当没有命令行传参时 使用这些参数
strNotCompareKeysFileName = '在这里用正则表达式列举不需要参与比对的键.txt'
strTestISSAIUILogDir = './'
strBVTFileName = 'BVT.xls'
strNliResultFileName = 'output_cloud_nli_consistent.csv'
strCompareResultFileName = 'output_CompareResult.csv'


# 上一次的音频结果 给了下一次时，配置这个时间间隔
configure_callback_msg_should_not_be_late_then_audio_path_time = 600

# 读取命令行参数
if(len(sys.argv) != 1 and len(sys.argv) != 6):
    print('Invalid Arg! len(sys.argv)=%d' % (len(sys.argv)))
    print('Uasge: %s <在这里用正则表达式列举不需要参与比对的键.txt> <strTestISSAIUILogDir> <BVT.xls> <cloud_nli_consistent.csv> <CompareResult.csv>'
          % (sys.argv[0]))
    sys.exit(1)
if(len(sys.argv) == 6):
    strNotCompareKeysFileName = sys.argv[1]
    strTestISSAIUILogDir = sys.argv[2]
    strBVTFileName = sys.argv[3]
    strNliResultFileName = sys.argv[4]
    strCompareResultFileName = sys.argv[5]


# 读取用正则表达式表示的列举不需要参与比对的键
NotCompareKeysUsingRE = []
try:
    fileNotCompareKeysUsingRE = open(strNotCompareKeysFileName, mode='r', encoding='utf-8')
    for line in fileNotCompareKeysUsingRE: NotCompareKeysUsingRE.append(line.strip())
except Exception as e:
    try:
        fileNotCompareKeysUsingRE = open(NotCompareKeysUsingRE, mode='r', encoding='gbk')
        for line in fileNotCompareKeysUsingRE: NotCompareKeysUsingRE.append(line.strip())
    except Exception as e:
        print('File Error:' + str(e))
        sys.exit(1)


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
    print('正在处理日志文件: %s' % (log_file_name))
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
            print('这条音频给结果太快了 认为是上一条音频的结果, strAudioPath: %s'
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

# 输出语义结果的报表 到csv格式的文件中
print("从日志文件中 找到录音文件数: %d" % (len(listRecords)))  # 这里输出一下录音文件个数
with open(strNliResultFileName, 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile)
    spamwriter.writerow(
        ["strAudioPath", "text_0", "text_1", "operation", "service", "slots", "strISS_SR_MSG_Result_0", "strISS_SR_MSG_Result_1"])
    for line in listRecords:
        strAudioPath = line['strAudioPath']
        text = ''
        operation = ''
        service = ''
        slots = ''
        strISS_SR_MSG_Result_0 = ''
        if('strISS_SR_MSG_Result_0' in line): strISS_SR_MSG_Result_0 = line['strISS_SR_MSG_Result_0']
        if(len(strISS_SR_MSG_Result_0) > 0):
            json_obj = json.loads(strISS_SR_MSG_Result_0)
            text = json_obj['intent']['text']
            if('operation' in json_obj['intent']): operation = json_obj['intent']['operation']
            if('service' in json_obj['intent']): service = json_obj['intent']['service']
            if('semantic' in json_obj['intent']
               and 'slots' in json_obj['intent']['semantic']):
                slots = json.dumps(json_obj['intent']['semantic'],
                                   sort_keys=False, ensure_ascii=False, separators=(', ', ': ')) 
        strISS_SR_MSG_Result_1 = ''
        text_1 = ''
        if('strISS_SR_MSG_Result_1' in line): strISS_SR_MSG_Result_1 = line['strISS_SR_MSG_Result_1']
        if(len(strISS_SR_MSG_Result_1) > 0):
            json_obj = json.loads(strISS_SR_MSG_Result_1)
            text_1 = json_obj['intent']['text']
        spamwriter.writerow(
            [strAudioPath, text, text_1, operation, service, slots, strISS_SR_MSG_Result_0, strISS_SR_MSG_Result_1])
    print('输出从日志中提取到的语义结果到文件: %s.' % (strNliResultFileName))


# 读取语义结果1
NliResult1 = []
csv_reader = csv.reader(open(strNliResultFileName, mode='r', encoding='gbk'))
for row in csv_reader:
    NliResult1.append([row[0], row[1], row[3], row[4], row[5]])
del NliResult1[0]
# 读取语义结果2
NliResult2 = []
book = xlrd.open_workbook(strBVTFileName)
sheet = book.sheet_by_name("sheet0")
for row in range(1, sheet.nrows):
    strAudioPath = ''
    text = sheet.cell_value(rowx=row, colx=5)
    operation = sheet.cell_value(rowx=row, colx=8)
    service = sheet.cell_value(rowx=row, colx=6)
    if(service == 'rc=4'):
        service = ''
    slots = sheet.cell_value(rowx=row, colx=7)
    NliResult2.append([strAudioPath, text, operation, service, slots])
if(len(NliResult1) <= 0 or len(NliResult2) <= 0 or len(NliResult1) != len(NliResult2)):
    print('输入的语义文件有问题，条目个数不匹配, %d!=%d' % (len(NliResult1), len(NliResult2)))
    sys.exit(1)
print("获取到%d条语义条目" % (len(NliResult1)))

def expendJson(jsonObj, prefix, dictExpendedJson):
    if(type(jsonObj) == type(0)
       or type(jsonObj) == type('')
       or type(jsonObj) == type(True)
       or type(jsonObj) == type(0.0)):
        dictExpendedJson[prefix[1:]] = jsonObj
    elif(type(jsonObj) == type([])):
        for i in range(len(jsonObj)):
            expendJson(jsonObj[i], prefix + '[%d]' % (i), dictExpendedJson)
    elif(type(jsonObj) == type({})):
        for (key, value) in jsonObj.items():
            expendJson(value, prefix + '.' + key, dictExpendedJson)
    else:
        print('非法的json对象: %s' % (jsonObj))
        sys.exit(1)

def delNotCompare(dictExpendedJson1, NotCompareKeysUsingRE):
    keys_del = []
    for key in dictExpendedJson1.keys():
        match_flag = 0
        for some_val in NotCompareKeysUsingRE:
            if(re.search(some_val, key)):
                match_flag = 1
                break
        if(match_flag == 1):
            keys_del.append(key)
    for key_del in keys_del:
        del dictExpendedJson1[key_del]

def getOnlyInTheFirst(dictExpendedJsonFirst, dictExpendedJsonSecond):
    strOnlyInNliResult = ''
    for (key, value) in dictExpendedJsonFirst.items():
        if(not(key in dictExpendedJsonSecond)):
            if(len(strOnlyInNliResult) > 0):
                strOnlyInNliResult += '\n' + key + ': ' + value
            else:
                strOnlyInNliResult += key + ': ' + value
    return strOnlyInNliResult

def getValueDifferent(dictExpendedJsonFirst, dictExpendedJsonSecond):
    strValueDifferent = ''
    for (key, value) in dictExpendedJsonFirst.items():
        if(key in dictExpendedJsonSecond):
            if(value != dictExpendedJsonSecond[key]):
                if(len(strValueDifferent) > 0):
                    strValueDifferent += '\n' + key + ': ' + value + ', ' + dictExpendedJsonSecond[key]
                else:
                    strValueDifferent += key + ': ' + value + ', ' + dictExpendedJsonSecond[key]
    return strValueDifferent

# 比对一致率
listOutRows = []
operation_equal_count = 0
service_equal_count = 0
slots_equal_count = 0
all_equal_count = 0
for i in range(len(NliResult1)):
    all_equal_flag = True
    row_out = []
    row_out.append(NliResult1[i][0])
    row_out.append(NliResult1[i][1])

    row_out.append(NliResult1[i][2])
    row_out.append(NliResult2[i][2])
    if(NliResult1[i][2] == NliResult2[i][2]):
        row_out.append('True')
        operation_equal_count += 1
    else:
        row_out.append('False')
        all_equal_flag = False

    row_out.append(NliResult1[i][3])
    row_out.append(NliResult2[i][3])
    if(NliResult1[i][3] == NliResult2[i][3]):
        row_out.append('True')
        service_equal_count += 1
    else:
        row_out.append('False')
        all_equal_flag = False

    row_out.append(NliResult1[i][4])
    row_out.append(NliResult2[i][4])
    strOnlyInNliResult1 = ''
    strOnlyInNliResult2 = ''
    strValueDifferent = ''
    
    dictExpendedJson1 = {}
    if(NliResult1[i][4] == ''): NliResult1[i][4] = '{}'
    expendJson(json.loads(NliResult1[i][4]), '', dictExpendedJson1)  # 把json对象 拆成键值对
    delNotCompare(dictExpendedJson1, NotCompareKeysUsingRE)  # 删除不需要比对的
    dictExpendedJson2 = {}
    if(NliResult2[i][4] == ''): NliResult2[i][4] = '{}'
    expendJson(json.loads(NliResult2[i][4]), '', dictExpendedJson2)
    delNotCompare(dictExpendedJson2, NotCompareKeysUsingRE)
    strOnlyInNliResult1 = getOnlyInTheFirst(dictExpendedJson1, dictExpendedJson2)  # 在NliResult1中出现的
    strOnlyInNliResult2 = getOnlyInTheFirst(dictExpendedJson2, dictExpendedJson1)
    strValueDifferent = getValueDifferent(dictExpendedJson1, dictExpendedJson2)

    if(len(strOnlyInNliResult1) == 0
       and len(strOnlyInNliResult2) == 0
       and len(strValueDifferent) == 0):
        row_out.append('True')
        slots_equal_count += 1
    else:
        row_out.append('False')
        all_equal_flag = False
    row_out.append(strOnlyInNliResult1)
    row_out.append(strOnlyInNliResult2)
    row_out.append(strValueDifferent)

    if(all_equal_flag):
        row_out.append('True')
        all_equal_count += 1
    else:
        row_out.append('False')

    listOutRows.append(row_out)

# 输出云端的格式到csv文件
with open(strCompareResultFileName, 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile)
    spamwriter.writerow(
        ['strAudioPath', 'text',
         'operation1', 'operation2',
         'operation equal?(%%) %d/%d=%.2f%%'
         % (operation_equal_count, len(NliResult1), operation_equal_count * 100 / len(NliResult1)),
         'service1', 'service2',
         'service equal?(%%) %d/%d=%.2f%%'
         % (service_equal_count, len(NliResult1), service_equal_count * 100 / len(NliResult1)),
         'slots1', 'slots2',
         'slots equal?(%%) %d/%d=%.2f%%'
         % (slots_equal_count, len(NliResult1), slots_equal_count * 100 / len(NliResult1)),
         'OnlyInNliResult1', 'OnlyInNliResult2', 'ValueDifferent',
         'all equal?(%%) %d/%d=%.2f%%'
         % (all_equal_count, len(NliResult1), all_equal_count * 100 / len(NliResult1))])
    for line in listOutRows:
        spamwriter.writerow(line)
print("输出和语义引擎的比对结果到文件: %s." % (strCompareResultFileName))
