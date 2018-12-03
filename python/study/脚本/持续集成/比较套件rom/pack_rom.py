# coding=utf-8

__author__ = 'hcliu'

'''
读取某个版本的资源文件大小并和上一个版本做比较，生成excel统计表。
'''

import argparse
from DBEarrow import delete_data, insert_data
import pickle
import pymysql
from parse_rom import ParseRom
import os
from os.path import join, exists
import xlwt
import sys

parser = argparse.ArgumentParser(
    description='读取某个版本的资源文件大小并和上一个版本做比较，生成excel统计表。')
parser.add_argument('-v', '--version',
                    dest='version',
                    action='store',
                    help='版本')
parser.add_argument('-c', '--cmpVersion',
                    dest='cmpVersion',
                    action='store',
                    help='比对基线版本')
parser.add_argument('-j', '--JSONdir',
                    dest='JSONdir',
                    action='store',
                    help='保存json结果文件的目录')
parser.add_argument('-r', '--releaseDir',
                    dest='releaseDir',
                    action='store',
                    help='07.Release目录的路径')
parser.add_argument('-e', '--retPath',
                    dest='retPath',
                    action='store',
                    help='结果生成路径')
parser.add_argument('-u', '--user',
                    dest='user',
                    action='store',
                    help='数据库用户名')
parser.add_argument('-p', '--pwd',
                    dest='pwd',
                    action='store',
                    help='数据库密码')
parser.add_argument('-d', '--DBname',
                    dest='DBname',
                    action='store',
                    help='数据库名')
parser.add_argument('-a', '--address',
                    dest='address',
                    action='store',
                    help='数据库地址')
args = parser.parse_args()

# 解析资源文件
parseRom = ParseRom(args.version, args.releaseDir)
ret = parseRom.parse()

# 插入数据库
db = pymysql.connect(args.address, args.user, args.pwd,
                     args.DBname, charset='utf8')
cursor = db.cursor()
if delete_data(db, cursor, 'rom', version=args.version):
    print('版本数据已存在，删除成功')

print('开始插入数据库...')

for module in ret:
    for path in ret[module]:
        insert_data(db, cursor, 'rom',
                    version=args.version,
                    module=module,
                    path=path.replace('\\', '\\\\'),
                    size=ret[module][path])
print('数据插入成功\n')

# 写入pickle
pickle_name = 'rom.pickle'
pickle_path = join(args.JSONdir, pickle_name)
pickle_ret = {}

if exists(pickle_path):
    with open(pickle_path, 'rb') as fp:
        pickle_ret = pickle.load(fp)

pickle_ret[args.version] = ret
with open(pickle_path, 'wb') as fp:
    pickle.dump(pickle_ret, fp)

# 历史比对
cmp_ret = pickle_ret[args.cmpVersion]

from collections import defaultdict
not_equle = defaultdict(list)
more = defaultdict(list)
less = defaultdict(list)

for module in ret:
    for key in ret[module].keys() & cmp_ret[module].keys():
        if ret[module][key] != cmp_ret[module][key]:
            not_equle[module].append(
                (key, str(ret[module][key]), str(cmp_ret[module][key])))

    for key in ret[module].keys() - cmp_ret[module].keys():
        more[module].append((key, str(ret[module][key]), '0'))

    for key in cmp_ret[module].keys() - ret[module].keys():
        less[module].append((key, '0', str(cmp_ret[module][key])))


def write_diff_to_sheet(sheet, diff):
    '''
    资源比较结果写入excel
    '''
    row = 0
    sheet.write(row, 0, '模块')
    sheet.write(row, 1, 'path')
    sheet.write(row, 2, args.version)
    sheet.write(row, 3, args.cmpVersion)

    for module in diff:
        row += 1
        sheet.write(row, 0, module)

        for data in diff[module]:
            sheet.write(row, 1, data[0])
            sheet.write(row, 2, data[1])
            sheet.write(row, 3, data[2])

            row += 1

if not_equle or more or less or parseRom.res_version_correct:
    file = xlwt.Workbook()

    if not_equle:
        sheet = file.add_sheet('大小不一致项')
        write_diff_to_sheet(sheet, not_equle)
    if more:
        sheet = file.add_sheet('增加项')
        write_diff_to_sheet(sheet, more)
    if less:
        sheet = file.add_sheet('减少项')
        write_diff_to_sheet(sheet, less)
    if parseRom.res_version_correct:
        sheet = file.add_sheet('res_version版本不一致')
        sheet.write(0, 0, 'res_version文件路径')
        row = 1
        for path in parseRom.res_version_correct:
            sheet.write(row, 0, path)
            row += 1

    if args.retPath:
        file.save(join(args.retPath, 'rom_cmp.xls'))
    else:
        file.save('rom_cmp.xls')
    sys.exit(1)
