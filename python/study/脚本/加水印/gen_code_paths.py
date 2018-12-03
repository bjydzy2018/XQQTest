#coding=utf-8

import sys
import os.path
import os

# 音频路径
file_path = sys.argv[1]

file_ret = open(file_path.split('.')[0]+'_code.txt', 'w')
# 需要存放加密后音频的目录名
file_path_ret_dir = sys.argv[2]

num = 0
path_set = set()

with open(file_path, encoding='utf-8') as fp:
    for line in fp:
        line = line.strip()
        line_code = file_path_ret_dir+'/'+'/'.join(line.split('/')[1:]).split('.')[0]+'.'+line.split('.')[-1]
        file_ret.write(line+' '+line_code+'\n')

        line_code_dir = '/'.join(line_code.split('/')[:-1])
        if line_code_dir not in path_set:
            path_set.add(line_code_dir)
            os.system('md '+line_code_dir.replace('/', '\\'))
