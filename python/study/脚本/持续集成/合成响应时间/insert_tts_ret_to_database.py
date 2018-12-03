# coding=utf-8

__author__ = 'hcliu'

import pymysql
import argparse
from handle_tts_time import handle_tts_time_ret_file

parser = argparse.ArgumentParser(description='将合成响应时间的结果插入数据库')
parser.add_argument('-f', '--file', dest='file',
                    action='store', help='需要处理的结果文件')
parser.add_argument('-o', '--os', dest='os', action='store', help='系统平台')
parser.add_argument('-v', '--version', dest='version',
                    action='store', help='版本号')
parser.add_argument('-t', '--tag', dest='tag', action='store', help='标签')
parser.add_argument('-u', '--user', dest='user', action='store', help='数据库用户名')
parser.add_argument('-p', '--pwd', dest='pwd', action='store', help='数据库密码')
parser.add_argument('-d', '--DBname', dest='DBname',
                    action='store', help='数据库名')
parser.add_argument('-a', '--host', dest='host', action='store', help='数据库地址')
args = parser.parse_args()

host = args.host
user = args.user
pwd = args.pwd
db_name = args.DBname
version = args.version
file = args.file
os = args.os
tag = args.tag

db = pymysql.connect(host, user, pwd, db_name, charset='utf8')
cursor = db.cursor()


def delete_existed_data(table, **fields):
    # 删除已经存在的数据
    sql = 'SELECT count(*) FROM {0} WHERE '.format(table)
    first_add = True
    for field in fields:
        if first_add:
            sql+=(field+'="{0}"'.format(fields[field]))
            first_add = False
        else:
            sql+=(' and '+field+'="{0}"'.format(fields[field]))

    cursor.execute(sql)
    data_count = int(cursor.fetchone()[0])
    if data_count > 0:
        print('%s表中 os:%s, version:%s, tag:%s 数据已存在，开始删除...' % (table, os, version, tag))

        try:
            sql = 'DELETE FROM {0} WHERE '.format(table)
            first_add = True
            for field in fields:
                if first_add:
                    sql+=(field+'="{0}"'.format(fields[field]))
                    first_add = False
                else:
                    sql+=(' and '+field+'="{0}"'.format(fields[field]))

            cursor.execute(sql)
            db.commit()
            print('删除成功')
        except Exception as err:
            print('删除失败:', err)
            db.rollback()

delete_existed_data('tts_init_time_ret', os=os, version=version, tag=tag)
delete_existed_data('tts_proc_time_ret', os=os, version=version, tag=tag)

ret = handle_tts_time_ret_file(file)
print('数据读取成功，开始插入数据库...')

sql_init = 'INSERT INTO tts_init_time_ret(version, os, init_time, create_time, tag) VALUES ("{0}","{1}",{2},{3},"{4}")'.format(
    version, os, ret['init_time'], ret['create_time'], tag)

try:
    cursor.execute(sql_init)
    db.commit()
except Exception as err:
    print('插入数据失败:', sql_init)
    print(err)
    db.rollback()

for proc_time in ret['proc_times']:
    sql_proc = 'INSERT INTO tts_proc_time_ret(version, os, proc_time, tag) VALUES ("{0}","{1}",{2}, "{3}")'.format(
        version, os, proc_time, tag)

    try:
        cursor.execute(sql_proc)
        db.commit()
    except Exception as err:
        print('插入数据失败:', sql_proc)
        print(err)
        db.rollback()
