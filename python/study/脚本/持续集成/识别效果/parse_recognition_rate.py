# coding=utf-8

__author__ = 'hcliu'

'''
解析识别标注和结果文件。
'''

import csv
import pickle
import os
from os.path import join, exists
import re
from DBEarrow import delete_data, insert_data
import pymysql


class ParseRecognitionRate:
    def __init__(self):
        # 标注
        self._label = {}
        # 识别结果
        self._ret = {}
        # 错误结果
        self._cmp_ret = {}

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
                            if 'wrong_pcms' in old[version][os][language][mode]:
                                new[version][os][language][
                                    mode]['wrong_pcms'] = {}

                        if 'wrong_pcms' in old[version][os][language][mode]:
                            for pcm_path in old[version][os][language][
                                    mode]['wrong_pcms']:
                                new[version][os][language][mode]['wrong_pcms'][pcm_path] = old[version][os][language][
                                    mode]['wrong_pcms'][pcm_path]

                            new[version][os][language][mode]['sr_rate'] = old[version][os][language][
                                mode]['sr_rate']

                            new[version][os][language][mode]['sr_rate_by_service'] = old[version][os][language][mode]['sr_rate_by_service']
                        else:
                            for pcm_path in old[version][os][language][
                                    mode]:
                                new[version][os][language][mode][pcm_path] = old[version][os][language][
                                    mode][pcm_path]

    def write_label_to_pickle(self, JSONdir):
        '''
        将标注数据写入pickle文件，相同语种的会覆盖。
        '''
        pickle_name = 'sr_label.pickle'
        pickle_path = join(JSONdir, pickle_name)
        pickle_ret = {}

        if exists(pickle_path) and os.path.getsize(pickle_path) > 0:
            with open(pickle_path, 'rb') as fp:
                pickle_ret = pickle.load(fp)

        pickle_ret.update(self._label)

        with open(pickle_path, 'wb') as fp:
            pickle.dump(pickle_ret, fp)

    def write_ret_to_pickle(self, JSONdir):
        '''
        将识别结果数据写入pickle文件，相同条件的会覆盖。
        '''
        pickle_name = 'sr_ret.pickle'
        pickle_path = join(JSONdir, pickle_name)
        pickle_ret = {}

        if exists(pickle_path) and os.path.getsize(pickle_path) > 0:
            with open(pickle_path, 'rb') as fp:
                pickle_ret = pickle.load(fp)

        self._update_ret_dict(self._ret, pickle_ret)

        with open(pickle_path, 'wb') as fp:
            pickle.dump(pickle_ret, fp)

    def write_cmp_ret_to_pickle(self, JSONdir):
        '''
        将识别比对结果数据写入pickle文件，相同条件的会覆盖。
        '''
        pickle_name = 'sr_wrong.pickle'
        pickle_path = join(JSONdir, pickle_name)
        pickle_ret = {}

        if exists(pickle_path) and os.path.getsize(pickle_path) > 0:
            with open(pickle_path, 'rb') as fp:
                pickle_ret = pickle.load(fp)

        self._update_ret_dict(self._cmp_ret, pickle_ret)

        with open(pickle_path, 'wb') as fp:
            pickle.dump(pickle_ret, fp)

    def write_cmp_ret_to_database(self, address, user, pwd, DBname):
        '''
        将识别比对结果数据写入数据库，相同条件的会覆盖。
        '''
        db = pymysql.connect(address, user, pwd, DBname, charset='utf8')
        cursor = db.cursor()

        # 删除相同版本和平台的老数据
        for version in self._cmp_ret:
            for os in self._cmp_ret[version]:
                for language in self._cmp_ret[version][os]:
                    for mode in self._cmp_ret[version][os][language]:
                        if delete_data(db, cursor, 'sr_cmp', version=version, os=os, language=language, mode=mode):
                            print('{0}-{1}-{2}-{3} 数据已存在，删除成功'.format(version, os, language, mode))

        # 插入数据
        print('\n开始插入数据库...')
        for version in self._cmp_ret:
            for os in self._cmp_ret[version]:
                for language in self._cmp_ret[version][os]:
                    for mode in self._cmp_ret[version][os][language]:
                        for pcm_path in self._cmp_ret[version][os][language][mode]['wrong_pcms']:
                            if language == 'ch':
                                insert_data(db, cursor, 'sr_cmp', version=version, os=os, language=language, mode=mode, pcm_path=pcm_path, ret=self._cmp_ret[version][os][language][mode]['wrong_pcms'][
                                            pcm_path][0], label_local=self._cmp_ret[version][os][language][mode]['wrong_pcms'][pcm_path][1], label_cloud=self._cmp_ret[version][os][language][mode]['wrong_pcms'][pcm_path][2])
                            elif language == 'en':
                                insert_data(db, cursor, 'sr_cmp', version=version, os=os, language=language, mode=mode, pcm_path=pcm_path, ret=self._cmp_ret[
                                            version][os][language][mode]['wrong_pcms'][pcm_path][0], label_en=self._cmp_ret[version][os][language][mode]['wrong_pcms'][pcm_path][1])

                        delete_data(db, cursor, 'sr_rate', version=version,os=os, language=language, mode=mode)
                        insert_data(db, cursor, 'sr_rate', version=version, os=os, language=language, mode=mode, rate='%.2f' % (float(
                            self._cmp_ret[version][os][language][mode]['sr_rate']) * 100))

                        delete_data(db, cursor, 'sr_rate_by_service', version=version,os=os, language=language, mode=mode)
                        
                        for service in self._cmp_ret[version][os][language][mode]['sr_rate_by_service']:
                            insert_data(db, cursor, 'sr_rate_by_service', version=version, os=os, language=language, mode=mode, service=service, rate='%.2f' % (float(
                            self._cmp_ret[version][os][language][mode]['sr_rate_by_service'][service]) * 100))

        db.close()

    def _handle_label_pcm_path(self, path, language):
        '''
        从标注的音频路径中提取测试集相对路径，去除开头的绝对路径。
        '''
        if language == 'ch':
            return path.split('chinese')[1].replace('\\', '/')
        elif language == 'en':
            # return path
            return path.lower().split('english')[1].replace('\\', '/')

    def _handle_ret_pcm_path(self, path, language):
        '''
        从结果的音频路径中提取测试集相对路径，去除开头的绝对路径。
        '''
        if language == 'ch':
            return path.split('chinese')[1].replace('\\', '/')
        elif language == 'en':
            return path.lower().split('english')[1].replace('\\', '/')

    def parse_label(self, label_path, language):
        '''
        解析中、英文识别标注文件。中文区分本地、云端标注，英文只有一个标注。

        params:
            label_path: 标注文件路径
            language: 标注的语种
        '''
        label = {}

        with open(label_path) as fp:
            reader = csv.reader(fp)
            label[language] = {}
            first_line = True

            if language == 'ch':
                for line in reader:
                    if not first_line:
                        pcm_path = self._handle_label_pcm_path(line[0], 'ch')
                        label[language][pcm_path] = {}
                        label[language][pcm_path]['cloud'] = line[2]
                        label[language][pcm_path]['local'] = line[3]

                        # 业务
                        service = line[1]
                        if service != '':
                            if 'service' not in label[language]:
                                label[language]['service'] = {}
                            if service not in label[language]['service']:
                                label[language]['service'][service] = set()

                            label[language]['service'][service].add(pcm_path)

                    first_line = False
            elif language == 'en':
                for line in reader:
                    if not first_line:
                        pcm_path = self._handle_label_pcm_path(line[2], 'en')
                        label[language][pcm_path] = line[3]

                        service = line[1]
                        if service != '':
                            if 'service' not in label[language]:
                                label[language]['service'] = {}
                            if service not in label[language]['service']:
                                label[language]['service'][service] = set()

                            label[language]['service'][service].add(pcm_path)

                    first_line = False

        for language in label:
            self._label[language] = label[language]

        return label

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

        with open(ret_path, encoding='gbk') as fp:
            reader = csv.reader(fp)
            first_line = True

            ret[version] = {}
            ret[version][os] = {}
            ret[version][os][language] = {}
            ret[version][os][language][mode] = {}

            for line in reader:
                if not first_line:
                    try:
                        pcm_path = self._handle_ret_pcm_path(line[0], language)
                        if os != 'android':
                            ret[version][os][language][
                                mode][pcm_path] = line[11]
                        else:
                            ret[version][os][language][
                                mode][pcm_path] = line[9]
                    except Exception as err:
                        # 结果文件最后三行是识别响应时间的统计
                        print(err)

                first_line = False

        self._update_ret_dict(ret, self._ret)

        return ret

    def _compare(self, ret, label, language):
        '''
        对识别结果进行处理后同标注进行比较。
        '''
        if language == 'ch':
            # 去除结尾的.
            if ret.endswith('.'):
                ret = ret[:-1]

            # 去除文本中的，,。?？!！
            pattern = re.compile('[，,。?？!！]')
            ret = re.sub(pattern, '', ret)

            # 去除文本中的空格
            ret = ret.replace(' ', '')
        elif language == 'en':
            # 去除结尾的.
            if ret.endswith('.'):
                ret = ret[:-1]

            # 去除文本中的,，
            pattern = re.compile('[,，]')
            ret = re.sub(pattern, '', ret)

            # 去除文本中的。
            ret = ret.replace(' 。', '')

        ret = ret.strip()
        ret = ret.lower()
        label = label.lower()
        return ret == label

    def compare(self, label_JSONdir):
        '''
        将对象保存的所有结果同标注进行比较，返回本次比较结果。
        '''
        label = pickle.load(
            open(join(label_JSONdir, 'sr_label.pickle'), 'rb'))

        for version in self._ret:
            if version not in self._cmp_ret:
                self._cmp_ret[version] = {}

            for _os in self._ret[version]:
                if _os not in self._cmp_ret[version]:
                    self._cmp_ret[version][_os] = {}

                for language in self._ret[version][_os]:
                    if language not in self._cmp_ret[version][_os]:
                        self._cmp_ret[version][_os][language] = {}

                    for mode in self._ret[version][_os][language]:
                        if mode not in self._cmp_ret[version][_os][language]:
                            self._cmp_ret[version][_os][language][mode] = {}
                            self._cmp_ret[version][_os][
                                language][mode]['wrong_pcms'] = {}

                        for pcm_path in self._ret[version][_os][language][mode]:
                            if language == 'ch':
                                if not self._compare(self._ret[version][_os][language][mode][pcm_path], label[language][pcm_path]['cloud'], language) and not self._compare(self._ret[version][_os][language][mode][pcm_path], label[language][pcm_path]['local'], language):
                                    # 输出结果为（识别结果，本地标注，云端标注）
                                    self._cmp_ret[version][_os][language][mode]['wrong_pcms'][pcm_path] = (
                                        self._ret[version][_os][language][mode][pcm_path], label[language][pcm_path]['local'], label[language][pcm_path]['cloud'])
                            elif language == 'en':
                                if not self._compare(self._ret[version][_os][language][mode][pcm_path], label[language][pcm_path], language):
                                    self._cmp_ret[version][_os][language][mode]['wrong_pcms'][pcm_path] = (
                                        self._ret[version][_os][language][mode][pcm_path], label[language][pcm_path])

        for version in self._cmp_ret:
            for os in self._cmp_ret[version]:
                for language in self._cmp_ret[version][os]:
                    for mode in self._cmp_ret[version][os][language]:
                        # 句识别率
                        # label[language]的元素数比总标注多一个，因为还有一个'service'集合
                        sr_correct_rate = (len(label[language]) - len(self._cmp_ret[version][os][language][
                            mode]['wrong_pcms']) - 1) / (len(label[language]) - 1)
                        self._cmp_ret[version][os][language][mode]['sr_rate'] = sr_correct_rate

                        # 分业务识别率
                        sr_correct_rate_by_service = {service:(len(label[language]['service'][service])-len(label[language]['service'][service]&self._cmp_ret[version][os][language][mode]['wrong_pcms'].keys()))/len(label[language]['service'][service]) for service in label[language]['service']}
                        self._cmp_ret[version][os][language][
                        mode]['sr_rate_by_service'] = sr_correct_rate_by_service

        return self._cmp_ret

if __name__ == '__main__':
    import pprint
    parse = ParseRecognitionRate()

    ret = parse.parse_label('label.csv', 'ch')
    print(len(ret['ch']))

    ret = parse.parse_label('label_en.csv', 'en')
    print(len(ret['en']))
    
    parse.write_label_to_pickle('.')
