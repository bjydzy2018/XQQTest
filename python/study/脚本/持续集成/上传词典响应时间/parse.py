# coding=utf-8

__author__ = 'hcliu'

'''
统计上传词典响应时间。

2017.2.4 创建
'''

import re
from collections import defaultdict


def _parse_android_contact(file):
    '''
    解析安卓平台联系人词典时间。
    '''
    pattern_level = r'.*dict_M-(.*)\.txt'
    pattern_local_time = r'local time: (.*)'
    pattern_cloud_time = r'cloud time: (.*)'

    ret = defaultdict(dict)

    with open(file, encoding='utf-8') as fp:
        level = None
        local_time = None
        cloud_time = None

        for line in fp:
            line = line.strip()

            if 'dict' in line:
                level = re.match(pattern_level, line).group(1)
            if 'local time' in line:
                local_time = re.match(pattern_local_time, line).group(1)
                ret[level]['local'] = local_time
            if 'cloud time' in line:
                cloud_time = re.match(pattern_cloud_time, line).group(1)
                ret[level]['cloud'] = cloud_time

    return ret


def _parse_other_contact(file):
    '''
    解析非安卓平台联系人词典。
    '''
    pattern_level = r'.*/isr/dict/(.*)\.json'
    pattern_local_time = r'.*init_upload_dict time = (.*)'

    ret = defaultdict(dict)

    with open(file, encoding='utf-8') as fp:
        level = None
        local_time = None
        cloud_time = None

        for line in fp:
            line = line.strip()

            if 'Open dict_Dir success' in line:
                level = re.match(pattern_level, line).group(1)
            if 'ISSSRUpLoadDict to local success' in line:
                local_time = re.match(pattern_local_time, line).group(1)
                ret[level]['local'] = local_time

    return ret


def parse(os, file, dict_type):
    if dict_type == 'contact':
        if os == 'android':
            return _parse_android_contact(file)
        else:
            return _parse_other_contact(file)

if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='解析上传词典响应时间结果')
    parser.add_argument('-f', '--file', dest='file',
                        action='store', help='需要处理的结果文件')
    parser.add_argument('-o', '--os', dest='os',
                        action='store', help='系统')
    parser.add_argument('-t', '--type', dest='type',
                        action='store', help='词典类型')
    args = parser.parse_args()

    ret = parse(args.os, args.file, args.type)
    print(ret)
