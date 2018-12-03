# coding=utf-8

__author__ = 'hcliu'

'''
配置c测试工具配置文件isr_list.cfg
'''

import sys
import json

cfg_file = sys.argv[1]
cfgs = sys.argv[2:]

with open(cfg_file, encoding='utf-8') as fp:
    cfg_data = json.load(fp)

for cfg in cfgs:
    cfg_name = cfg.split(':')[0]
    cfg_value = cfg.split(':')[1]

    cfg_data[cfg_name] = cfg_value

with open(cfg_file, 'w', encoding='utf-8') as fp:
    json.dump(cfg_data, fp, ensure_ascii=False, indent=2)
