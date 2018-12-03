# -*- coding: utf-8 -*-
import os
import sys
import shutil
import random
import os.path

'''
从一批音频中随机抽取出指定数量的音频。

参数：原始音频根目录、放置音频的目录、需要抽取的音频数。
'''

files = []
files_choice = set()

old_pcms_dir = sys.argv[1]
new_pcms_dir_name = sys.argv[2]
new_pcms_num = sys.argv[3]


def get_files_in_dir(rootDir):
    for lists in os.listdir(rootDir):
        path = os.path.join(rootDir, lists)

        if os.path.isdir(path):
            get_files_in_dir(path)
        else:
            if path.endswith('pcm') or path.endswith('wav'):
                files.append(path)

if __name__ == '__main__':
    get_files_in_dir(old_pcms_dir)

    while len(files_choice) < int(new_pcms_num):
        file_choice = random.choice(files)
        files_choice.add(file_choice)

    if os.path.exists(new_pcms_dir_name):
        shutil.rmtree(new_pcms_dir_name)
    os.makedirs(new_pcms_dir_name)

    with open(os.path.join(new_pcms_dir_name, 'pcm_list.txt'), 'w') as fp:
        num = 1
        for file in files_choice:
            file_name = file.split('\\')[-1]
            file_path = file.split('\\' + file_name)[0]

            new_file_path = file_path.replace(old_pcms_dir, new_pcms_dir_name)
            if not os.path.exists(new_file_path):
                os.makedirs(new_file_path)

            shutil.copyfile(file, os.path.join(new_file_path, file_name))
            print(str(num) + ': ' + os.path.join(new_file_path, file_name))
            fp.write(os.path.join(new_file_path, file_name) + '\n')
            num += 1
