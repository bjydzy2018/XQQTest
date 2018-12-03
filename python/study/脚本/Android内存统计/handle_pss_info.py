# coding=utf-8

'''
处理get_pss_info.py获取的安卓app内存使用情况统计结果文本。
'''

import re
import sys

pattern_native_pss = 'Native\\s+(\\d+)\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d.*'
pattern_native_shared_dirty = 'Native\\s+\\d+\\s+(\\d+)\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d.*'
pattern_native_private_dirty = 'Native\\s+\\d+\\s+\\d+\\s+(\\d+)\\s+\\d+\\s+\\d+\\s+\\d.*'
pattern_native_heap_size = 'Native\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\d+)\\s+\\d+\\s+\\d.*'
pattern_native_heap_alloc = 'Native\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\d+)\\s+\\d.*'
pattern_native_heap_free = 'Native\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\d+).*'

pattern_dalvik_pss = 'Dalvik\\s+(\\d+)\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d.*'
pattern_dalvik_shared_dirty = 'Dalvik\\s+\\d+\\s+(\\d+)\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d.*'
pattern_dalvik_private_dirty = 'Dalvik\\s+\\d+\\s+\\d+\\s+(\\d+)\\s+\\d+\\s+\\d+\\s+\\d.*'
pattern_dalvik_heap_size = 'Dalvik\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\d+)\\s+\\d+\\s+\\d.*'
pattern_dalvik_heap_alloc = 'Dalvik\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\d+)\\s+\\d.*'
pattern_dalvik_heap_free = 'Dalvik\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\d+).*'

pattern_stack_pss = 'Stack\\s+(\\d+)\\s+\\d+\\s+\\d+.*'
pattern_stack_shared_dirty = 'Stack\\s+\\d+\\s+(\\d+)\\s+\\d+.*'
pattern_stack_private_dirty = 'Stack\\s+\\d+\\s+\\d+\\s+(\\d+).*'

pattern_cursor_pss = 'Cursor\\s+(\\d+)\\s+\\d+\\s+\\d+.*'
pattern_cursor_shared_dirty = 'Cursor\\s+\\d+\\s+(\\d+)\\s+\\d+.*'
pattern_cursor_private_dirty = 'Cursor\\s+\\d+\\s+\\d+\\s+(\\d+).*'

pattern_ashmem_pss = 'Ashmem\\s+(\\d+)\\s+\\d+\\s+\\d+.*'
pattern_ashmem_shared_dirty = 'Ashmem\\s+\\d+\\s+(\\d+)\\s+\\d+.*'
pattern_ashmem_private_dirty = 'Ashmem\\s+\\d+\\s+\\d+\\s+(\\d+).*'

pattern_other_dev_pss = 'Other dev\\s+(\\d+)\\s+\\d+\\s+\\d+.*'
pattern_other_dev_shared_dirty = 'Other dev\\s+\\d+\\s+(\\d+)\\s+\\d+.*'
pattern_other_dev_private_dirty = 'Other dev\\s+\\d+\\s+\\d+\\s+(\\d+).*'

pattern_so_mmap_pss = '\\.so mmap\\s+(\\d+)\\s+\\d+\\s+\\d+.*'
pattern_so_mmap_shared_dirty = '\\.so mmap\\s+\\d+\\s+(\\d+)\\s+\\d+.*'
pattern_so_mmap_private_dirty = '\\.so mmap\\s+\\d+\\s+\\d+\\s+(\\d+).*'

pattern_jar_mmap_pss = '\\.jar mmap\\s+(\\d+)\\s+\\d+\\s+\\d+.*'
pattern_jar_mmap_shared_dirty = '\\.jar mmap\\s+\\d+\\s+(\\d+)\\s+\\d+.*'
pattern_jar_mmap_private_dirty = '\\.jar mmap\\s+\\d+\\s+\\d+\\s+(\\d+).*'

pattern_apk_mmap_pss = '\\.apk mmap\\s+(\\d+)\\s+\\d+\\s+\\d+.*'
pattern_apk_mmap_shared_dirty = '\\.apk mmap\\s+\\d+\\s+(\\d+)\\s+\\d+.*'
pattern_apk_mmap_private_dirty = '\\.apk mmap\\s+\\d+\\s+\\d+\\s+(\\d+).*'

