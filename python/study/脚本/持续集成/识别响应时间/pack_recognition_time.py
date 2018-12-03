# coding=utf-8

__author__ = 'hcliu'

'''
将一次识别结果同历史数据进行比较，并生成excel统计表。
'''

import xlwt
import os
from os.path import join, exists
from parse_recognition_time import ParseRecognitionTime
import argparse
import pickle
import sys

parser = argparse.ArgumentParser(description='将一次识别结果同历史数据进行比较，并生成excel统计表。')
parser.add_argument('-v', '--version',
                    dest='version',
                    action='store',
                    help='版本')
parser.add_argument('-o', '--os',
                    dest='os',
                    action='store',
                    help='平台')
parser.add_argument('-l', '--lang',
                    dest='lang',
                    action='store',
                    help='语种')
parser.add_argument('-m', '--mode',
                    dest='mode',
                    action='store',
                    help='识别模式')
parser.add_argument('-t', '--deadline',
                    dest='deadline',
                    action='store',
                    help='指标差值，平均识别响应时间相差超过该值将预警')
parser.add_argument('-f', '--file',
                    dest='file',
                    action='store',
                    help='识别结果文件')
parser.add_argument('-j', '--JSONdir',
                    dest='JSONdir',
                    action='store',
                    help='保存结果pickle文件的目录')
parser.add_argument('-a', '--address',
                    dest='address',
                    action='store',
                    help='数据库地址')
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
args = parser.parse_args()


def parse():
    '''
    解析本次识别结果。
    '''
    parse = ParseRecognitionTime()

    ret = parse.parse_ret(args.file, args.version,
                          args.os, args.lang, args.mode)
    print('读取识别结果{0}条'.format(
        len(parse._ret[args.version][args.os][args.lang][args.mode]['pcms'])))

    parse.write_ret_to_pickle(args.JSONdir)
    print('结果已写入sr_time.pickle')

    parse.write_ret_to_database(
        args.address, args.user, args.pwd, args.DBname)
    print('结果已写入数据库')

    return ret


def is_dead(ret):
    '''
    判断本次识别结果是否达标。

    当前达标标准为：本次识别响应时间和历史版本相差不超过(args.deadline)%
    '''
    with open(join(args.JSONdir, 'sr_time.pickle'), 'rb') as fp:
        old_ret = pickle.load(fp)

        old_rates = {version: float(old_ret[version][os][lang][mode]['avg_time']) for version in old_ret for os in old_ret[version] for lang in old_ret[
            version][os] for mode in old_ret[version][os][lang] if os == args.os and lang == args.lang and mode == args.mode}

        for v in old_rates:
            if (ret[args.version][args.os][args.lang][args.mode]['avg_time'] - old_rates[v]) / old_rates[v] >= float(args.deadline) / 100:

                return v


def _write_dead_to_excel(new_v, new_time, old_v, old_time):
    '''
    将本次数据和历史版本数据写到excel。
    '''
    file = xlwt.Workbook()
    sheet = file.add_sheet('识别响应时间比较')

    sheet.write(0, 0, '版本')
    sheet.write(0, 1, '平台')
    sheet.write(0, 2, '语种')
    sheet.write(0, 3, '模式')
    sheet.write(0, 4, '平均识别响应时间')

    sheet.write(1, 0, new_v)
    sheet.write(1, 1, args.os)
    sheet.write(1, 2, args.lang)
    sheet.write(1, 3, args.mode)
    sheet.write(1, 4, new_time)

    sheet.write(2, 0, old_v)
    sheet.write(2, 1, args.os)
    sheet.write(2, 2, args.lang)
    sheet.write(2, 3, args.mode)
    sheet.write(2, 4, old_time)

    file.save('sr_cmp_ret.xls')


def handle_dead_ret(v):
    '''
    如果本次识别响应时间比历史某个版本低了超过指定值，则调用该函数将两个版本的结果输出。
    '''
    with open(join(args.JSONdir, 'sr_time.pickle'), 'rb') as fp:
        old_ret = pickle.load(fp)

        _write_dead_to_excel(args.version, old_ret[args.version][args.os][args.lang][
                             args.mode]['avg_time'], v, old_ret[v][args.os][args.lang][args.mode]['avg_time'])

if __name__ == '__main__':
    ret = parse()

    v = is_dead(ret)
    if v:
        handle_dead_ret(v)
        sys.exit(1)
