  # coding=utf-8
# python3

import sys
import csv
import xlrd
import re

# 默认参数，当没有命令行传参时 使用这些参数
strResultFileName = './out_mix_result.csv'
strBiaozhuFileName = './离线效果测试验收集_用于20170707语义套件一致性对比.xlsx'
strOutputEffectReportFileName = './out_mix_result_effect_report.csv'

# 读取命令行参数
if(len(sys.argv) != 1 and len(sys.argv) != 4):
    print('Invalid Arg! len(sys.argv)=%d' % (len(sys.argv)))
    print('Usage: %s <strResultFileName> <离线效果测试验收集_用于20170707语义套件一致性对比.xlsx> <strOutputEffectReportFileName>' % (sys.argv[0]))
    sys.exit(1)
if(len(sys.argv) == 4):
    # 如果传入了命令行参数，就按照命令行参数为准
    strResultFileName = sys.argv[1]
    strBiaozhuFileName = sys.argv[2]
    strOutputEffectReportFileName = sys.argv[3]

# 读取识别结果
ListSrResult = []
csv_reader = csv.reader(open(strResultFileName, mode='r', encoding='gbk'))
FinalResultText0_rownum = 0
FinalResultText1_rownum = 0
for row in csv_reader:
    if(FinalResultText0_rownum == 0 and FinalResultText1_rownum == 0):
        if(row[2] == 'FinalResultText0' and row[3] == 'FinalResultText1'):
            FinalResultText0_rownum = 2
            FinalResultText1_rownum = 3
        elif(row[6] == 'FinalResultText0' and row[8] == 'FinalResultText1'):
            FinalResultText0_rownum = 6
            FinalResultText1_rownum = 8
        else:
            print('%s格式错误，没有找到 FinalResultText0 和 FinalResultText1 列.' % (strOutputEffectReportFileName))
            sys.exit(1)
        continue
    dictRecord = {}
    dictRecord['audio_path'] = row[0]
    dictRecord['audioTag'] = row[1]
    dictRecord['ResultText'] = "%s%s" % (row[FinalResultText0_rownum], row[FinalResultText1_rownum])
    ListSrResult.append(dictRecord.copy())
print("获取到%d条识别结果" % (len(ListSrResult)))

# 读取识别结果标注
ListSrResultLabel = []
book = xlrd.open_workbook(strBiaozhuFileName)
sheet = book.sheet_by_name("600通全业务验收集")
for row in range(1, sheet.nrows):
    ListSrResultLabel.append(sheet.cell_value(rowx=row, colx=5))
print("获取到%d条识别结果标注" % (len(ListSrResultLabel)))

# 检查识别和标注数目 是否对得上
if(len(ListSrResult) != len(ListSrResultLabel)):
    print("识别结果和标注数目对不上，请查证!")
    sys.exit(1)

# 比对识别结果和标注，输出报表 到csv格式的文件中
num_right = 0
outputs = []
for num in range(0, len(ListSrResult)):
    audio_path = ListSrResult[num]['audio_path']
    audioTag = ListSrResult[num]['audioTag']
    ResultOrgText = ListSrResult[num]['ResultText']
    ResultCompareText = re.sub("[，。？！：　,.?!: ]", "", ResultOrgText).lower()
    ResultOrgLabel = ListSrResultLabel[num]
    ResultCompareLabel = re.sub("[，。？！：　,.?!: ]", "", ResultOrgLabel).lower()
    IsEqual = ''
    if(ResultCompareText == ResultCompareLabel):
        IsEqual = 'TRUE'
        num_right += 1
    else:
        IsEqual = 'FALSE'
    outputs.append([audio_path, audioTag, ResultOrgText, ResultCompareText, ResultOrgLabel, ResultCompareLabel, IsEqual])
with open(strOutputEffectReportFileName, 'w', newline='') as csvfile:
    spamwriter = csv.writer(csvfile)
    spamwriter.writerow(['audio_path', 'audioTag',
                         'ResultOrgText', 'ResultCompareText',
                         'ResultOrgLabel', 'ResultCompareLabel',
                         'IsEqual?(%%) %d/%d=%.2f%%' % (num_right, len(ListSrResult), num_right * 100 / len(ListSrResult))])
    print("句识别率(%%): %d/%d=%.2f%%" % (num_right, len(ListSrResult), num_right * 100 / len(ListSrResult)))
    for line in outputs:
        spamwriter.writerow(line)
