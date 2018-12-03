# coding=utf-8

import pickle
import re
from collections import defaultdict
import sys

lang = sys.argv[1]

pattern = r'.*[MF]\d{3}/(.*).wav'
ret = defaultdict(dict)

label = pickle.load(open('label_text.pickle', 'rb'))
service = pickle.load(open('service.pickle', 'rb'))

with open('pcm_list.txt', encoding='utf-8') as fp:
    for line in fp:
        line = line.strip()

        match = re.match(pattern, line)
        if match:
            ret[match.group(1)]['path'] = line.replace('\\', '/')

for pcm_name in label:
    ret[pcm_name]['label'] = label[pcm_name]

for pcm_name in service:
    ret[pcm_name]['service'] = service[pcm_name]

if lang == 'ch':
    with open('label.csv', 'w') as fp:
        fp.write('列表,业务,云端标注,本地标注\n')
        for pcm_name in ret:
            if 'path' in ret[pcm_name]:
                print(ret[pcm_name]['path'], ret[pcm_name]['service'] if 'service' in ret[
                    pcm_name] else '', ret[pcm_name]['label'], ret[pcm_name]['label'], sep=',', file=fp)
elif lang == 'en':
    with open('label_en.csv', 'w') as fp:
        fp.write('录音,业务,录音,标注\n')
        num = 1

        for pcm_name in ret:
            if 'path' in ret[pcm_name]:
                print(str(num), ret[pcm_name]['service'] if 'service' in ret[
                    pcm_name] else '', ret[pcm_name]['path'], ret[pcm_name]['label'], sep=',', file=fp)
                num += 1

print(ret)