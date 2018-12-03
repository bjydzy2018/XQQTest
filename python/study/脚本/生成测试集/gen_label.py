#coding=utf-8

import re
import sys
import pickle

label_file = sys.argv[1]
lang = sys.argv[2]

label_dict = {}
pattern = r'"\*/(.*)_LSA.*'

with open(label_file) as fp:
    in_file = False
    pcm_label = ''

    for line in fp:
        line = line.strip()

        if not in_file:
            match = re.match(pattern, line)
            if match:
                pcm_name = match.group(1)
                in_file = True
        else:
            if line != '.':
                if lang=='ch':
                    pcm_label += line
                elif lang=='en':
                    pcm_label += line+' '
            else:
                label_dict[pcm_name] = pcm_label.strip()
                pcm_label = ''
                in_file = False

with open('label_text.txt', 'w', encoding='utf-8') as fp:
    for pcm_name in label_dict:
        fp.write(pcm_name+'\t'+label_dict[pcm_name]+'\n')

pickle.dump(label_dict, open('label_text.pickle', 'wb'))

print(label_dict)   