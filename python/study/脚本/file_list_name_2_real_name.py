#coding=utf-8

'''
将列表中的文件名替换为实际的文件名，适用于实际文件名在原有基础上增加新内容的情况，如：1.txt -> 1-a.txt。
'''

import os
import sys

src_file = sys.argv[1] # 需要修改的列表文件
walk_path = sys.argv[2] # 遍历的目录
walk_type = sys.argv[3].split('/') # 需要提取路径的文件格式，以/分隔
walk_list = []
src_list = []

def isFileTypeNeed(file_name):
    for t in walk_type:
        if file_name.endswith(t):
            return True

    return False

for root, dirs, files in os.walk(walk_path):
    for file in files:
        if isFileTypeNeed(file):
            walk_list.append(os.path.join(root, file))

with open(src_file) as fp:
    for line in fp:
        src_list.append(line)

with open(src_file, 'w') as fp:
    for old_name in src_list:
        old_file_name = old_name.split('/')[-1].split('.')[0]
        old_file_type = old_name.split('/')[-1].split('.')[1]
        old_file_xtype = old_name.split('/')[-2]

        for real_name in walk_list:
            real_name = real_name.replace('\\', '/')

            real_file_name = real_name.split('/')[-1].split('.')[0]
            real_file_type = real_name.split('/')[-1].split('.')[1]
            real_file_xtype = real_name.split('/')[-2]

            if old_file_name in real_file_name and old_file_xtype == real_file_xtype:
                fp.write(real_name+'\n')
                break
        else:
            fp.write(old_name+'\n')