pattern_ttf_mmap_pss = '\\.ttf mmap\\s+(\\d+)\\s+\\d+\\s+\\d+.*'
pattern_ttf_mmap_shared_dirty = '\\.ttf mmap\\s+\\d+\\s+(\\d+)\\s+\\d+.*'
pattern_ttf_mmap_private_dirty = '\\.ttf mmap\\s+\\d+\\s+\\d+\\s+(\\d+).*'

pattern_dex_mmap_pss = '\\.dex mmap\\s+(\\d+)\\s+\\d+\\s+\\d+.*'
pattern_dex_mmap_shared_dirty = '\\.dex mmap\\s+\\d+\\s+(\\d+)\\s+\\d+.*'
pattern_dex_mmap_private_dirty = '\\.dex mmap\\s+\\d+\\s+\\d+\\s+(\\d+).*'

pattern_other_mmap_pss = 'Other mmap\\s+(\\d+)\\s+\\d+\\s+\\d+.*'
pattern_other_mmap_shared_dirty = 'Other mmap\\s+\\d+\\s+(\\d+)\\s+\\d+.*'
pattern_other_mmap_private_dirty = 'Other mmap\\s+\\d+\\s+\\d+\\s+(\\d+).*'

pattern_unknown_pss = 'Unknown\\s+(\\d+)\\s+\\d+\\s+\\d+.*'
pattern_unknown_shared_dirty = 'Unknown\\s+\\d+\\s+(\\d+)\\s+\\d+.*'
pattern_unknown_private_dirty = 'Unknown\\s+\\d+\\s+\\d+\\s+(\\d+).*'

pattern_TOTAL_pss = 'TOTAL\\s+(\\d+)\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d.*'
pattern_TOTAL_shared_dirty = 'TOTAL\\s+\\d+\\s+(\\d+)\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d.*'
pattern_TOTAL_private_dirty = 'TOTAL\\s+\\d+\\s+\\d+\\s+(\\d+)\\s+\\d+\\s+\\d+\\s+\\d.*'
pattern_TOTAL_heap_size = 'TOTAL\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\d+)\\s+\\d+\\s+\\d.*'
pattern_TOTAL_heap_alloc = 'TOTAL\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\d+)\\s+\\d.*'
pattern_TOTAL_heap_free = 'TOTAL\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\d+).*'

native_pss = None
native_shared_dirty = None
native_private_dirty = None
native_heap_size = None
native_heap_alloc = None
native_heap_free = None

dalvik_pss = None
dalvik_shared_dirty = None
dalvik_private_dirty = None
dalvik_heap_size = None
dalvik_heap_alloc = None
dalvik_heap_free = None

stack_pss = None
stack_shared_dirty = None
stack_private_dirty = None

cursor_pss = None
cursor_shared_dirty = None
cursor_private_dirty = None

ashmem_pss = None
ashmem_shared_dirty = None
ashmem_private_dirty = None

other_dev_pss = None
other_dev_shared_dirty = None
other_dev_private_dirty = None

so_mmap_pss = None
so_mmap_shared_dirty = None
so_mmap_private_dirty = None

jar_mmap_pss = None
jar_mmap_shared_dirty = None
jar_mmap_private_dirty = None

apk_mmap_pss = None
apk_mmap_shared_dirty = None
apk_mmap_private_dirty = None

ttf_mmap_pss = None
ttf_mmap_shared_dirty = None
ttf_mmap_private_dirty = None

dex_mmap_pss = None
dex_mmap_shared_dirty = None
dex_mmap_private_dirty = None

other_mmap_pss = None
other_mmap_shared_dirty = None
other_mmap_private_dirty = None

unknown_pss = None
unknown_shared_dirty = None
unknown_private_dirty = None

TOTAL_pss = None
TOTAL_shared_dirty = None
TOTAL_private_dirty = None
TOTAL_heap_size = None
TOTAL_heap_alloc = None
TOTAL_heap_free = None

