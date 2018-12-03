# coding=utf-8

__author__ = 'hcliu'

'''
解析版本配置文件，并与基线相比较，并输出变动内容。
'''

from parse_cfg import CfgParse
import os
import os.path as op
import pickle
from collections import defaultdict


class PackCfg:
    def __init__(self, JSONdir, path, address, DBname, user, pwd, version):
        self.JSONdir = JSONdir
        self.path = path
        self.address = address
        self.DBname = DBname
        self.user = user
        self.pwd = pwd
        self.version = version
        self.cfgs = [
            ('win32', 'sr-common', 'win32\\bin\\SRRes\\isssrcom.cfg'),
            ('win32', 'sr-common-mvwInSr', 'win32\\bin\\SRRes\\issmvwcom.cfg'),
            ('win32', 'sr-ResMandarin', 'win32\\bin\\SRRes\\ResMandarin\\isssr.cfg'),
            ('win32', 'sr-ResEnglish', 'win32\\bin\\SRRes\\ResEnglish\\isssr.cfg'),
            ('win32', 'mvw-common', 'win32\\bin\\MVWRes\\issmvwcom.cfg'),
            ('win32', 'mvw-ResMandarin',
             'win32\\bin\\Active\\MVWRes\\FirstRes\\ResMandarin\\issmvw.cfg'),
            ('win32', 'mvw-ResEnglish',
             'win32\\bin\\Active\\MVWRes\\FirstRes\\ResEnglish\\issmvw.cfg'),
            ('win32', 'tts-common', 'win32\\bin\\TTSRes\\isstts.cfg'),
            ('win32', 'cata-common', 'win32\\bin\\CataRes\\isscata.cfg'),

            ('qnx660', 'sr-common', 'QNX.arm\\bin\\SRRes\\isssrcom.cfg'),
            ('qnx660', 'sr-common-mvwInSr', 'QNX.arm\\bin\\SRRes\\issmvwcom.cfg'),
            ('qnx660', 'sr-ResMandarin', 'QNX.arm\\bin\\SRRes\\ResMandarin\\isssr.cfg'),
            ('qnx660', 'sr-ResEnglish', 'QNX.arm\\bin\\SRRes\\ResEnglish\\isssr.cfg'),
            ('qnx660', 'mvw-common', 'QNX.arm\\bin\\MVWRes\\issmvwcom.cfg'),
            ('qnx660', 'mvw-ResMandarin',
             'QNX.arm\\bin\\Active\\MVWRes\\FirstRes\\ResMandarin\\issmvw.cfg'),
            ('qnx660', 'mvw-ResEnglish',
             'QNX.arm\\bin\\Active\\MVWRes\\FirstRes\\ResEnglish\\issmvw.cfg'),
            ('qnx660', 'tts-common', 'QNX.arm\\bin\\TTSRes\\isstts.cfg'),
            ('qnx660', 'cata-common', 'QNX.arm\\bin\\CataRes\\isscata.cfg'),

            ('qnx650', 'sr-common', 'QNX650.arm\\bin\\SRRes\\isssrcom.cfg'),
            ('qnx650', 'sr-common-mvwInSr', 'QNX650.arm\\bin\\SRRes\\issmvwcom.cfg'),
            ('qnx650', 'sr-ResMandarin',
             'QNX650.arm\\bin\\SRRes\\ResMandarin\\isssr.cfg'),
            ('qnx650', 'sr-ResEnglish',
             'QNX650.arm\\bin\\SRRes\\ResEnglish\\isssr.cfg'),
            ('qnx650', 'mvw-common', 'QNX650.arm\\bin\\MVWRes\\issmvwcom.cfg'),
            ('qnx650', 'mvw-ResMandarin',
             'QNX650.arm\\bin\\Active\\MVWRes\\FirstRes\\ResMandarin\\issmvw.cfg'),
            ('qnx650', 'mvw-ResEnglish',
             'QNX650.arm\\bin\\Active\\MVWRes\\FirstRes\\ResEnglish\\issmvw.cfg'),
            ('qnx650', 'tts-common', 'QNX650.arm\\bin\\TTSRes\\isstts.cfg'),
            ('qnx650', 'cata-common', 'QNX650.arm\\bin\\CataRes\\isscata.cfg'),

            ('linux', 'sr-common', 'linux.arm\\bin\\SRRes\\isssrcom.cfg'),
            ('linux', 'sr-common-mvwInSr', 'linux.arm\\bin\\SRRes\\issmvwcom.cfg'),
            ('linux', 'sr-ResMandarin',
             'linux.arm\\bin\\SRRes\\ResMandarin\\isssr.cfg'),
            ('linux', 'sr-ResEnglish', 'linux.arm\\bin\\SRRes\\ResEnglish\\isssr.cfg'),
            ('linux', 'mvw-common', 'linux.arm\\bin\\MVWRes\\issmvwcom.cfg'),
            ('linux', 'mvw-ResMandarin',
             'linux.arm\\bin\\Active\\MVWRes\\FirstRes\\ResMandarin\\issmvw.cfg'),
            ('linux', 'mvw-ResEnglish',
             'linux.arm\\bin\\Active\\MVWRes\\FirstRes\\ResEnglish\\issmvw.cfg'),
            ('linux', 'tts-common', 'linux.arm\\bin\\TTSRes\\isstts.cfg'),
            ('linux', 'cata-common', 'linux.arm\\bin\\CataRes\\isscata.cfg'),

            ('android', 'sr-common',
             'android\\bin\\Resources\\iflytek\\res\\sr\\isssrcom.cfg'),
            ('android', 'sr-common-mvwInSr',
             'android\\bin\\Resources\\iflytek\\res\\sr\\issmvwcom.cfg'),
            ('android', 'sr-ResMandarin',
             'android\\bin\\Resources\\iflytek\\res\\sr\\ResMandarin\\isssr.cfg'),
            ('android', 'sr-ResEnglish',
             'android\\bin\\Resources\\iflytek\\res\\sr\\ResEnglish\\isssr.cfg'),
            ('android', 'mvw-common',
             'android\\bin\\Resources\\iflytek\\res\\mvw\\issmvwcom.cfg'),
            ('android', 'mvw-ResMandarin',
             'android\\bin\\Resources\\iflytek\\res\\Active\\MVWRes\\FirstRes\\ResMandarin\\issmvw.cfg'),
            ('android', 'mvw-ResEnglish',
             'android\\bin\\Resources\\iflytek\\res\\Active\\MVWRes\\FirstRes\\ResEnglish\\issmvw.cfg'),
            ('android', 'tts-common',
             'android\\bin\\Resources\\iflytek\\res\\tts\\isstts.cfg'),
            ('android', 'cata-common',
             'android\\bin\\Resources\\iflytek\\res\\cata\\isscata.cfg')
        ]

    def pack(self):
        for cfg in self.cfgs:
            print('开始插入：' + str(cfg))
            path = os.path.join(self.path, cfg[2])
            os.system('python cfg_to_u8cfg.py {0}'.format(path))

            gen = CfgParse(path, cfg[0], self.version,
                           cfg[1].split('-')[0], cfg[1])
            gen.gen()
            if self.version == 'std':
                gen.insert_into_pickle(self.JSONdir)
            gen.insert_into_database(
                self.address, self.user, self.pwd, self.DBname)

    def compare(self):
        cfg_content = None
        diff_cfg_content = {}
        more_cfg_content = {}
        less_cfg_content = {}

        with open(op.join(self.JSONdir, 'cfg.pickle'), 'rb') as fp:
            cfg_content = pickle.load(fp)

        for module in cfg_content:
            for os in cfg_content[module]:
                if self.version in cfg_content[module][os]:
                    for tag in cfg_content[module][os][self.version]:
                        if tag in cfg_content[module][os]['std']:
                            # 内容不同的配置项
                            for cfg_label in (cfg_content[module][os][self.version][tag]['cfg'].keys() & cfg_content[module][os]['std'][tag]['cfg'].keys()):
                                if cfg_content[module][os][self.version][tag]['cfg'][cfg_label] != cfg_content[module][os]['std'][tag]['cfg'][cfg_label]:
                                    if module not in diff_cfg_content:
                                        diff_cfg_content[module] = {}

                                    if os not in diff_cfg_content[module]:
                                        diff_cfg_content[module][os] = {}

                                    if tag not in diff_cfg_content[module][os]:
                                        diff_cfg_content[module][os][tag] = []

                                    diff_cfg_content[module][os][tag].append((cfg_label, cfg_content[module][os][self.version][tag][
                                        'cfg'][cfg_label], cfg_content[module][os]['std'][tag]['cfg'][cfg_label]))
                            # 比基线多的配置项
                            for cfg_label in (cfg_content[module][os][self.version][tag]['cfg'].keys() - cfg_content[module][os]['std'][tag]['cfg'].keys()):
                                if module not in more_cfg_content:
                                    more_cfg_content[module] = {}

                                if os not in more_cfg_content[module]:
                                    more_cfg_content[module][os] = {}

                                if tag not in more_cfg_content[module][os]:
                                    more_cfg_content[module][os][tag] = []
                                more_cfg_content[module][os][tag].append((cfg_label, cfg_content[module][os][self.version][tag][
                                    'cfg'][cfg_label], ''))
                            # 比基线少的配置项
                            for cfg_label in (cfg_content[module][os]['std'][tag]['cfg'].keys() - cfg_content[module][os][self.version][tag]['cfg'].keys()):
                                if module not in less_cfg_content:
                                    less_cfg_content[module] = {}

                                if os not in less_cfg_content[module]:
                                    less_cfg_content[module][os] = {}

                                if tag not in less_cfg_content[module][os]:
                                    less_cfg_content[module][os][tag] = []
                                less_cfg_content[module][os][tag].append(
                                    (cfg_label, '', cfg_content[module][os]['std'][tag]['cfg'][cfg_label]))

        return diff_cfg_content, more_cfg_content, less_cfg_content

