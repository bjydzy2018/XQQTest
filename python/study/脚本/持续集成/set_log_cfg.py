#coding=utf-8

import ConfigParser
import sys
import os
from os.path import exists

cf = ConfigParser.ConfigParser()
cfg_file = sys.argv[1]
cfgs = sys.argv[2:]

cf.read(cfg_file)

sections = cf.sections()
# 每个log配置文件仅有一个section
section = sections[0]

cfg_file_bak = cfg_file+'.bak'
if not exists(cfg_file_bak):
    with open(cfg_file_bak, 'wb') as fp_bak:
        with open(cfg_file, 'rb') as fp:
            fp_bak.write(fp.read())

    print '%s is copied to %s' % (cfg_file, cfg_file_bak)
else:
    print '%s exists, and will not copy %s' % (cfg_file_bak, cfg_file)

for cfg in cfgs:
    cfg_name = cfg.split(':')[0]
    cfg_value = cfg.split(':')[1]

    cfg_data = cf.get(section, cfg_name)
    print 'the current %s is: %s, and is set to %s' % (cfg_name, cfg_data, cfg_value)

    cf.set(section, cfg_name, cfg_value)

cf.write(open(cfg_file, 'w'))