fp_ret = open('pss_info_handled.csv', 'w')
fp_ret.write('native_pss,native_shared_dirty,native_private_dirty,native_heap_size,native_heap_alloc,native_heap_free,dalvik_pss,dalvik_shared_dirty,dalvik_private_dirty,dalvik_heap_size,dalvik_heap_alloc,dalvik_heap_free,stack_pss,stack_shared_dirty,stack_private_dirty,cursor_pss,cursor_shared_dirty,cursor_private_dirty,ashmem_pss,ashmem_shared_dirty,ashmem_private_dirty,other_dev_pss,other_dev_shared_dirty,other_dev_private_dirty,so_mmap_pss,so_mmap_shared_dirty,so_mmap_private_dirty,jar_mmap_pss,jar_mmap_shared_dirty,jar_mmap_private_dirty,apk_mmap_pss,apk_mmap_shared_dirty,apk_mmap_private_dirty,ttf_mmap_pss,ttf_mmap_shared_dirty,ttf_mmap_private_dirty,dex_mmap_pss,dex_mmap_shared_dirty,dex_mmap_private_dirty,other_mmap_pss,other_mmap_shared_dirty,other_mmap_private_dirty,unknown_pss,unknown_shared_dirty,unknown_private_dirty,TOTAL_pss,TOTAL_shared_dirty,TOTAL_private_dirty,TOTAL_heap_size,TOTAL_heap_alloc,TOTAL_heap_free\n')

