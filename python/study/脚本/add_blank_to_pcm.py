#coding=utf-8

import os
import sys

'''
!!!use python2 to run this script!!!

传入保存音频的文件夹和需添加的尾静音的毫秒数作为参数。
脚本会处理文件夹下所有的wav或pcm格式文件，若包含RIFF头则会去头。
'''

handle_dir = sys.argv[1]
add_size = (int(sys.argv[2]))*32

print('add blank:',add_size)

try:
    for root,dirs,files in os.walk(handle_dir):
        for file in files:

            if file.endswith('pcm') or file.endswith('wav'):
                print('handling file:',file)
                data = None

                with open(os.path.join(root, file),'rb') as fp:
                    data = fp.read()
                
                with open(os.path.join(root, file), 'wb') as fp:
                    if data[0]=='R' and data[1]=='I' and data[2]=='F' and data[3]=='F':
                        fp.write(data[44:])
                    else:
                        fp.write(data)
                    
                    for i in range(add_size):
                        fp.write('\0')
except TypeError:
    print('\n!!!use python2 to run this script!!!')