if __name__ == '__main__':
    import argparse
    import sys
    parser = argparse.ArgumentParser(description='解析版本配置文件，并与基线相比较，并输出变动内容。')
    parser.add_argument('-v', '--version',
                        dest='version',
                        action='store',
                        help='版本')
    parser.add_argument('-j', '--JSONdir',
                        dest='JSONdir',
                        action='store',
                        help='保存json结果文件的目录')
    parser.add_argument('-r', '--releaseDir',
                        dest='releaseDir',
                        action='store',
                        help='07.Release目录的路径')
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

    packCfg = PackCfg(args.JSONdir, args.releaseDir, args.address,
                      args.DBname, args.user, args.pwd, args.version)
    packCfg.pack()
    if args.version != 'std':
        diff_cfg_content, more_cfg_content, less_cfg_content = packCfg.compare()
        if diff_cfg_content or more_cfg_content or less_cfg_content:
            import xlwt

            def write_to_sheet(diff_cfg_content, sheet):
                row = 0
                for module in diff_cfg_content:
                    sheet.write(row, 0, '模块')
                    sheet.write(row, 1, '平台')
                    sheet.write(row, 2, '标签')
                    sheet.write(row, 3, '路径')
                    sheet.write(row, 4, '配置项')
                    sheet.write(row, 5, '当前版本值')
                    sheet.write(row, 6, '基线值')
                    row += 1

                    sheet.write(row, 0, module)
                    for os in diff_cfg_content[module]:
                        sheet.write(row, 1, os)

                        for tag in diff_cfg_content[module][os]:
                            sheet.write(row, 2, tag)
                            sheet.write(row, 3, [cfg[2] for cfg in packCfg.cfgs if cfg[
                                        1] == tag and cfg[0] == os][0])

                            for content in diff_cfg_content[module][os][tag]:
                                sheet.write(row, 4, str(content[0]))
                                sheet.write(row, 5, str(content[1]))
                                sheet.write(row, 6, str(content[2]))
                                row += 1

            file = xlwt.Workbook()

            if diff_cfg_content:
                sheet_diff = file.add_sheet('内容不同项')
                write_to_sheet(diff_cfg_content, sheet_diff)
            if more_cfg_content:
                sheet_more = file.add_sheet('内容多余项')
                write_to_sheet(more_cfg_content, sheet_more)
            if less_cfg_content:
                sheet_less = file.add_sheet('内容缺少项')
                write_to_sheet(less_cfg_content, sheet_less)

            file.save('cfg_compare.xls')
            sys.exit(1)
