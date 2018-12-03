#coding=utf-8

import os
import sys

walk_path = sys.argv[1] # 遍历路径
ret_file_name = sys.argv[2] # 保存结果的文件名
walk_type = sys.argv[3].split('/') # 需要提取路径的文件格式，以/分隔

ret_file = open(ret_file_name, 'w')

def isFileTypeNeed(file_name):
    for t in walk_type:
        if file_name.endswith(t):
            return True

    return False

for root, dirs, files in os.walk(walk_path):
    for file in files:
        if isFileTypeNeed(file):
            ret_file.write(os.path.join(root, file) + '\n')