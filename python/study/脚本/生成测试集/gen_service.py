#coding=utf-8

import re
import sys
import pickle
import os

service_src_dir = sys.argv[1]

service_dict = {}
pattern_pcm = r'.*\\(.*)_LSA.*'
pattern_service = r'.*(?:local|cloude)_(.*)\.scp'

for root,dirs,files in os.walk(service_src_dir):
    for file in files:
        file_path = os.path.join(root, file)
        match = re.match(pattern_service, file)
        service_name = match.group(1)

        with open(file_path, encoding='utf-8') as fp:
            for line in fp:
                line = line.strip()

                match = re.match(pattern_pcm, line)
                if match:
                    service_dict[match.group(1)] = service_name

pickle.dump(service_dict, open('service.pickle', 'wb'))

print(service_dict)
