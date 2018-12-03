# coding=utf-8

__author__ = 'hcliu'

'''
获取单个词典上传后的资源大小比较。
'''

import os
from os.path import join, getsize, exists
import sys
from collections import defaultdict


def get_folder_size(folder):
    '''
    获取一个文件夹的大小
    '''
    size = 0
    for root, dirs, files in os.walk(folder):
        size += sum([getsize(join(root, name)) for name in files])

    return size


def _is_usable_folder(folder):
    '''
    判断目录是否需要统计。
    '''
    if 'CataRes' in folder or 'dicts' in folder or 'GrmBuild' in folder or 'GrmSR' in folder:
        if 'SRRes' in folder:
            if 'ResEnglish' not in folder and 'ResCantonese' not in folder:
                return True
        else:
            return True

    return False


def _get_dict_folder_paths(folder):
    '''
    获取需要统计的目录路径。
    '''
    paths = [root for root, dirs, files in os.walk(
        folder) if _is_usable_folder(root)]

    return paths


def _get_dict_folder_size(dict_folder):
    '''
    统计上传词典后变化的目录的大小。
    '''
    sizes = {}
    paths = _get_dict_folder_paths(dict_folder)
    for folder in paths:
        sizes[folder] = get_folder_size(folder)

    return sizes


def _relative2absolute(folders):
    '''
    将需要统计的绝对路径映射为各目录名。

    会将dicts, GrmSR, GrmBuild, CataRes(2个)的路径统一映射为'dicts', 'GrmSR', 'GrmBuild',
    'CataRes', 'CataRes_Active'的字典键。外部直接通过这些键来访问真实路径。

    CataRes_Active代表识别中文语种资源里的CataRes目录，CataRes代表bin目录下的CataRes目录。
    '''
    ret = {}

    # 记录CataRes信息。结果中可能会有两个CataRes结果，一个是Active中的，一个是之外的。
    # 套件只统计Active目录里的CataRes，对话引擎会两个都统计。
    # 这里用路径长度来做判断，较长的那个是Active中的
    temp_CataRes = defaultdict(dict)
    temp_num = 0

    for folder in folders:
        if 'dicts' in folder:
            ret['dicts'] = folder
        elif 'GrmBuild' in folder:
            ret['GrmBuild'] = folder
        elif 'GrmSR' in folder:
            ret['GrmSR'] = folder
        elif 'CataRes' in folder:
            temp_CataRes[temp_num]['folder'] = folder
            temp_CataRes[temp_num]['len'] = len(folder)
            temp_num += 1

    if temp_num > 1:
        if temp_CataRes[0]['len'] > temp_CataRes[1]['len']:
            ret['CataRes_Active'] = temp_CataRes[0]['folder']
            ret['CataRes'] = temp_CataRes[1]['folder']
        else:
            ret['CataRes_Active'] = temp_CataRes[1]['folder']
            ret['CataRes'] = temp_CataRes[0]['folder']
    else:
        ret['CataRes_Active'] = temp_CataRes[0]['folder']

    return ret


def get_dict_compare_ret(old_path, new_path):
    '''
    获取上传词典前后rom大小变化的结果。
    '''
    old_path = old_path.replace('\\', os.path.sep).replace('/', os.path.sep)
    new_path = new_path.replace('\\', os.path.sep).replace('/', os.path.sep)

    if not exists(old_path) or not exists(new_path):
        return None

    ret = {}
    old_sizes = _get_dict_folder_size(old_path)
    new_sizes = _get_dict_folder_size(new_path)

    old_sizes_relative = _relative2absolute(old_sizes)
    new_sizes_relative = _relative2absolute(new_sizes)

    for path in new_sizes_relative:
        old_size = old_sizes[old_sizes_relative[path]]
        new_size = new_sizes[new_sizes_relative[path]]
        ret[path] = (old_size, new_size, new_size - old_size)

    return ret


if __name__ == '__main__':
    old_path = sys.argv[1]
    new_path = sys.argv[2]

    ret = get_dict_compare_ret(old_path, new_path)

    import xlwt

    ret_file = xlwt.Workbook()
    table = ret_file.add_sheet('上传词典rom变化统计')

    col = 1
    for key in ret:
        table.write(0, col, key)
        table.write(1, col, str(ret[key][0]))
        table.write(2, col, str(ret[key][1]))
        table.write(3, col, str(ret[key][2]))

        col += 1

    table.write(1, 0, '原始大小')
    table.write(2, 0, '上传后大小')
    table.write(3, 0, '上传后增幅')

    ret_file.save('upload_dict_ret.xls')
