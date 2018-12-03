#coding=utf-8

import pprint
import collections

data = collections.OrderedDict()
ret = []

def fun(name, up_code):
    if up_code == '0':
        ret.append(name)
    else:
        if data.has_key(up_code):
            name = data[up_code][0]+name
            fun(name, data[up_code][1])
        else:
            ret.append(name)

with open('fullname.txt') as fp:
    for line in fp:
        texts = [i.strip() for i in line.split('\t')]
        data[texts[0]]=(texts[1], texts[2])

pprint.pprint(data)

for key in data:
    fun(data[key][0], data[key][1])

with open('ret.txt', 'w') as ret_fp:
    for line in ret:
        ret_fp.write(line+'\n')