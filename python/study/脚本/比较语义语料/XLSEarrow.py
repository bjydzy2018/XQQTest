#coding=utf-8

__author__ = 'hcliu'

'''
处理excel文件的通用库。
'''

import xlrd
from collections import defaultdict

def get_table_names(sheet, table_row=0):
    '''
    获取一个excel sheet的列名和列号字典。
    '''
    table_names = {}

    for idx, val in enumerate(sheet.row_values(table_row)):
        table_names[val] = idx

    return table_names


def get_sheet_data(sheet, table_names, data_first_row=1, label_cols=(0,)):
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