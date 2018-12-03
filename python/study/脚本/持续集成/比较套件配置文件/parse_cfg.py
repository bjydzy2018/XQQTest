# coding=utf-8

__author__ = 'hcliu'

'''
解析单个配置文件，插入数据库和pickle文件。
'''

import json
import argparse
import collections
import pickle
import pymysql
from DBEarrow import delete_data, insert_data
import os


class CfgParse:
    def __init__(self, cfg_file, _os, version, module, tag):
        self._cfg_file_path = cfg_file
        self._cfg = None
        if os.path.exists(cfg_file):
            self._cfg = json.load(open(cfg_file, encoding='utf-8'), encoding='utf-8')
        self._cfg_content = collections.defaultdict(list)
        self.os = _os
        self.version = version
        self.module = module
        self.tag = tag

    def _parse_cfg(self, obj, path):
        '''
        params:
            _cfg_content: 保存各配置项信息的字典
            obj: 读取的对象
            path: 拼接的各配置项名称
        '''
        if isinstance(obj, str) or isinstance(obj, int):
            self._cfg_content[path].append(str(obj))
        elif isinstance(obj, dict):
            for key in obj:
                if not key.endswith('Doc'):
                    self._parse_cfg(obj[key], '{0}/{1}'.format(path, key))
        elif isinstance(obj, list):
            for item in obj:
                self._parse_cfg(item, path)
        else:
            print('检测到未处理类型：' + path)
            print(obj, type(obj))

    def gen(self):
        if self._cfg:
            self._parse_cfg(self._cfg, '')
            return self._cfg_content
        else:
            return None

    def insert_into_database(self, address, user, pwd, DBname):
        db = pymysql.connect(address, user,
                             pwd, DBname, charset='utf8')
        cursor = db.cursor()

        if delete_data(db, cursor, 'cfg', tag=self.tag, version=self.version, module=self.module, os=self.os):
            print('{0}配置数据已存在，删除成功'.format(self.tag))

        print('{0}配置数据开始插入数据库...'.format(self.tag))
        if self._cfg:
            for key in self._cfg_content:
                insert_data(db, cursor, 'cfg',
                            tag=self.tag,
                            version=self.version,
                            module=self.module,
                            os=self.os,
                            cfg_label=key,
                            cfg_value=self._cfg_content[key].replace('\\', '\\\\') if not isinstance(
                                self._cfg_content[key], list) else (','.join(self._cfg_content[key]).replace('\\', '\\\\')),
                            file_path=self._cfg_file_path.replace('\\', '\\\\'))
            print('{0}配置数据插入数据库成功\n'.format(self.tag))
        else:
            print('{0}配置文件不存在\n'.format(self.tag))
        db.close()

    def insert_into_pickle(self, JSONdir):
        ret_name = 'cfg.pickle'
        ret_path = os.path.join(JSONdir, ret_name)

        ret = {}
        if not os.path.exists(ret_path):
            ret[self.module] = {}
            ret[self.module][self.os] = {}
            ret[self.module][self.os][self.version] = {}
            ret[self.module][self.os][self.version][self.tag] = {}
        else:
            with open(ret_path, 'rb') as fp:
                ret = pickle.load(fp)

            if self.module not in ret:
                ret[self.module] = {}
            if self.os not in ret[self.module]:
                ret[self.module][self.os] = {}
            if self.version not in ret[self.module][self.os]:
                ret[self.module][self.os][self.version] = {}
            if self.tag not in ret[self.module][self.os][self.version]:
                ret[self.module][self.os][self.version][self.tag] = {}

        ret[self.module][self.os][self.version][
            self.tag]['cfg'] = self._cfg_content
        with open(ret_path, 'wb') as fp:
            pickle.dump(ret, fp)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='解析某个配置文件的相关信息')
    parser.add_argument('-o', '--os',
                        dest='os',
                        action='store',
                        help='系统平台')
    parser.add_argument('-v', '--version',
                        dest='version',
                        action='store',
                        help='版本')
    parser.add_argument('-m', '--module',
                        dest='module',
                        action='store',
                        help='模块')
    parser.add_argument('-f', '--file',
                        dest='file',
                        action='store',
                        help='配置文件')
    parser.add_argument('-t', '--tag',
                        dest='tag',
                        action='store',
                        help='区分配置文件的标签，不同目录下的配置文件名可能相同')
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

    gen = CfgParse(args.file, args.os, args.version,
                   args.module, args.tag)
    data = gen.gen()
    gen.insert_into_pickle(args.JSONdir)
    gen.insert_into_database(args.address, args.user, args.pwd, args.DBname)
