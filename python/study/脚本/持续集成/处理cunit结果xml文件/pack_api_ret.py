# coding=utf-8

__author__ = 'hcliu'

'''
将指定版本失败的用例统计到excel表中。

统计时会同时和历史数据进行对比，并将每个版本都失败的用例和历史版本有成功过的失败用例分开输出。
'''

import argparse
import xlwt
import os
import pickle
from collections import defaultdict

parser = argparse.ArgumentParser(description='接口测试结果写入excel表')
parser.add_argument('-o', '--os',
                    dest='os',
                    action='store',
                    help='系统平台')
parser.add_argument('-v', '--version',
                    dest='version',
                    action='store',
                    help='版本')
parser.add_argument('-m', '--module',
                    dest='module',
                    action='store',
                    help='模块')
parser.add_argument('-j', '--JSONdir',
                    dest='JSONdir',
                    action='store',
                    help='保存json结果文件的目录')
parser.add_argument('-r', '--RetDir',
                    dest='RetDir',
                    action='store',
                    help='保存excel结果文件的目录')
args = parser.parse_args()

ret_name = 'api_ret.pickle'
ret_path = os.path.join(args.JSONdir, ret_name)
ret = pickle.load(open(ret_path, 'rb'))
cur_ret = ret[args.module][args.os][args.version]

success_versions = defaultdict(list)

for version in ret[args.module][args.os]:
    if version != args.version:
        for fail_case in cur_ret['failure_records']:
            if fail_case not in ret[args.module][args.os][version]['failure_records']:
                success_versions[fail_case].append(version)

file = xlwt.Workbook()
sheet_successed = None
if success_versions:
    sheet_successed = file.add_sheet('新增未通过用例')
sheet = file.add_sheet('未通过用例')

row = 0
row_successed = 0

if success_versions:
    sheet_successed.write(row_successed, 0, '统计')
    sheet_successed.write(row_successed, 1, '总数')
    sheet_successed.write(row_successed, 2, '运行')
    sheet_successed.write(row_successed, 3, '成功')
    sheet_successed.write(row_successed, 4, '失败')

    row_successed += 1
    sheet_successed.write(row_successed, 0, '用例')
    sheet_successed.write(row_successed, 1, cur_ret['sum_records'][0][0])
    sheet_successed.write(row_successed, 2, cur_ret['sum_records'][0][1])
    sheet_successed.write(row_successed, 3, cur_ret['sum_records'][0][2])
    sheet_successed.write(row_successed, 4, cur_ret['sum_records'][0][3])

    row_successed += 1
    sheet_successed.write(row_successed, 0, '断言')
    sheet_successed.write(row_successed, 1, cur_ret['sum_records'][1][0])
    sheet_successed.write(row_successed, 2, cur_ret['sum_records'][1][1])
    sheet_successed.write(row_successed, 3, cur_ret['sum_records'][1][2])
    sheet_successed.write(row_successed, 4, cur_ret['sum_records'][1][3])

    row_successed += 2
    sheet_successed.write(row_successed, 0, '新增失败用例统计')

    row_successed += 1
    sheet_successed.write(row_successed, 0, '用例')
    sheet_successed.write(row_successed, 1, '文件名')
    sheet_successed.write(row_successed, 2, '历史成功版本')
    sheet_successed.write(row_successed, 3, '行号')
    sheet_successed.write(row_successed, 4, '断言条件')

sheet.write(row, 0, '失败用例统计')

row += 1
sheet.write(row, 0, '用例')
sheet.write(row, 1, '文件名')
sheet.write(row, 2, '行号')
sheet.write(row, 3, '断言条件')

for failure_record in cur_ret['failure_records']:
    if success_versions and failure_record in success_versions:
        row_successed += 1

        sheet_successed.write(row_successed, 0, failure_record)
        sheet_successed.write(row_successed, 1, cur_ret['failure_records']
                              [failure_record]['FILE_NAME'])
        sheet_successed.write(
            row_successed, 2, str(success_versions[failure_record]))

        for line, cond in cur_ret['failure_records'][failure_record]['CONDITION']:
            sheet_successed.write(row_successed, 3, line)
            sheet_successed.write(row_successed, 4, cond)

            row_successed += 1
    else:
        row += 1

        sheet.write(row, 0, failure_record)
        sheet.write(row, 1, cur_ret['failure_records']
                    [failure_record]['FILE_NAME'])

        for line, cond in cur_ret['failure_records'][failure_record]['CONDITION']:
            sheet.write(row, 2, line)
            sheet.write(row, 3, cond)

            row += 1

file.save(os.path.join(args.RetDir, '{0}接口测试结果.xls'.format(args.module)))
