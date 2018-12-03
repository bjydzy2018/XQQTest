#coding=utf-8

import glob
import json
import pprint
import sys

files = [file.split('\\')[1] for file in glob.glob('./*.txt')]

dic = {}
dic['grm'] = []

current_dict_file_index = 0

for file in files:
    ret_file = open(sys.argv[1]+'.dict', 'w', encoding='utf-8')

    dict_name = file.split('.')[0]

    dict_file = {}
    dic['grm'].append(dict_file)

    dic['grm'][current_dict_file_index]['dictname'] = dict_name

    dict_id = 0
    dic['grm'][current_dict_file_index]['dictcontant'] = []

    with open(file, encoding='utf-8') as fp:
        num = 1
        for line in fp:
            line = line.strip()
            dic['grm'][current_dict_file_index]['dictcontant'].append(dict(id=dict_id, name=line))
            dict_id += 1

            if num == int(sys.argv[1]):
                break

            num += 1

    current_dict_file_index += 1

    ret_file.write(json.dumps(dic, sort_keys='id', ensure_ascii=False, indent=2))