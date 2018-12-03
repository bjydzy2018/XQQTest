# coding=utf-8

__author__ = 'hcliu'

'''
操作mysql数据库的工具库。
'''

import pymysql


def delete_data(db, cursor, table, **fields):
    '''删除已经存在的数据'''
    sql = 'SELECT count(*) FROM {0} WHERE '.format(table)
    first_add = True
    for field in fields:
        if first_add:
            sql += (field + '="{0}"'.format(fields[field]))
            first_add = False
        else:
            sql += (' and ' + field + '="{0}"'.format(fields[field]))

    cursor.execute(sql)
    data_count = int(cursor.fetchone()[0])
    if data_count > 0:
        try:
            sql = 'DELETE FROM {0} WHERE '.format(table)
            first_add = True
            for field in fields:
                if first_add:
                    sql += (field + '="{0}"'.format(fields[field]))
                    first_add = False
                else:
                    sql += (' and ' + field + '="{0}"'.format(fields[field]))

            cursor.execute(sql)
            db.commit()

            return True
        except Exception as err:
            db.rollback()
            raise

    return False


def insert_data(db, cursor, table, **fields):
    '''插入数据'''
    try:
        sql = 'INSERT INTO {0}('.format(table)

        first_add = True
        for field in fields:
            if first_add:
                first_add = False
            else:
                sql += ', '
            sql += field
        sql += ') VALUES ('

        first_add = True
        for field in fields:
            if first_add:
                first_add = False
            else:
                sql += ', '

            sql += (('"' if isinstance(fields[field], str) else '') + str(
                    fields[field]) + ('"' if isinstance(fields[field], str) else ''))
        sql += ')'

        cursor.execute(sql)
        db.commit()

        return True
    except Exception as err:
        db.rollback()
        raise

    return False
