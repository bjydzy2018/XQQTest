# coding=utf-8

__author__ = 'hcliu'

'''
处理可见即可说的log。
'''
import re
import datetime
import sys
import csv


class ParseSTKSLog:
    def __init__(self, log_file):
        self.log_file = log_file

        # 匹配时间
        self.pattern_time = '\[(.*?)\].*'
        # 匹配poixp结果
        self.pattern_poixp = '.*SPW Result: (.*)'
        # 匹配cata搜索结果
        self.pattern_cataSearch = '.*szQuery=(.*), retn=.*'
        # 匹配传给识别的cmdlist
        self.pattern_cmdlist = r'.*\[TAG_A\] Start: iMod\(1\) Scene\(selectlist_poi\) Cmd\((.*)\).*'
        # 匹配二次识别的语义结果
        self.pattern_nluResult = '.*nluResult: (.*)'

        # 状态
        self.state_second_sr = False  # 第二次识别为分词识别
        self.state_poixp = False
        self.state_was_second_sr = False

        # 需要统计的时间量
        self.time_poixp = None
        self.time_cataIndex = None
        self.time_GrmDictBegin = None
        self.time_GrmDictInsert = None
        self.time_GrmDictEnd = None
        self.time_EsrCommitNetWork = None
        self.time_nli = None
        self.time_iFlyPResBuildInit = None
        self.time_Start_sum = None
        self.time_nli_parse = None
        self.time_cataSearch = None

        # 需要统计的结果值
        self.text_poixp = []
        self.text_cataSearch = None
        self.text_cmdlist = None
        self.text_nluResult = None

    def init_msg(self):
        # 各需要统计的时间量
        self.time_poixp = None
        self.time_cataIndex = None
        self.time_GrmDictBegin = None
        self.time_GrmDictInsert = None
        self.time_GrmDictEnd = None
        self.time_EsrCommitNetWork = None
        self.time_nli = None
        self.time_iFlyPResBuildInit = None
        self.time_Start_sum = None
        self.time_nli_parse = None
        self.time_cataSearch = None

        # 各需要统计的结果值
        self.text_poixp = []
        self.text_cataSearch = None
        self.text_cmdlist = None
        self.text_nluResult = None

        # 状态
        self.state_second_sr = False  # 第二次识别为分词识别
        self.state_poixp = False
        self.state_was_second_sr = False

    def get_time_from_str(self, time_str):
        '''
        将log中的时间字符串转换为datetime对象。
        '''
        return datetime.datetime.strptime('20' + time_str, '%Y/%m/%d-%H:%M:%S %f')

    def get_time_diff(self, time1, time2):
        '''
        计算时间差。

        计算公式为time2-time1。
        '''
        ret = (time2 - time1).seconds * 1000 + \
            (time2 - time1).microseconds / 1000
        return ret

    def _handleText(self, text):
        if text:
            return '"' + text + '"'
        else:
            return 'None'

    def parse(self):
        with open('ret.csv', 'w', newline='') as ret_fp:
            f_csv = csv.writer(ret_fp)
            f_csv.writerow('text_cmdlist,text_poixp,text_nluResult,text_cataSearch,time_poixp,time_cataIndex, time_GrmDictBegin,time_GrmDictInsert,time_GrmDictEnd,time_EsrCommitNetWork,time_nli,time_iFlyPResBuildInit,time_Start_sum,time_nli_parse,time_cataSearch'.split(','))

            with open(self.log_file, 'rt', encoding='utf-8', errors='ignore') as fp:
                for line in fp:
                    line = line.strip()
                    try:
                        time = self.get_time_from_str(
                            re.match(self.pattern_time, line).group(1))
                    except:
                        continue

                    if 'Start Called, iMode = 1' in line:
                        # 进入第二次识别
                        time_Start_Called_iMode_1 = time

                        self.state_second_sr = True
                    elif 'Sr to Start Aitalk Select List' in line:
                        # 二次识别结束
                        time_Sr_to_Start_Aitalk_Select_List = time
                        self.time_Start_sum = self.get_time_diff(
                            time_Start_Called_iMode_1, time_Sr_to_Start_Aitalk_Select_List)

                        self.state_second_sr = False
                        self.state_was_second_sr = True
                    elif 'nluResult' in line:
                        if self.state_was_second_sr:
                            self.text_nluResult = re.match(
                                self.pattern_nluResult, line).group(1)
                    elif 'Before NLIInterpret' in line:
                        if self.state_was_second_sr:
                            time_Before_NLIInterpret = time
                    elif 'NLIInterpret ret' in line:
                        if self.state_was_second_sr:
                            time_NLIInterpret_ret = time

                            self.time_nli_parse = self.get_time_diff(
                                time_Before_NLIInterpret, time_NLIInterpret_ret)
                    elif 'Start Called, iMode = 2' in line:
                        self.state_was_second_sr = False

                        f_csv.writerow([self.text_cmdlist, str(self.text_poixp), self.text_nluResult, self.text_cataSearch, self.time_poixp, self.time_cataIndex, self.time_GrmDictBegin, self.time_GrmDictInsert, self.time_GrmDictEnd,
                                        self.time_EsrCommitNetWork, self.time_nli, self.time_iFlyPResBuildInit, self.time_Start_sum, self.time_nli_parse, self.time_cataSearch])

                        self.init_msg()
                    else:
                        if self.state_second_sr:
                            # 在二次识别条件下

                            if 'before poixp' in line:
                                time_before_poixp = time
                                self.state_poixp = True
                            elif 'after poixp' in line:
                                time_after_poixp = time
                                self.state_poixp = False

                                self.time_poixp = self.get_time_diff(
                                    time_before_poixp, time_after_poixp)
                            elif 'CCataIndex::CCataIndex In.' in line:
                                time_CCataIndex_CCataIndex_In = time
                            elif 'CCataIndex::~CCataIndex Out' in line:
                                time_CCataIndex_CCataIndex_Out = time

                                self.time_cataIndex = self.get_time_diff(
                                    time_CCataIndex_CCataIndex_In, time_CCataIndex_CCataIndex_Out)
                            elif 'GrmDictBeginNormal Selectpoi ivHotWordDict success' in line:
                                time_GrmDictBeginNormal_Selectpoi_ivHotWordDict_success = time

                                self.time_GrmDictBegin = self.get_time_diff(
                                    time_GrmDictBeginNormal_Selectpoi_ivHotWordDict, time_GrmDictBeginNormal_Selectpoi_ivHotWordDict_success)
                            elif 'GrmDictBeginNormal Selectpoi ivHotWordDict' in line:
                                time_GrmDictBeginNormal_Selectpoi_ivHotWordDict = time
                            elif 'before GrmDictEnd' in line:
                                time_before_GrmDictEnd = time

                                try:
                                    self.time_GrmDictInsert = self.get_time_diff(
                                        time_GrmDictBeginNormal_Selectpoi_ivHotWordDict_success, time_before_GrmDictEnd)
                                except Exception as err:
                                    print(line)
                                    print(err)
                                    sys.exit(1)
                            elif 'GrmDictEnd success' in line:
                                time_GrmDictEnd_success = time

                                self.time_GrmDictEnd = self.get_time_diff(
                                    time_before_GrmDictEnd, time_GrmDictEnd_success)
                            elif 'before EsrCommitNetWork Selectpoi.t' in line:
                                time_before_EsrCommitNetWork_Selectpoi_t = time
                            elif 'EsrCommitNetWork success' in line:
                                time_EsrCommitNetWork_success = time

                                self.time_EsrCommitNetWork = self.get_time_diff(
                                    time_before_EsrCommitNetWork_Selectpoi_t, time_EsrCommitNetWork_success)
                            elif 'before nli build and load dict.bin' in line:
                                time_before_nli_build_and_load_dict_bin = time
                            elif 'after nli build and load dict.bin' in line:
                                time_after_nli_build_and_load_dict_bin = time

                                self.time_nli = self.get_time_diff(
                                    time_before_nli_build_and_load_dict_bin, time_after_nli_build_and_load_dict_bin)
                            elif 'Before iFlyPResBuildInit' in line:
                                time_Before_iFlyPResBuildInit = time
                            elif 'iFlyPResBuildInit ret 0' in line:
                                time_iFlyPResBuildInit_ret_0 = time

                                self.time_iFlyPResBuildInit = self.get_time_diff(
                                    time_Before_iFlyPResBuildInit, time_iFlyPResBuildInit_ret_0)
                            elif '[TAG_A] Start: iMod(1) Scene(selectlist_poi)' in line:
                                try:
                                    self.text_cmdlist = re.match(
                                        self.pattern_cmdlist, line).group(1)
                                except:
                                    print('cmdlist匹配失败：', line)
                            else:
                                if self.state_poixp and 'no result' not in line:
                                    # 提取poixp结果
                                    try:
                                        poixp_ret = re.match(
                                            self.pattern_poixp, line).group(1)
                                    except:
                                        print(
                                            'poixp_ret match failed：', line[:50])

                                    self.text_poixp.extend(
                                        poixp_ret.split(';'))
                        else:
                            if 'CCataSearch::SearchSync In' in line:
                                time_CCataSearch_SearchSync_In = time

                                self.text_cataSearch = re.match(
                                    self.pattern_cataSearch, line).group(1)
                            elif 'CCataSearch::SearchSync Out' in line:
                                time_CCataSearch_SearchSync_Out = time

                                self.time_cataSearch = self.get_time_diff(
                                    time_CCataSearch_SearchSync_In, time_CCataSearch_SearchSync_Out)


if __name__ == '__main__':
    import sys
    log_file = sys.argv[1]

    parse = ParseSTKSLog(log_file)
    parse.parse()
