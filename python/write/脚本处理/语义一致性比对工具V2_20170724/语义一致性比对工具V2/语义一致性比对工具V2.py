# coding=utf-8
# python3

import xlrd
import re
import sys
import json
import csv

# 默认参数，当没有命令行传参时 使用这些参数
strNotCompareKeysFileName = '在这里用正则表达式列举不需要参与比对的键.txt'
strBVTFileName = 'BVT.xls'
strNliResultFileName = 'out_cloud_result.csv '
strCompareResultFileName = 'out_cloud_CompareResult.csv'

# 读取命令行参数
if(len(sys.argv) != 1 and len(sys.argv) != 5):
    print('Invalid Arg! len(sys.argv)=%d' % (len(sys.argv)))
    print('Uasge: %s <在这里用正则表达式列举不需要参与比对的键.txt> <BVT.xls> <strNliResultFileName.csv> <strCompareResultFileName.csv>'
          % (sys.argv[0]))
    sys.exit(1)
if(len(sys.argv) == 5):
    strNotCompareKeysFileName = sys.argv[1]
    strBVTFileName = sys.argv[2]
    strNliResultFileName = sys.argv[3]
    strCompareResultFileName = sys.argv[4]

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

# 读取语义结果1
NliResult1 = []
csv_reader = csv.reader(open(strNliResultFileName, mode='r', encoding='gbk'))
rownum_audio_path = 0
rownum_audioTag = 0
rownum_FinalResult0 = 0
for row in csv_reader:
    if(rownum_audio_path == 0 and rownum_audioTag == 0 and rownum_FinalResult0 == 0):
        num = 0
        for line in row:
            if(line == 'audio_path'):
                rownum_audio_path = num
            elif(line == 'audioTag'):
                rownum_audioTag = num
            elif(line == 'FinalResult0'):
                rownum_FinalResult0 = num
            num += 1
    else:
        NliResult1.append([row[rownum_audio_path], row[rownum_audioTag], row[rownum_FinalResult0]])
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
    elif(str(type(jsonObj)) == '<class \'NoneType\'>'):
        dictExpendedJson[prefix[1:]] = "null"
    elif(type(jsonObj) == type([])):
        for i in range(len(jsonObj)):
            expendJson(jsonObj[i], prefix + '[%d]' % (i), dictExpendedJson)
    elif(type(jsonObj) == type({})):
        for (key, value) in jsonObj.items():
            expendJson(value, prefix + '.' + key, dictExpendedJson)
    else:
        print('非法的json对象: %s, type(jsonObj)=%s' % (jsonObj, str(type(jsonObj))))
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

    audio_path = NliResult1[i][0]
    text_0 = ''
    operation = ''
    service = ''
    slots = ''
    if(len(NliResult1[i][2]) > 0):
        json_obj = json.loads(NliResult1[i][2])
        text = json_obj['intent']['text']
        if('operation' in json_obj['intent']): operation = json_obj['intent']['operation']
        if('service' in json_obj['intent']): service = json_obj['intent']['service']
        if('semantic' in json_obj['intent']
           and 'slots' in json_obj['intent']['semantic']):
            slots = json.dumps(json_obj['intent']['semantic'],
                               sort_keys=False, ensure_ascii=False, separators=(', ', ': ')) 

    row_out.append(audio_path)
    row_out.append(text_0)

    row_out.append(operation)
    row_out.append(NliResult2[i][2])
    if(operation == NliResult2[i][2]):
        row_out.append('True')
        operation_equal_count += 1
    else:
        row_out.append('False')
        all_equal_flag = False

    row_out.append(service)
    row_out.append(NliResult2[i][3])
    if(service == NliResult2[i][3]):
        row_out.append('True')
        service_equal_count += 1
    else:
        row_out.append('False')
        all_equal_flag = False

    row_out.append(slots)
    row_out.append(NliResult2[i][4])
    strOnlyInNliResult1 = ''
    strOnlyInNliResult2 = ''
    strValueDifferent = ''

    dictExpendedJson1 = {}
    if(slots == ''): slots = '{}'
    expendJson(json.loads(slots), '', dictExpendedJson1)  # 把json对象 拆成键值对
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
    print("输出和语义引擎的比对结果到文件: %s, all equal?(%%) %d/%d=%.2f%%"
          % (strCompareResultFileName, all_equal_count, len(NliResult1), all_equal_count * 100 / len(NliResult1)))
