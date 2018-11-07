#coding=utf-8

__author__ = 'hcliu'

import json
import sys
import os
import shutil

cfg_path = sys.argv[1]
cfgs = sys.argv[2:]

if not os.path.exists(cfg_path+'.bak'):
    shutil.copyfile(cfg_path, cfg_path+'.bak') 

with open(cfg_path, encoding='utf-8') as fp:
    cfg_data = json.load(fp, encoding='utf-8')

for test in cfg_data['test_to_run']:
    if not test.endswith('doc'):
        cfg_data['test_to_run'][test] = 0

for temp_test in cfg_data['temp_test_to_run']:
    if not temp_test.endswith('doc'):
        cfg_data['temp_test_to_run'][temp_test] = 0

for cfg in cfgs:
    cfg_name = cfg.split(':')[0]
    cfg_val = cfg.split(':')[1]

    cfg_data['test_to_run'][cfg_name] = int(cfg_val)

with open(cfg_path, 'w', encoding='utf-8') as fp:
    json.dump(cfg_data, fp, indent=4, ensure_ascii=False, sort_keys=True)
