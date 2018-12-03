# coding=utf-8

__author__ = 'hcliu'

'''
设置win32平台识别配置文件。
'''

import sys
import json

cfg_file = sys.argv[1]
cfgs = sys.argv[2:]

with open(cfg_file, encoding='utf-8-sig') as fp:
    cfg_data = json.load(fp)

for cfg in cfgs:
    cfg_name = cfg.split(':')[0]
    cfg_value = cfg.split(':')[1]
    cfg_type = cfg.split(':')[2]

    if ('int' == cfg_type):
        cfg_data[cfg_name] = int(cfg_value)
    else:
        cfg_data[cfg_name] = cfg_value

with open(cfg_file, 'w', encoding='utf-8-sig') as fp:
    json.dump(cfg_data, fp, ensure_ascii=False, indent=2)
