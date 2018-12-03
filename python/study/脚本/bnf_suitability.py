#coding = utf-8

'''
检测bnf语法和功能说法列表的匹配度。

运行版本：python 3.X

运行参数：说法excel文件路径、说法在excel中的sheet名、bnf说法目录
其中『bnf说法目录』为bnf_analyzer分析bnf文件后生成的目录；支持一次传入多个目录。
如：python3 bnf_suitability.py 柳汽SX5说法录音.xls all Call Other

运行结果：命令行输出bnf语法和功能说法的匹配度；脚本目录生成『不包含说法.txt』文件，其中包含
所有bnf语法中缺乏的说法列表
'''
__author__ = 'hcliu'

import sys
import xlrd
import pprint
import os
import re
import dig2cn

label = sys.argv[1]
table_name = sys.argv[2]
bnf_dirs = sys.argv[3:]

def get_bnf_type_from_dir():
    bnf_types = []

    for bnf_dir in bnf_dirs:
        for root,dirs,files in os.walk(bnf_dir):
            for file in files:
                if not file.endswith('report.txt'):
                    print('读取：',file)

                    with open(os.path.join(root, file)) as fp:
                        for line in fp:
                            bnf_types.append(line.strip())

    return bnf_types

def get_label():
    labels = []

    data = xlrd.open_workbook(label)
    table = data.sheet_by_name(table_name)

    pattern = re.compile('\w*?(\d+)\w*')

    for row in range(1, table.nrows):
        value = table.col(1)[row].value
        match = re.match(pattern, value)
        if match:
            value = re.sub('\d+', dig2cn.translate_number_to_chinese(int(match.group(1))), value)

        labels.append(value)

    return labels

if __name__ == '__main__':
    labels = get_label()
    bnf_types = get_bnf_type_from_dir()

    sum_num = 0.0
    has_num = 0.0


    with open('不包含说法.txt', 'w') as fp:
        for label in labels:
            sum_num += 1

            if label in bnf_types:
                has_num += 1
            else:
                fp.write(label+'\n')

        fp.write('\n匹配度：'+str(has_num/sum_num*100)+'%')

    print('匹配度：',str(has_num/sum_num*100)+'%')

