# coding=utf-8

__author__ = 'hcliu'

'''
自动处理所有测试结果。

2017.2.5 创建
'''

import argparse
import os
import json

parser = argparse.ArgumentParser(description='自动处理所有测试结果')
parser.add_argument('-r', '--path', dest='path',
                    action='store', help='存放测试结果文件的路径')
parser.add_argument('-t', '--tool', dest='tool',
                    action='store', help='存放测试工具的路径')
parser.add_argument('-v', '--version', dest='version',
                    action='store', help='版本号')
parser.add_argument('-j', '--sr_JSONdir', dest='sr_JSONdir', action='store', help='识别标注路径')
parser.add_argument('-u', '--user', dest='user', action='store', help='数据库用户名')
parser.add_argument('-p', '--pwd', dest='pwd', action='store', help='数据库密码')
parser.add_argument('-d', '--DBname', dest='DBname',
                    action='store', help='数据库名')
parser.add_argument('-a', '--host', dest='host', action='store', help='数据库地址')
args = parser.parse_args()

with open('path.cfg', encoding='utf-8') as fp:
    paths = json.load(fp)

def _get_csv_from_dir(dir):
    '''
    获取一个文件夹的csv文件路径，用于提取识别结果文件。应确保目录中只有一个csv文件。

    param:
        dir: 存有csv结果文件的目录。

    return:
        csv文件路径，如果存在多个，会返回读取到的第一个。
    '''
    for root, dirs, files in os.walk(dir):
        for file in files:
            if file.endswith('csv'):
                return os.path.join(root, file)

def handle_cata_time():
    for cata_type in paths['cata_time']:
        for platform in paths['cata_time'][cata_type]:
            os.system('python3 {0} --file={1} --version={2} --type={3} --host={4} --user={5} --pwd={6} --DBname={7} --os={8}'.format(
                args.tool + '/cata响应时间/pack.py', os.path.join(args.path, paths['cata_time'][cata_type][platform]), args.version, cata_type, args.host, args.user, args.pwd, args.DBname, platform))


def handle_tts_time():
    for tts_type in paths['tts_time']:
        for platform in paths['tts_time'][tts_type]:
            os.system('python3 {0} --version={1} --tag={2} --file={3} --host={4} --user={5} --pwd={6} --DBname={7} --os={8}'.format(args.tool + '/合成响应时间/insert_tts_ret_to_database.py', args.version, tts_type+'/no_check', os.path.join(args.path, paths['tts_time'][tts_type][platform]), args.host, args.user, args.pwd, args.DBname, platform))


def handle_mvw():
    for platform in paths['mvw']:
        os.system('python3 {0} --host={1} --user={2} --pwd={3} --DBName={4} --version={5} --os={6} --HandleDir={7}'.format(args.tool + '/批量唤醒/insert_ret_to_database.py', args.host, args.user, args.pwd, args.DBname, args.version, platform, os.path.join(args.path, paths['mvw'][platform])))


def handle_uploaddict_time():
    for dict_type in paths['uploaddict_time']:
        for platform in paths['uploaddict_time'][dict_type]:
            os.system('python3 {0} --os={1} --type={2} --file={3} --host={4} --user={5} --pwd={6} --DBname={7} --version={8}'.format(args.tool + '/上传词典响应时间/pack.py', platform, dict_type, os.path.join(args.path, paths['uploaddict_time'][dict_type][platform]), args.host, args.user, args.pwd, args.DBname, args.version))


def handle_sr_time():
    for platform in paths['sr_time']:
        for lang in paths['sr_time'][platform]:
            for mode in paths['sr_time'][platform][lang]:
                ret_file = _get_csv_from_dir(os.path.join(args.path, paths['sr_time'][platform][lang][mode]))

                os.system('python3 {0} --version={1} --os={2} --lang={3} --mode={4} --file={5} --JSONdir={6} --address={7} --user={8} --pwd={9} --DBname={10} --deadline=0'.format(args.tool + '/识别响应时间/pack_recognition_time.py', args.version, platform, lang, mode, ret_file, args.sr_JSONdir, args.host, args.user, args.pwd, args.DBname))


def handle_sr_rate():
    for platform in paths['sr_rate']:
        for lang in paths['sr_rate'][platform]:
            if lang == 'ch':
                for mode in paths['sr_rate'][platform][lang]:
                    ret_file = _get_csv_from_dir(os.path.join(args.path, paths['sr_rate'][platform][lang][mode]))

                    os.system('python3 {0} --version={1} --os={2} --lang={3} --mode={4} --file={5} --JSONdir={6} --address={7} --user={8} --pwd={9} --DBname={10} --deadline=0'.format(args.tool + '/识别效果/pack_recognition_rate.py', args.version, platform, lang, mode, ret_file, args.sr_JSONdir, args.host, args.user, args.pwd, args.DBname))


if __name__ == '__main__':
    handle_cata_time()
    handle_tts_time()
    handle_mvw()
    handle_uploaddict_time()
    handle_sr_time()
    handle_sr_rate()
