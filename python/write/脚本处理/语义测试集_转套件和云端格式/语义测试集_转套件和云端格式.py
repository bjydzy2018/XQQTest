# coding=utf-8
# python3

import sys
import xlrd
import json
import csv

# 默认参数，当没有命令行传参时 使用这些参数
strNliTestSetFileName = '离线效果测试验收集.xlsx'
strNliTestSetSheetName = '600通全业务验收集'
strAiuiTestSetOutputFileName = '600通全业务验收集_套件格式.csv'
strCloudSrTestSetOutputFileName = '600通全业务验收集_云端格式.csv'

# 读取命令行参数
if(len(sys.argv) != 1 and len(sys.argv) != 5):
    print('Invalid Arg! len(sys.argv)=%d' % (len(sys.argv)))
    print('Uasge: %s <strNliTestSetFileName.xlsx> <strNliTestSetSheetName> <strAiuiTestSetOutputFileName.csv> <strCloudSrTestSetOutputFileName.csv>' % (sys.argv[0]))
    sys.exit(1)
if(len(sys.argv) == 5):
    strNliTestSetFileName = sys.argv[1]
    strNliTestSetSheetName = sys.argv[2]
    strAiuiTestSetOutputFileName = sys.argv[3]
    strCloudSrTestSetOutputFileName = sys.argv[4]


# 打开excel
book = xlrd.open_workbook(strNliTestSetFileName)
sheet = book.sheet_by_name(strNliTestSetSheetName)

# 查找'用户说法' 和 'user_data'列
user_saying_col = -1
user_data_col = -1
for col in range(sheet.ncols):
    if(sheet.cell_value(rowx=0, colx=col) == '用户说法'):
        user_saying_col = col
    if(sheet.cell_value(rowx=0, colx=col) == 'user_data'):
        user_data_col = col
if(user_saying_col == -1):
    print('Error: 输入的excel中 没有找到\'用户说法\'列')
    sys.exit(1)
if(user_data_col == -1):
    print('Error: 输入的excel中 没有找到\'user_data\'列')
    sys.exit(1)

# 提取'用户说法' 和 'user_data'列
NliTestSet = []
for row in range(1, sheet.nrows):
    user_saying = sheet.cell_value(rowx=row, colx=user_saying_col)
    user_data = sheet.cell_value(rowx=row, colx=user_data_col)
    NliTestSet.append([user_saying, user_data])
print("Found %d TestCases." % (len(NliTestSet)))


def find_hotInfo_viewCmd(obj_user_data_org):
    if('viewCmd' in obj_user_data_org):
        return obj_user_data_org['viewCmd']
    if(type(obj_user_data_org) == type({})):
        for value in obj_user_data_org.values():
            if(type(value) == type({})):
                ret = find_hotInfo_viewCmd(value)
                if(len(ret) > 0):
                    return ret
    return ''
# 转换成套件的格式
AiuiTestSet = []
for some_nli_test_set in NliTestSet:
    obj_user_data_org = json.loads(some_nli_test_set[1])
    str_hotInfo_viewCmd = find_hotInfo_viewCmd(obj_user_data_org)
    str_hotInfo_viewCmds = str_hotInfo_viewCmd.split(sep='|')
    user_data_aiui = {}
    user_data_aiui['UserData'] = obj_user_data_org
    user_data_aiui['HotWords'] = str_hotInfo_viewCmds
    AiuiTestSet.append(
        [some_nli_test_set[0],
         json.dumps(user_data_aiui, sort_keys=False, ensure_ascii=False, separators=(',', ':'))])
# 输出套件的格式到csv文件
with open(strAiuiTestSetOutputFileName, 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile)
    spamwriter.writerow(['用户说法', 'user_data'])
    for line in AiuiTestSet:
        spamwriter.writerow(line)
print("Output Aiui Format TestCase To File: %s." % (strAiuiTestSetOutputFileName))


# 转换成云端识别的格式
CloudSrTestSet = []
for some_nli_test_set in NliTestSet:
    obj_user_data_org = json.loads(some_nli_test_set[1])
    cloud_sr_data = find_hotInfo_viewCmd(obj_user_data_org)
    CloudSrTestSet.append([some_nli_test_set[0], cloud_sr_data])
# 输出云端的格式到csv文件
with open(strCloudSrTestSetOutputFileName, 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile)
    spamwriter.writerow(['用户说法', 'user_data'])
    for line in CloudSrTestSet:
        spamwriter.writerow(line)
print("Output CloudSr Format TestCase To File: %s." % (strCloudSrTestSetOutputFileName))
