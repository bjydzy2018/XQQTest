# coding=utf-8

'''
对话引擎上传词典响应时间统计。
'''

__author__ = 'hcliu'

import json
import sys
from collections import defaultdict
import pprint
import xlwt

file_path = sys.argv[1]
ret = defaultdict(dict)

with open(file_path, encoding='utf-8') as fp:
    for line in fp:
        line = line.strip().replace('\\', '/')

        try:
            dict_ret = json.loads(line)

            for dict_name in dict_ret:
                dict_time = dict_ret[dict_name]

                for dict_time_label in dict_time:
                    if dict_time_label not in ret[dict_name]:
                        ret[dict_name][dict_time_label] = []
                    ret[dict_name][dict_time_label].append(
                        dict_time[dict_time_label])

        except json.decoder.JSONDecodeError:
            # 文件里的空行会解析出错
            pass

file = xlwt.Workbook()
table = file.add_sheet('上传词典时间统计')

origin_col = 0
origin_row = 0
for dict_name in ret:
    col = origin_col
    row = origin_row
    table.write(origin_row, origin_col, dict_name.split('/')[-1])

    for dict_time_label in ret[dict_name]:
        col += 1
        table.write(origin_row, col, dict_time_label)

        times = ret[dict_name][dict_time_label]

        temp_sum_time = 0
        for time in times:
            row += 1
            # 如果是第一次写数据，还要写入次数
            if list(ret[dict_name].keys()).index(dict_time_label) == 0:
                table.write(row, origin_col, '第{0}次'.format(row))

            table.write(row, col, str(time))
            temp_sum_time += time

        if list(ret[dict_name].keys()).index(dict_time_label) == 0:
            table.write(row + 1, origin_col, '平均值'.format(row))
        table.write(row + 1, col, temp_sum_time / row)

        row = origin_row

    origin_col += 5

file.save('statistical_{0}.xls'.format(
    (file_path.replace('\\', '/').split('/')[-1]).split('.')[0]))
