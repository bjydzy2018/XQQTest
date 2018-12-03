# coding=utf-8

__author__ = 'hcliu'

'''
解析cata响应时间结果。输入测试源结果文件，输出统计的最大值、最小值、平均值。

2017.2.4 创建
2017.2.9 增加打开结果文件时指定编码为gbk
'''


import csv


def parse_time(cata_type, file):
    '''
    统计响应时间。

    param:
        cata_type: cata模式，search或index
        file: 结果文件路径

    return:
        一个包含模式、最大值、最小值、平均值的字典，如:{'search': {'avg': 218.86, 'max': 916.0, 'min': 17.0}}
    '''
    type_ret = {}
    type_ret[cata_type] = []

    with open(file, encoding='gbk') as fp:
        f_csv = csv.DictReader(fp)

        if cata_type == 'index':
            for row in f_csv:
                type_ret[cata_type].append(float(row['index_time']))
        elif cata_type == 'search':
            for row in f_csv:
                type_ret[cata_type].append(float(row['srh_time']))

    ret = {}
    ret[cata_type] = {}
    ret[cata_type]['max'] = max(type_ret[cata_type])
    ret[cata_type]['min'] = min(type_ret[cata_type])
    ret[cata_type]['avg'] = round(
        sum(type_ret[cata_type]) / len(type_ret[cata_type]), 2)

    return ret

if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='解析cata响应时间结果')
    parser.add_argument('-f', '--file', dest='file',
                        action='store', help='需要处理的结果文件')
    parser.add_argument('-t', '--type', dest='type',
                        action='store', help='cata模式，search或index')
    args = parser.parse_args()

    ret = parse_time(args.type, args.file)
    print(ret)
