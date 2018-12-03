# coding=utf-8

__author__ = 'hcliu'

'''
将cata响应时间处理后的结果插入数据库。

2017.2.4 创建
'''

import pymysql
import argparse
from parse import parse_time
import DBEarrow

parser = argparse.ArgumentParser(description='统计cata响应时间结果')
parser.add_argument('-f', '--file', dest='file',
                    action='store', help='需要处理的结果文件')
parser.add_argument('-t', '--type', dest='type',
                    action='store', help='cata模式，search或index')
parser.add_argument('-o', '--os', dest='os', action='store', help='系统平台')
parser.add_argument('-v', '--version', dest='version',
                    action='store', help='版本号')
parser.add_argument('-u', '--user', dest='user', action='store', help='数据库用户名')
parser.add_argument('-p', '--pwd', dest='pwd', action='store', help='数据库密码')
parser.add_argument('-d', '--DBname', dest='DBname',
                    action='store', help='数据库名')
parser.add_argument('-a', '--host', dest='host', action='store', help='数据库地址')
args = parser.parse_args()

db = pymysql.connect(args.host, args.user, args.pwd,
                     args.DBname, charset='utf8')
cursor = db.cursor()

ret = parse_time(args.type, args.file)

if DBEarrow.delete_data(db, cursor, 'cata_time', os=args.os, version=args.version, type=args.type):
    print('{0}-{1}-{2} 数据已存在，删除成功'.format(args.version, args.os, args.type))

print('\n开始插入数据库...')
DBEarrow.insert_data(db, cursor, 'cata_time', os=args.os, version=args.version, type=args.type, max=ret[
            args.type]['max'], min=ret[args.type]['min'], avg=ret[args.type]['avg'])
print('{0}-{1}-{2} 数据插入成功'.format(args.version, args.os, args.type))

db.close()
