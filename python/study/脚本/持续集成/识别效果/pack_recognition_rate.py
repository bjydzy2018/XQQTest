# coding=utf-8

__author__ = 'hcliu'

'''
将一次识别结果同历史数据进行比较，并生成excel统计表。

2017.1.6 增加预警条件：识别率低于0.65
    excel输出增加本次测试全部识别错误结果
    增加输出文件目录配置

2017.1.9 增加英文识别预警条件：识别率低于0.35
'''

import xlwt
import os
from os.path import join, exists
from parse_recognition_rate import ParseRecognitionRate
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
                    help='识别率指标差值，识别率相差超过该值将输出差异结果')
parser.add_argument('-f', '--file',
                    dest='file',
                    action='store',
                    help='识别结果文件')
parser.add_argument('-j', '--JSONdir',
                    dest='JSONdir',
                    action='store',
                    help='保存标注、结果pickle文件的目录')
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
parser.add_argument('-e', '--retPath',
                    dest='retPath',
                    action='store',
                    help='结果生成路径')
args = parser.parse_args()


def parse():
    '''
    解析本次识别结果。
    '''
    parse = ParseRecognitionRate()

    ret = parse.parse_ret(args.file, args.version,
                          args.os, args.lang, args.mode)
    print('读取识别结果{0}条'.format(
        len(parse._ret[args.version][args.os][args.lang][args.mode])))

    parse.write_ret_to_pickle(args.JSONdir)
    print('结果已写入sr_ret.pickle')

    cmp_ret = parse.compare(args.JSONdir)
    print('比较结束，错误结果{0}条，识别正确率{1}'.format(len(cmp_ret[args.version][args.os][args.lang][
          args.mode]['wrong_pcms']), cmp_ret[args.version][args.os][args.lang][args.mode]['sr_rate']))

    parse.write_cmp_ret_to_pickle(args.JSONdir)
    print('比较结果已写入sr_wrong.pickle')
    parse.write_cmp_ret_to_database(
        args.address, args.user, args.pwd, args.DBname)
    print('比较结果已写入数据库')

    return ret, cmp_ret


def is_dead(cmp_ret):
    '''
    判断本次识别结果是否达标。

    当前达标标准为：本次识别率和历史识别率相差不超过args.deadline
    '''
    with open(join(args.JSONdir, 'sr_wrong.pickle'), 'rb') as fp:
        old_ret = pickle.load(fp)

        old_rates = {version: float(old_ret[version][os][lang][mode]['sr_rate']) for version in old_ret for os in old_ret[version] for lang in old_ret[
            version][os] for mode in old_ret[version][os][lang] if os == args.os and lang == args.lang and mode == args.mode}

        for v in old_rates:
            # 和历史版本比较
            print('old:', old_rates[v], 'cur:', cmp_ret[args.version][
                  args.os][args.lang][args.mode]['sr_rate'])
            if (old_rates[v] - cmp_ret[args.version][args.os][args.lang][args.mode]['sr_rate']) != 0 and (old_rates[v] - cmp_ret[args.version][args.os][args.lang][args.mode]['sr_rate']) >= float(args.deadline) / 100:
                print('识别率低于历史')
                return v

            if (args.lang=='ch' and cmp_ret[args.version][args.os][args.lang][args.mode]['sr_rate'] <= 0.65) or (args.lang=='en' and cmp_ret[args.version][args.os][args.lang][args.mode]['sr_rate'] <= 0.2):
                print('识别率过低')
                return args.version


def _write_dead_to_excel(pcms, cmp_ret):
    '''
    将本次错误的结果写到excel。
    '''
    file = xlwt.Workbook()

    # 历史结果正确但本次错误
    sheet = file.add_sheet('本版本新增识别错误')

    row = 0
    sheet.write(row, 0, 'pcm')
    sheet.write(row, 1, '识别结果')
    if args.lang == 'ch':
        sheet.write(row, 2, '本地标注')
        sheet.write(row, 3, '云端标注')

        for pcm in pcms:
            row += 1

            sheet.write(row, 0, pcm)
            sheet.write(row, 1, cmp_ret[args.version][args.os][
                        args.lang][args.mode]['wrong_pcms'][pcm][0])
            sheet.write(row, 2, cmp_ret[args.version][args.os][
                        args.lang][args.mode]['wrong_pcms'][pcm][1])
            sheet.write(row, 3, cmp_ret[args.version][args.os][
                        args.lang][args.mode]['wrong_pcms'][pcm][2])

    elif args.lang == 'en':
        sheet.write(row, 2, '标注')

        for pcm in pcms:
            row += 1

            sheet.write(row, 0, pcm)
            sheet.write(row, 1, cmp_ret[args.version][args.os][
                        args.lang][args.mode]['wrong_pcms'][pcm][0])
            sheet.write(row, 2, cmp_ret[args.version][args.os][
                        args.lang][args.mode]['wrong_pcms'][pcm][1])

    # 本次错误
    sheet = file.add_sheet('本版本识别错误')

    row = 0
    sheet.write(row, 0, 'pcm')
    sheet.write(row, 1, '识别结果')

    if args.lang == 'ch':
        sheet.write(row, 2, '本地标注')
        sheet.write(row, 3, '云端标注')

        for pcm in cmp_ret[args.version][args.os][args.lang][args.mode]['wrong_pcms']:
            row += 1

            sheet.write(row, 0, pcm)
            sheet.write(row, 1, cmp_ret[args.version][args.os][
                        args.lang][args.mode]['wrong_pcms'][pcm][0])
            sheet.write(row, 2, cmp_ret[args.version][args.os][
                        args.lang][args.mode]['wrong_pcms'][pcm][1])
            sheet.write(row, 3, cmp_ret[args.version][args.os][
                        args.lang][args.mode]['wrong_pcms'][pcm][2])

    elif args.lang == 'en':
        sheet.write(row, 2, '标注')

        for pcm in cmp_ret[args.version][args.os][args.lang][args.mode]['wrong_pcms']:
            row += 1

            sheet.write(row, 0, pcm)
            sheet.write(row, 1, cmp_ret[args.version][args.os][
                        args.lang][args.mode]['wrong_pcms'][pcm][0])
            sheet.write(row, 2, cmp_ret[args.version][args.os][
                        args.lang][args.mode]['wrong_pcms'][pcm][1])

    if args.retPath:
        file.save(join(args.retPath, 'sr_cmp_ret.xls'))
    else:
        file.save('sr_cmp_ret.xls')


def handle_dead_ret(v, cmp_ret):
    '''
    如果本次识别率比历史某个版本低了超过指定值，则调用该函数将历史结果正确但本次错误的结果和
    本次错误的全部结果输出。
    '''
    with open(join(args.JSONdir, 'sr_wrong.pickle'), 'rb') as fp:
        old_ret = pickle.load(fp)

        _write_dead_to_excel(cmp_ret[args.version][args.os][args.lang][args.mode]['wrong_pcms'].keys(
        ) - old_ret[v][args.os][args.lang][args.mode]['wrong_pcms'].keys(), cmp_ret)

if __name__ == '__main__':
    ret, cmp_ret = parse()

    v = is_dead(cmp_ret)
    if v:
        handle_dead_ret(v, cmp_ret)
        sys.exit(1)
