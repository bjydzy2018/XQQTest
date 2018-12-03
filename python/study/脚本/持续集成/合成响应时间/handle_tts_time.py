# coding=utf-8

__author__ = 'hcliu'

import re


def handle_tts_time_ret_file(file):
    pattern = re.compile('.* (\d.*) ms')
    ret = {}
    ret['proc_times'] = []

    with open(file, encoding='utf-8', errors='ignore') as fp:
        for line in fp:
            if 'Init resume' in line:
                ret['init_time'] = int(round(float(re.match(pattern, line).group(1)), 0))
            elif 'Create resume' in line:
                ret['create_time'] = int(round(float(re.match(pattern, line).group(1)), 0))
            elif 'tts proc result resume' in line and 'the avg' not in line:
                try:
                    ret['proc_times'].append(int(round(float(re.match(pattern, line).group(1)), 0)))
                except AttributeError:
                    continue

    return ret

if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='统计合成响应时间结果')
    parser.add_argument(dest='filenames', nargs='+', help='需要处理的结果文件，可传入多个')
    args = parser.parse_args()

    with open('ret.txt', 'wt') as fp:
        for file in args.filenames:
            ret = handle_tts_time_ret_file(file)

            print(file, file=fp)
            print('init_time: {0}, create_time: {1}, avg_proc_time: {2}, max_proc_time: {3}, min_proc_time: {4}\n'.format(ret['init_time'], ret[
                  'create_time'], sum(ret['proc_times']) / len(ret['proc_times']), max(ret['proc_times']), min(ret['proc_times'])), file=fp)
