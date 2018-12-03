# coding=utf-8

__author__ = 'hcliu'

'''
解析套件版本资源文件大小。

12.30: 修改音频路径截取规则，现在只保留『release目录+系统路径分隔符』后的路径
'''

import os
from os.path import join, getsize
from collections import OrderedDict


class ParseRom:
    def __init__(self, _version, _releaseDir):
        self._version = str(_version)
        self._releaseDir = _releaseDir
        self.res_version_correct = {}
        self._module_resourse_paths = {
            'sr': [
                'win32\\bin\\SRRes',
                'win32\\bin\\Active\\SRRes',
                'linux.arm\\bin\\SRRes',
                'linux.arm\\bin\\Active\\SRRes',
                'QNX.arm\\bin\\SRRes',
                'QNX.arm\\bin\\Active\\SRRes',
                'QNX650.arm\\bin\\SRRes',
                'QNX650.arm\\bin\\Active\\SRRes',
                'android\\bin\\Resources\\iflytek\\res\\sr',
                'android\\bin\\Resources\\iflytek\\res\\Active\\SRRes'
            ],
            'mvw': [
                'win32\\bin\\MVWRes',
                'win32\\bin\\Active\\MVWRes',
                'linux.arm\\bin\\MVWRes',
                'linux.arm\\bin\\Active\\MVWRes',
                'QNX.arm\\bin\\MVWRes',
                'QNX.arm\\bin\\Active\\MVWRes',
                'QNX650.arm\\bin\\MVWRes',
                'QNX650.arm\\bin\\Active\\MVWRes',
                'android\\bin\\Resources\\iflytek\\res\\mvw',
                'android\\bin\\Resources\\iflytek\\res\\Active\\MVWRes'
            ],
            'tts': [
                'win32\\bin\\TTSRes',
                'win32\\bin\\Active\\TTSRes',
                'linux.arm\\bin\\TTSRes',
                'linux.arm\\bin\\Active\\TTSRes',
                'QNX.arm\\bin\\TTSRes',
                'QNX.arm\\bin\\Active\\TTSRes',
                'QNX650.arm\\bin\\TTSRes',
                'QNX650.arm\\bin\\Active\\TTSRes',
                'android\\bin\\Resources\\iflytek\\res\\tts',
                'android\\bin\\Resources\\iflytek\\res\\Active\\TTSRes'
            ],
            'cata': [
                'win32\\bin\\CataRes',
                'win32\\bin\\Active\\CataRes',
                'linux.arm\\bin\\CataRes',
                'linux.arm\\bin\\Active\\CataRes',
                'QNX.arm\\bin\\CataRes',
                'QNX.arm\\bin\\Active\\CataRes',
                'QNX650.arm\\bin\\CataRes',
                'QNX650.arm\\bin\\Active\\CataRes',
                'android\\bin\\Resources\\iflytek\\res\\cata',
                'android\\bin\\Resources\\iflytek\\res\\Active\\CataRes'
            ]
        }

    def _check_res_version(self, path):
        with open(path, encoding='utf-8') as fp:
            data = fp.read()
            if self._version not in data:
                self.res_version_correct[path] = False

    def _is_file_sized(self, path):
        '''
        判断是否统计某个文件的大小。
        '''
        if 'res_version' in path:
            return False

        return True

    def _get_folder_size(self, folder):
        '''
        获取一个文件夹的大小。
        '''
        size = 0
        for root, dirs, files in os.walk(folder):
            size += sum([getsize(join(root, name))
                         for name in files if self._is_file_sized(join(root, name))])

        return size

    def _get_module_rom(self, module):
        '''
        获取某个模块下各资源文件的大小。
        '''
        ret = {}
        for path in self._module_resourse_paths[module]:
            for root, dirs, files in os.walk(join(self._releaseDir, path)):
                for file in files:
                    file_path = join(root, file)
                    if self._is_file_sized(file_path):
                        # 获取资源文件大小
                        ret[file_path.split(
                            self._releaseDir + os.sep)[1]] = getsize(file_path)
                    elif file_path.endswith('res_version'):
                        # 检查res_version内容
                        self._check_res_version(file_path)

        # return OrderedDict(sorted(ret.items(), key=lambda t: t[1]))
        return ret

    def parse(self):
        roms = {}
        roms['sr'] = self._get_module_rom('sr')
        roms['mvw'] = self._get_module_rom('mvw')
        roms['tts'] = self._get_module_rom('tts')
        roms['cata'] = self._get_module_rom('cata')

        return roms


if __name__ == '__main__':
    parseRom = ParseRom(
        'test', 'E:\\project\\SE_ISS3.0\\Tags\\1079')

    import pprint
    ret = parseRom.parse()
    pprint.pprint(ret)
    # pprint.pprint(parseRom.res_version_correct)

    with open('ret.txt', 'wt') as fp:
        for pcm in ret['sr']:
            print(pcm, ret['sr'][pcm], file=fp)
