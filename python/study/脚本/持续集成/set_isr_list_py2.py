# coding=utf-8

__author__ = 'hcliu'

'''
配置c测试工具配置文件isr_list.cfg

python2 版本
'''

import sys
import json

is_utf8bom = False

cfg_file = sys.argv[1]

if sys.argv[-1] != 'bom':
    cfgs = sys.argv[2:]
else:
    cfgs = sys.argv[2:-1]
    is_utf8bom = True


def byteify(input):
    if isinstance(input, dict):
        return {byteify(key): byteify(value) for key, value in input.iteritems()}
    elif isinstance(input, list):
        return [byteify(element) for element in input]
    elif isinstance(input, unicode):
        return input.encode('utf-8')
    else:
        return input

with open(cfg_file) as fp:
    if not is_utf8bom:
        cfg_data = json.loads(fp.read().decode('utf-8'))
    else:
        cfg_data = json.loads(fp.read()[3:].decode('utf-8'))
    cfg_data = byteify(cfg_data)

for cfg in cfgs:
    cfg_name = cfg.split(':')[0]
    cfg_value = cfg.split(':')[1]

    try:
        cfg_type = cfg.split(':')[2]
        cfg_data[cfg_name] = int(cfg_value)
    except IndexError:
        cfg_data[cfg_name] = cfg_value

with open(cfg_file, 'w') as fp:
    a = json.dumps(cfg_data, ensure_ascii=False, indent=2)

    fp.write(a)
