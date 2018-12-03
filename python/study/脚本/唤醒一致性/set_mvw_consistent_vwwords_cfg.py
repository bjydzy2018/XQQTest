#coding=utf-8

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

cfg_data['WakeUpScenes'][0]['KeyWordsType'] = 0
cfg_data['WakeUpScenes'][0]['ResourcePath'] = 'nhyyzl.irf'

cfg_data['WakeUpScenes'][1]['KeyWordsType'] = 0
cfg_data['WakeUpScenes'][1]['ResourcePath'] = 'quxiao.irf'

cfg_data['WakeUpScenes'][2]['KeyWordsType'] = 0
cfg_data['WakeUpScenes'][2]['ResourcePath'] = 'cancel.irf'
cfg_data['WakeUpScenes'][2]['KeyWordsAndDefaultThresholds'][0]['KeyWord'] = 'cancel'

cfg_data['WakeUpScenes'][3]['KeyWordsType'] = 0
cfg_data['WakeUpScenes'][3]['ResourcePath'] = 'xanh.irf'
cfg_data['WakeUpScenes'][3]['KeyWordsAndDefaultThresholds'][0]['KeyWord'] = '小艾你好'

with open(cfg_path, 'w', encoding='utf-8') as fp:
    json.dump(cfg_data, fp, indent=4, ensure_ascii=False, sort_keys=True)
