# coding=utf-8

__author__ = 'hcliu'

'''
获取多个词典上传后的资源变化大小。
'''

from compare_upload_rom import get_dict_compare_ret
import argparse
import xlwt
import os

parser = argparse.ArgumentParser(description='获取多个词典上传后的资源变化大小')
parser.add_argument('-o', '--old', dest='old',
                    action='store', help='原始资源路径，传入bin目录')
parser.add_argument('-n', '--new', dest='new',
                    nargs='+', help='上传词典后的资源路径，可传入多个')
args = parser.parse_args()

file = xlwt.Workbook()
table = file.add_sheet('上传词典rom变化统计')

# 每个词典结果矩阵的左上格坐标
origin_col = 0
origin_row = 0
for new_path in args.new:
    col = origin_col
    row = origin_row

    dict_name = new_path.replace('\\', os.path.sep).replace('/', os.path.sep).split(os.path.sep)[-1]

    ret = get_dict_compare_ret(args.old, new_path)

    if ret:
        for key in ret:
            table.write(row, col + 1, key)
            table.write(row + 1, col + 1, str(ret[key][0]))
            table.write(row + 2, col + 1, str(ret[key][1]))
            table.write(row + 3, col + 1, str(ret[key][2]))

            col += 1

        table.write(row, origin_col, dict_name)
        table.write(row + 1, origin_col, '原始大小')
        table.write(row + 2, origin_col, '上传后大小')
        table.write(row + 3, origin_col, '上传后增幅')

        origin_row += 5
    else:
        print('{0} 不存在！'.format(dict_name))

file.save('upload_dict_ret.xls')
