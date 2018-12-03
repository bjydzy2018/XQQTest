# coding=utf-8

__author__ = 'hcliu'

import xlrd
import xlwt3
import re
import argparse
from collections import defaultdict
import dig2cn


def get_table_names(sheet, table_row=0):
    '''
    获取一个excel sheet的列名和列号字典。
    '''
    table_names = {}

    for idx, val in enumerate(sheet.row_values(table_row)):
        table_names[val] = idx

    return table_names


def get_sheet_data(sheet, label_cols=(0,), table_names = None, data_first_row=1):
    '''
    获取sheet数据。

    params:
        sheet: 要处理的sheet对象
        table_names: sheet中的数据（表名：列号）字典
        data_first_row: 数据开始的行号
        label_cols: 数据标记列
    '''
    data = defaultdict(dict)

    # 构建（列号：表名）字典
    reverse_table_names = {}
    for idx, table_name in zip(table_names.values(), table_names.keys()):
        reverse_table_names[idx] = table_name

    not_label_names = [reverse_table_names[not_label_col]
                       for not_label_col in reverse_table_names if not_label_col not in label_cols]
    label_names = [reverse_table_names[label_col] for label_col in label_cols]

    for row in range(data_first_row, sheet.nrows):
        if not_label_names:
            for not_label_name in not_label_names:
                label_names_list = data[tuple([sheet.cell(row, table_names[label_name]).value for label_name in label_names])][
                    not_label_name] = sheet.cell(row, table_names[not_label_name]).value
        else:
            data[tuple([sheet.cell(row, table_names[label_name]
                                   ).value for label_name in label_names])] = None

    return data


def _dig2ch(dig):
    digs = {}
    digs['1'] = '一'
    digs['2'] = '二'
    digs['3'] = '三'
    digs['4'] = '四'
    digs['5'] = '五'
    digs['6'] = '六'
    digs['7'] = '七'
    digs['8'] = '八'
    digs['9'] = '九'
    digs['0'] = '零'
    digs['.'] = '点'

    ch = ''
    for d in dig:
        if d in digs:
            ch += digs[d]
        else:
            ch += d

    return ch


def _dig2ch2(dig):
    ch = ''
    import re
    pattern = re.compile('\w*?(\d+)\w*')
    match = re.match(pattern, dig)
    if match:
        ch = re.sub(
            '\d+', dig2cn.translate_number_to_chinese(int(match.group(1))), dig)
    else:
        ch = dig

    return ch


def dig_handle_dict(d):
    new_d = {}

    for key in d:
        if(key[1] in ['cmd', 'weather', 'flight', 'train', 'airControl', 'itemSelect', 'calc']):
            new_key = _dig2ch2(key[0])
            new_d[(new_key, key[1])] = d[key]
        else:
            new_key = _dig2ch(key[0])
            new_d[(new_key, key[1])] = d[key]

    return new_d

if __name__ == '__main__':
    import pprint

    parser = argparse.ArgumentParser(description='比较新旧语料，提取出相同语料的标注和不同的语料')

    parser.add_argument('-f', '--file', dest='source_file',
                        required=True, action='store', help='需要处理的源文件')
    parser.add_argument('-o', '--old', dest='old_sheet',
                        required='True', action='store', help='老说法sheet名')
    parser.add_argument('-n', '--new', dest='new_sheet',
                        required='True', action='store', help='新说法sheet名')
    args = parser.parse_args()

    data = xlrd.open_workbook(args.source_file)
    old_sheet = data.sheet_by_name(args.old_sheet)
    new_sheet = data.sheet_by_name(args.new_sheet)

    old_data = get_sheet_data(
        old_sheet, get_table_names(old_sheet), label_cols=(0, 1))
    print('old_sheet table names:', get_table_names(old_sheet))
    print('old_data len:', len(old_data))

    new_data = get_sheet_data(
        new_sheet, get_table_names(new_sheet), label_cols=(0, 1))
    print('new_sheet table names:', get_table_names(new_sheet))
    print('new_data len:', len(new_data))

    new_old_data = dig_handle_dict(old_data)
    print('new_old_data len:', len(new_old_data))

    # 写结果到文件
    file = xlwt3.Workbook()
    old_ret_sheet = file.add_sheet('带标注')
    new_ret_sheet = file.add_sheet('不带标注')

    row = 1
    old_ret_sheet.write(0, 0, '查询词')
    old_ret_sheet.write(0, 1, '标注业务')
    old_ret_sheet.write(0, 2, '标注结果')
    old_ret_sheet.write(0, 3, '标注操作')

    for has_label in (new_old_data.keys() & new_data.keys()):
        old_ret_sheet.write(row, 0, has_label[0])
        old_ret_sheet.write(row, 1, has_label[1])
        old_ret_sheet.write(row, 2, new_old_data[has_label]['标注结果'])
        old_ret_sheet.write(row, 3, new_old_data[has_label]['标注操作'])

        row += 1

    row = 1
    new_ret_sheet.write(0, 0, '查询词')
    new_ret_sheet.write(0, 1, '标注业务')
    for no_label in (new_data.keys() - new_old_data.keys()):
        new_ret_sheet.write(row, 0, no_label[0])
        new_ret_sheet.write(row, 1, no_label[1])

        row += 1

    file.save('ret.xls')
