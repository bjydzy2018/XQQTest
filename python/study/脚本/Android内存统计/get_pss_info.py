# coding=utf-8

'''
使用adb shell dumpsys meminfo命令获取安卓包的Native和Dalvik内存信息。

使用时传入需要检测的app包名作为脚本参数。
'''

import os
import time
import re
import sys

# 要检测的程序的包名
pkg_name = sys.argv[1]

pattern_native_heap_alloc = 'Native\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\d+)\\s+\\d.*'
pattern_dalvik_heap_alloc = 'Dalvik\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\d+)\\s+\\d.*'
pattern_native_pss = 'Native\\s+(\\d+)\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d.*'
pattern_dalvik_pss = 'Dalvik\\s+(\\d+)\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d.*'

fp_ret_raw = open('pss_info_raw.txt', 'w')

with open('pss_info.csv', 'w') as fp:
    fp.write('native_heap_alloc,native_pss,dalvik_heap_alloc,dalvik_pss\n')

    while True:
        data = os.popen('adb shell dumpsys meminfo ' + pkg_name).readlines()
        # data_raw = os.popen('adb shell dumpsys meminfo ' + pkg_name).read()
        fp_ret_raw.write(''.join(data) + '\n')
        if 'No process found for' in data[0]:
            break

        native_heap_alloc = None
        native_pss = None
        dalvik_heap_alloc = None
        dalvik_pss = None

        for line in data:
            line = line.strip()

            if 'Native' in line:
                native_heap_alloc = re.match(
                    pattern_native_heap_alloc, line).group(1)
                native_pss = re.match(pattern_native_pss, line).group(1)

            if 'Dalvik' in line:
                dalvik_heap_alloc = re.match(
                    pattern_dalvik_heap_alloc, line).group(1)
                dalvik_pss = re.match(pattern_dalvik_pss, line).group(1)

        fp.write(native_heap_alloc + ',' + native_pss + ',' +
                 dalvik_heap_alloc + ',' + dalvik_pss + '\n')
        print(native_heap_alloc + ',' + native_pss +
              ',' + dalvik_heap_alloc + ',' + dalvik_pss)

        time.sleep(1)

fp_ret_raw.close()
print('done')
