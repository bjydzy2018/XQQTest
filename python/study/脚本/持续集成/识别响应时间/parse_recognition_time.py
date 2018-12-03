# coding=utf-8

__author__ = 'hcliu'

'''
解析识别结果文件，提取识别响应时间。
'''

import csv
import pickle
import os
from os.path import join, exists
import re
from DBEarrow import delete_data, insert_data
import pymysql


class ParseRecognitionTime:
    def __init__(self):
        # 识别响应时间结果
        self._ret = {}

    def _update_ret_dict(self, old, new):
        for version in old:
            if version not in new:
                new[version] = {}

            for os in old[version]:
                if os not in new[version]:
                    new[version][os] = {}

                for language in old[version][os]:
                    if language not in new[version][os]:
                        new[version][os][language] = {}

                    for mode in old[version][os][language]:
                        if mode not in new[version][os][language]:
                            new[version][os][language][mode] = {}
                            new[version][os][language][mode]['pcms'] = {}

                        for pcm_path in old[version][os][language][
                                mode]['pcms']:
                            new[version][os][language][
                                mode]['pcms'][pcm_path] = {}

                            if 'sr_time' in old[version][os][language][mode]['pcms'][pcm_path]:
                                new[version][os][language][mode]['pcms'][pcm_path]['sr_time'] = old[
                                    version][os][language][mode]['pcms'][pcm_path]['sr_time']

                            if 'speechend_time' in old[version][os][language][mode]['pcms'][pcm_path]:
                                new[version][os][language][mode]['pcms'][pcm_path]['speechend_time'] = old[
                                    version][os][language][mode]['pcms'][pcm_path]['speechend_time']

                        new[version][os][language][mode]['avg_time'] = old[version][os][language][
                            mode]['avg_time']
                        new[version][os][language][mode]['avg_speechend_time'] = old[version][os][language][
                            mode]['avg_speechend_time']

    def write_ret_to_pickle(self, JSONdir):
        '''
        将识别响应时间数据写入pickle文件，相同条件的会覆盖。
        '''
        pickle_name = 'sr_time.pickle'
        pickle_path = join(JSONdir, pickle_name)
        pickle_ret = {}

        if exists(pickle_path):
            with open(pickle_path, 'rb') as fp:
                pickle_ret = pickle.load(fp)

        self._update_ret_dict(self._ret, pickle_ret)

        with open(pickle_path, 'wb') as fp:
            pickle.dump(pickle_ret, fp)

    def write_ret_to_database(self, address, user, pwd, DBname):
        '''
        将识别响应时间数据写入数据库，相同条件的会覆盖。
        '''
        db = pymysql.connect(address, user, pwd, DBname, charset='utf8')
        cursor = db.cursor()

        # 删除相同版本和平台的老数据
        for version in self._ret:
            for os in self._ret[version]:
                for language in self._ret[version][os]:
                    for mode in self._ret[version][os][language]:
                        if delete_data(db, cursor, 'sr_time', version=version, os=os, language=language, mode=mode):
                            delete_data(
                                db, cursor, 'sr_avg_time', version=version, os=os, language=language, mode=mode)
                            print(
                                '{0}-{1}-{2}-{3} 数据已存在，删除成功'.format(version, os, language, mode))

        # 插入数据
        print('\n开始插入数据库...')
        for version in self._ret:
            for os in self._ret[version]:
                for language in self._ret[version][os]:
                    for mode in self._ret[version][os][language]:
                        for pcm_path in self._ret[version][os][language][mode]['pcms']:
                            insert_data(db, cursor, 'sr_time', version=version, os=os, language=language, mode=mode,
                                        pcm_path=pcm_path, sr_time=self._ret[version][os][language][mode]['pcms'][pcm_path]['sr_time'] if 'sr_time' in self._ret[version][os][language][mode]['pcms'][pcm_path] else -1, speechend_time=self._ret[version][os][language][mode]['pcms'][pcm_path]['speechend_time'] if 'speechend_time' in self._ret[version][os][language][mode]['pcms'][pcm_path] else -1)

                        insert_data(db, cursor, 'sr_avg_time', version=version, os=os, language=language, mode=mode, sr_avg_time=self._ret[
                            version][os][language][mode]['avg_time'], speechend_avg_time=self._ret[version][os][language][mode]['avg_speechend_time'])

        db.close()

    def _handle_ret_pcm_path(self, path, language):
        '''
        从结果的音频路径中提取测试集相对路径，去除开头的绝对路径。
        '''
        if language == 'ch':
            return path.split('400')[1].replace('\\', '/')

    def parse_ret(self, ret_path, version, os, language, mode):
        '''
        解析识别结果。

        params:
            ret_path: 结果文件路径
            version: 结果版本
            os: 结果平台
            language: 结果语种
            mode: 该次结果的识别模式
        '''
        ret = {}

        with open(ret_path, encoding='gbk', errors='ignore') as fp:
            reader = csv.reader(fp)
            first_line = True

            ret[version] = {}
            ret[version][os] = {}
            ret[version][os][language] = {}
            ret[version][os][language][mode] = {}
            ret[version][os][language][mode]['pcms'] = {}

            sr_times = []
            speechend_times = []

            for line in reader:
                if not first_line:
                    try:
                        pcm_path = self._handle_ret_pcm_path(line[0], language)
                        if os != 'android':
                            sr_time = int(line[8])
                            speechend_time = int(line[4])
                        else:
                            sr_time = int(line[6])
                            speechend_time = int(line[2])

                        if sr_time > 0 and sr_time < 20000:
                            ret[version][os][language][
                                mode]['pcms'][pcm_path] = {}
                            ret[version][os][language][mode]['pcms'][
                                pcm_path]['sr_time'] = sr_time
                            sr_times.append(sr_time)

                        if speechend_time > 0 and speechend_time < 20000:
                            if pcm_path not in ret[version][os][language][mode]['pcms']:
                                ret[version][os][language][
                                    mode]['pcms'][pcm_path] = {}

                            ret[version][os][language][mode]['pcms'][
                                pcm_path]['speechend_time'] = speechend_time
                            speechend_times.append(speechend_time)

                    except Exception as err:
                        # 结果文件最后三行是识别响应时间的统计
                        print(err)

                first_line = False

            ret[version][os][language][mode][
                'avg_time'] = sum(sr_times) / len(sr_times)
            ret[version][os][language][mode][
                'avg_speechend_time'] = sum(speechend_times) / len(speechend_times)

        self._update_ret_dict(ret, self._ret)

        return ret

if __name__ == '__main__':
    import pprint
    parse = ParseRecognitionTime()

    ret = parse.parse_ret('efficient_QNX.csv', '1078', 'qnx650', 'ch', 'local')
    print(len(parse._ret['1078']['qnx650']['ch']['local']))

    ret = parse.parse_ret('android.csv', '1078', 'android', 'ch', 'local')
    print(len(parse._ret['1078']['android']['ch']['local']))

    parse.write_ret_to_pickle('.')
    parse.write_ret_to_database('192.168.24.45', 'root', '', 'test')
