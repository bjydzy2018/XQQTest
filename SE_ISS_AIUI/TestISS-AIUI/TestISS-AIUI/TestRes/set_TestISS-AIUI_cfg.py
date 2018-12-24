#encoding=utf-8

__author__ = 'hcliu'

import json
import sys
import os
import shutil

cfg_path = sys.argv[1]

if not os.path.exists(cfg_path+'.bak'):
    shutil.copyfile(cfg_path, cfg_path+'.bak') 

with open(cfg_path, encoding='utf-8') as fp:
    cfg_data = json.load(fp, encoding='utf-8')

for test in sys.argv[2:]:
    if test == 'sr_time_ch_local':
        for test_cfg in cfg_data['tests']:
            if test_cfg['name'] == 'sr_batch':
                test_cfg['mode'] = '1'
                test_cfg['lang'] = '0'
                test_cfg['test_mode'] = '4'
                test_cfg['loop_num'] = '1'

                break

with open(cfg_path, 'w', encoding='utf-8') as fp:
    json.dump(cfg_data, fp, indent=4, ensure_ascii=False, sort_keys=True)