with open('pss_info_raw.txt') as fp:
    for line in fp:
        line = line.strip()

        native_pss = re.match(pattern_native_pss, line).group(
            1) if re.match(pattern_native_pss, line) != None else native_pss
        native_shared_dirty = re.match(pattern_native_shared_dirty, line).group(
            1) if re.match(pattern_native_shared_dirty, line) != None else native_shared_dirty
        native_private_dirty = re.match(pattern_native_private_dirty, line).group(
            1) if re.match(pattern_native_private_dirty, line) != None else native_private_dirty
        native_heap_size = re.match(pattern_native_heap_size, line).group(
            1) if re.match(pattern_native_heap_size, line) != None else native_heap_size
        native_heap_alloc = re.match(pattern_native_heap_alloc, line).group(
            1) if re.match(pattern_native_heap_alloc, line) != None else native_heap_alloc
        native_heap_free = re.match(pattern_native_heap_free, line).group(
            1) if re.match(pattern_native_heap_free, line) != None else native_heap_free

        dalvik_pss = re.match(pattern_dalvik_pss, line).group(
            1) if re.match(pattern_dalvik_pss, line) != None else dalvik_pss
        dalvik_shared_dirty = re.match(pattern_dalvik_shared_dirty, line).group(
            1) if re.match(pattern_dalvik_shared_dirty, line) != None else dalvik_shared_dirty
        dalvik_private_dirty = re.match(pattern_dalvik_private_dirty, line).group(
            1) if re.match(pattern_dalvik_private_dirty, line) != None else dalvik_private_dirty
        dalvik_heap_size = re.match(pattern_dalvik_heap_size, line).group(
            1) if re.match(pattern_dalvik_heap_size, line) != None else dalvik_heap_size
        dalvik_heap_alloc = re.match(pattern_dalvik_heap_alloc, line).group(
            1) if re.match(pattern_dalvik_heap_alloc, line) != None else dalvik_heap_alloc
        dalvik_heap_free = re.match(pattern_dalvik_heap_free, line).group(
            1) if re.match(pattern_dalvik_heap_free, line) != None else dalvik_heap_free

        stack_pss = re.match(pattern_stack_pss, line).group(
            1) if re.match(pattern_stack_pss, line) != None else stack_pss
        stack_shared_dirty = re.match(pattern_stack_shared_dirty, line).group(
            1) if re.match(pattern_stack_shared_dirty, line) != None else stack_shared_dirty
        stack_private_dirty = re.match(pattern_stack_private_dirty, line).group(
            1) if re.match(pattern_stack_private_dirty, line) != None else stack_private_dirty

        cursor_pss = re.match(pattern_cursor_pss, line).group(
            1) if re.match(pattern_cursor_pss, line) != None else cursor_pss
        cursor_shared_dirty = re.match(pattern_cursor_shared_dirty, line).group(
            1) if re.match(pattern_cursor_shared_dirty, line) != None else cursor_shared_dirty
        cursor_private_dirty = re.match(pattern_cursor_private_dirty, line).group(
            1) if re.match(pattern_cursor_private_dirty, line) != None else cursor_private_dirty

        ashmem_pss = re.match(pattern_ashmem_pss, line).group(
            1) if re.match(pattern_ashmem_pss, line) != None else ashmem_pss
        ashmem_shared_dirty = re.match(pattern_ashmem_shared_dirty, line).group(
            1) if re.match(pattern_ashmem_shared_dirty, line) != None else ashmem_shared_dirty
        ashmem_private_dirty = re.match(pattern_ashmem_private_dirty, line).group(
            1) if re.match(pattern_ashmem_private_dirty, line) != None else ashmem_private_dirty

        other_dev_pss = re.match(pattern_other_dev_pss, line).group(
            1) if re.match(pattern_other_dev_pss, line) != None else other_dev_pss
        other_dev_shared_dirty = re.match(pattern_other_dev_shared_dirty, line).group(
            1) if re.match(pattern_other_dev_shared_dirty, line) != None else other_dev_shared_dirty
        other_dev_private_dirty = re.match(pattern_other_dev_private_dirty, line).group(
            1) if re.match(pattern_other_dev_private_dirty, line) != None else other_dev_private_dirty

        so_mmap_pss = re.match(pattern_so_mmap_pss, line).group(
            1) if re.match(pattern_so_mmap_pss, line) != None else so_mmap_pss
        so_mmap_shared_dirty = re.match(pattern_so_mmap_shared_dirty, line).group(
            1) if re.match(pattern_so_mmap_shared_dirty, line) != None else so_mmap_shared_dirty
        so_mmap_private_dirty = re.match(pattern_so_mmap_private_dirty, line).group(
            1) if re.match(pattern_so_mmap_private_dirty, line) != None else so_mmap_private_dirty

        jar_mmap_pss = re.match(pattern_jar_mmap_pss, line).group(
            1) if re.match(pattern_jar_mmap_pss, line) != None else jar_mmap_pss
        jar_mmap_shared_dirty = re.match(pattern_jar_mmap_shared_dirty, line).group(
            1) if re.match(pattern_jar_mmap_shared_dirty, line) != None else jar_mmap_shared_dirty
        jar_mmap_private_dirty = re.match(pattern_jar_mmap_private_dirty, line).group(
            1) if re.match(pattern_jar_mmap_private_dirty, line) != None else jar_mmap_private_dirty

        apk_mmap_pss = re.match(pattern_apk_mmap_pss, line).group(
            1) if re.match(pattern_apk_mmap_pss, line) != None else apk_mmap_pss
        apk_mmap_shared_dirty = re.match(pattern_apk_mmap_shared_dirty, line).group(
            1) if re.match(pattern_apk_mmap_shared_dirty, line) != None else apk_mmap_shared_dirty
        apk_mmap_private_dirty = re.match(pattern_apk_mmap_private_dirty, line).group(
            1) if re.match(pattern_apk_mmap_private_dirty, line) != None else apk_mmap_private_dirty

        ttf_mmap_pss = re.match(pattern_ttf_mmap_pss, line).group(
            1) if re.match(pattern_ttf_mmap_pss, line) != None else ttf_mmap_pss
        ttf_mmap_shared_dirty = re.match(pattern_ttf_mmap_shared_dirty, line).group(
            1) if re.match(pattern_ttf_mmap_shared_dirty, line) != None else ttf_mmap_shared_dirty
        ttf_mmap_private_dirty = re.match(pattern_ttf_mmap_private_dirty, line).group(
            1) if re.match(pattern_ttf_mmap_private_dirty, line) != None else ttf_mmap_private_dirty

        dex_mmap_pss = re.match(pattern_dex_mmap_pss, line).group(
            1) if re.match(pattern_dex_mmap_pss, line) != None else dex_mmap_pss
        dex_mmap_shared_dirty = re.match(pattern_dex_mmap_shared_dirty, line).group(
            1) if re.match(pattern_dex_mmap_shared_dirty, line) != None else dex_mmap_shared_dirty
        dex_mmap_private_dirty = re.match(pattern_dex_mmap_private_dirty, line).group(
            1) if re.match(pattern_dex_mmap_private_dirty, line) != None else dex_mmap_private_dirty

        other_mmap_pss = re.match(pattern_other_mmap_pss, line).group(
            1) if re.match(pattern_other_mmap_pss, line) != None else other_mmap_pss
        other_mmap_shared_dirty = re.match(pattern_other_mmap_shared_dirty, line).group(
            1) if re.match(pattern_other_mmap_shared_dirty, line) != None else other_mmap_shared_dirty
        other_mmap_private_dirty = re.match(pattern_other_mmap_private_dirty, line).group(
            1) if re.match(pattern_other_mmap_private_dirty, line) != None else other_mmap_private_dirty

        unknown_pss = re.match(pattern_unknown_pss, line).group(
            1) if re.match(pattern_unknown_pss, line) != None else unknown_pss
        unknown_shared_dirty = re.match(pattern_unknown_shared_dirty, line).group(
            1) if re.match(pattern_unknown_shared_dirty, line) != None else unknown_shared_dirty
        unknown_private_dirty = re.match(pattern_unknown_private_dirty, line).group(
            1) if re.match(pattern_unknown_private_dirty, line) != None else unknown_private_dirty

        TOTAL_pss = re.match(pattern_TOTAL_pss, line).group(
            1) if re.match(pattern_TOTAL_pss, line) != None else TOTAL_pss
        TOTAL_shared_dirty = re.match(pattern_TOTAL_shared_dirty, line).group(
            1) if re.match(pattern_TOTAL_shared_dirty, line) != None else TOTAL_shared_dirty
        TOTAL_private_dirty = re.match(pattern_TOTAL_private_dirty, line).group(
            1) if re.match(pattern_TOTAL_private_dirty, line) != None else TOTAL_private_dirty
        TOTAL_heap_size = re.match(pattern_TOTAL_heap_size, line).group(
            1) if re.match(pattern_TOTAL_heap_size, line) != None else TOTAL_heap_size
        TOTAL_heap_alloc = re.match(pattern_TOTAL_heap_alloc, line).group(
            1) if re.match(pattern_TOTAL_heap_alloc, line) != None else TOTAL_heap_alloc
        TOTAL_heap_free = re.match(pattern_TOTAL_heap_free, line).group(
            1) if re.match(pattern_TOTAL_heap_free, line) != None else TOTAL_heap_free

        if 'PAGECACHE_OVERFLOW' in line:
            fp_ret.write(native_pss + ',' + native_shared_dirty + ',' + native_private_dirty +
                         ',' + native_heap_size + ',' + native_heap_alloc + ',' + native_heap_free)
            fp_ret.write(',' + dalvik_pss + ',' + dalvik_shared_dirty + ',' + dalvik_private_dirty +
                         ',' + dalvik_heap_size + ',' + dalvik_heap_alloc + ',' + dalvik_heap_free)
            fp_ret.write(',' + stack_pss + ',' + stack_shared_dirty +
                         ',' + stack_private_dirty)
            fp_ret.write(',' + cursor_pss + ',' + cursor_shared_dirty +
                         ',' + cursor_private_dirty)
            fp_ret.write(',' + ashmem_pss + ',' + ashmem_shared_dirty +
                         ',' + ashmem_private_dirty)
            fp_ret.write(',' + other_dev_pss + ',' + other_dev_shared_dirty +
                         ',' + other_dev_private_dirty)
            fp_ret.write(',' + so_mmap_pss + ',' + so_mmap_shared_dirty +
                         ',' + so_mmap_private_dirty)
            fp_ret.write(',' + jar_mmap_pss + ',' + jar_mmap_shared_dirty +
                         ',' + jar_mmap_private_dirty)
            fp_ret.write(',' + apk_mmap_pss + ',' + apk_mmap_shared_dirty +
                         ',' + apk_mmap_private_dirty)
            fp_ret.write(',' + ttf_mmap_pss + ',' + ttf_mmap_shared_dirty +
                         ',' + ttf_mmap_private_dirty)
            fp_ret.write(',' + dex_mmap_pss + ',' + dex_mmap_shared_dirty +
                         ',' + dex_mmap_private_dirty)
            fp_ret.write(',' + other_mmap_pss + ',' + other_mmap_shared_dirty +
                         ',' + other_mmap_private_dirty)
            fp_ret.write(',' + unknown_pss + ',' + unknown_shared_dirty +
                         ',' + unknown_private_dirty)
            fp_ret.write(',' + TOTAL_pss + ',' + TOTAL_shared_dirty + ',' + TOTAL_private_dirty +
                         ',' + TOTAL_heap_size + ',' + TOTAL_heap_alloc + ',' + TOTAL_heap_free + '\n')

fp_ret.close()
