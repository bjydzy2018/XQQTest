#coding=utf-8
__author__ = 'hcliu'

import pymysql
import sys
import optparse
from handle_mvw_ret import handle_ret

parser = optparse.OptionParser()

parser.add_option("-t","--host",
                    dest="host",
                    type="string",
                    help='host')

parser.add_option("-u","--user",
                    dest="user",
                    type="string",
                    help='user')

parser.add_option("-p","--pwd",
                    dest="pwd",
                    type="string",
                    help='pwd')

parser.add_option("-n","--DBName",
                    dest="DBName",
                    type="string",
                    help='DBName')

parser.add_option("-v","--version",
                    dest="version",
                    type="string",
                    help='version')

parser.add_option("-d","--HandleDir",
                    dest="HandleDir",
                    type="string",
                    help='HandleDir')

parser.add_option("-o","--os",
                    dest="os",
                    type="string",
                    help='os')

options, args = parser.parse_args(sys.argv[1:])

host = options.host
user = options.user
pwd = options.pwd
db_name = options.DBName
version = options.version
handle_dir = options.HandleDir
os = options.os

db = pymysql.connect(host, user, pwd, db_name, charset='utf8')
cursor = db.cursor()

def delete_existed_data(table, os, version):
    # 删除已经存在的数据
    cursor.execute('SELECT count(*) FROM %s WHERE os="%s" and version="%s"' % (table, os, version))
    data_count = int(cursor.fetchone()[0])
    if data_count>0:
        print('%s表中 os:%s, version:%s 数据已存在，开始删除...' % (table, os, version))

        try:
            cursor.execute('DELETE FROM %s WHERE os="%s" and version="%s"' % (table, os, version))
            db.commit()
            print('删除成功')
        except Exception as err:
            print('删除失败:',err)
            db.rollback()

delete_existed_data('MVW_RET_SUM', os, version)
delete_existed_data('MVW_RET', os, version)

ret = handle_ret(handle_dir)
print('唤醒数据读取成功，开始插入数据库...')

for scene in ret['scenes']:
    for vw_word in ret['scenes'][scene]:
        for pcm_path in ret['scenes'][scene][vw_word]['vwRet']:
            sql = None

            if ret['scenes'][scene][vw_word]['vwRet'][pcm_path]:
                nStartBytes = ret['scenes'][scene][vw_word]['vwRet'][pcm_path]["nStartBytes"]
                nEndBytes = ret['scenes'][scene][vw_word]['vwRet'][pcm_path]["nEndBytes"]
                nMvwScene = ret['scenes'][scene][vw_word]['vwRet'][pcm_path]["nMvwScene"]
                nMvwId = ret['scenes'][scene][vw_word]['vwRet'][pcm_path]["nMvwId"]
                nMvwScore = ret['scenes'][scene][vw_word]['vwRet'][pcm_path]["nMvwScore"]

                sql_insert_ret = 'INSERT INTO MVW_RET(pcm_path, nStartBytes, nEndBytes, nMvwScene, nMvwId, nMvwScore, version, os, vwWordLabel, sceneLabel) VALUES ("%s",%d,%d,%d,%d,%d,"%s","%s","%s", %d)' % (pcm_path, int(nStartBytes), int(nEndBytes), int(nMvwScene), int(nMvwId), int(nMvwScore), version, os, vw_word, int(scene))
            else:
                sql_insert_ret = 'INSERT INTO MVW_RET(pcm_path, version, os, vwWordLabel, sceneLabel) VALUES ("%s","%s","%s","%s", %d)' % (pcm_path, version, os, vw_word, int(scene))

            try:
                cursor.execute(sql_insert_ret)
                db.commit()
            except Exception as err:
                print('插入数据失败:',sql_insert_ret,err)
                db.rollback()

        sum_num = int(ret['scenes'][scene][vw_word]['vwTry_time'])
        success_num = int(ret['scenes'][scene][vw_word]['vwSuccess_time'])
        success_rate = ret['scenes'][scene][vw_word]['vwSuccess_rate']
        vw_time = ret['scenes'][scene][vw_word]['vwTime']

        sql_insert_sum = 'INSERT INTO MVW_RET_SUM VALUES(%d,"%s",%d,%d,"%s","%s","%s","%s")' % (int(scene), vw_word, sum_num, success_num, success_rate, vw_time, os, version)

        try:
            cursor.execute(sql_insert_sum)
            db.commit()
            print('version:%s,os:%s,scene:%s,word:%s 数据插入成功' % (version, os, scene, vw_word))
        except Exception as err:
            print('插入数据失败:',sql_insert_sum,err)
            db.rollback()