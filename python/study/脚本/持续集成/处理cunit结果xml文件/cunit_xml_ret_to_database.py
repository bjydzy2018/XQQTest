# coding=utf-8

__author__ = 'hcliu'

'''
将handle_cunit_xml_ret脚本处理后的结果插入数据库,并在指定位置生成结果字典pickle文件。
'''

import pymysql
import argparse
from handle_cunit_xml_ret_etree import get_failure_records, get_sum_records
from DBEarrow import delete_data, insert_data

parser = argparse.ArgumentParser(description='将接口测试结果插入数据库')
parser.add_argument('-f', '--file',
                    dest='file',
                    action='store',
                    help='需要处理的结果文件')
parser.add_argument('-o', '--os',
                    dest='os',
                    action='store',
                    help='系统平台')
parser.add_argument('-v', '--version',
                    dest='version',
                    action='store',
                    help='版本号')
parser.add_argument('-m', '--module',
                    dest='module',
                    action='store',
                    help='模块')
parser.add_argument('-j', '--JSONdir',
                    dest='JSONdir',
                    action='store',
                    help='保存json结果文件的目录')
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
parser.add_argument('-a', '--address',
                    dest='address',
                    action='store',
                    help='数据库地址')
args = parser.parse_args()

db = pymysql.connect(args.address, args.user, args.pwd,
                     args.DBname, charset='utf8')
cursor = db.cursor()

failure_records = get_failure_records(args.file)
sum_records = get_sum_records(args.file)

# 插入数据库
for case_name in failure_records:
    if delete_data(db, cursor, 'api_fail_ret', case_name=case_name, version=args.version, module=args.module):
        print('{0}用例数据已存在，删除成功'.format(case_name))

    print('{0}用例开始插入数据库...\n'.format(case_name))
    for line_number, condition in failure_records[case_name]['CONDITION']:
        insert_data(db, cursor, 'api_fail_ret', case_name=case_name, version=args.version, module=args.module, os=args.os,
                    line_number=line_number, cond=condition, file_name=failure_records[case_name]['FILE_NAME'])

db.close()

# 写入json结果文件
import os
import pickle
ret_name = 'api_ret.pickle'
ret_path = os.path.join(args.JSONdir, ret_name)

ret = {}
if (not os.path.exists(ret_path)) or os.path.getsize(ret_path) == 0:
    ret[args.module] = {}
    ret[args.module][args.os] = {}
    ret[args.module][args.os][args.version] = {}
else:
    ret = pickle.load(open(ret_path, 'rb'))

    if args.module not in ret:
        ret[args.module] = {}
    if args.os not in ret[args.module]:
        ret[args.module][args.os] = {}
    if args.version not in ret[args.module][args.os]:
        ret[args.module][args.os][args.version] = {}

ret[args.module][args.os][args.version]['failure_records'] = failure_records
ret[args.module][args.os][args.version]['sum_records'] = sum_records
pickle.dump(ret, open(ret_path, 'wb'))
