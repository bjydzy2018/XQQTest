# coding=utf-8

__author__ = 'hcliu'

'''
将配置文件重写为utf-8格式。

python2
'''

import sys
import chardet


def convert_cfg_to_u8(f):
    cfg_data_encoded = None

    with open(f) as fp:
        cfg_data = fp.read()
        cfg_data_encoded = cfg_data.decode(
            (chardet.detect(cfg_data))['encoding'])

    with open(f, 'w') as fp:
        fp.write(cfg_data_encoded.encode('utf-8'))

if __name__ == '__main__':
    convert_cfg_to_u8(sys.argv[1])
