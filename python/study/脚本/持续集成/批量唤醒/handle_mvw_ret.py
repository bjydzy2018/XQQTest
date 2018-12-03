#coding=utf-8

'''
统计安卓平台唤醒结果

传入保存结果的路径作为参数

脚本生成统计excel文件
'''
__author__ = 'hcliu'

import os
import sys
import xlrd
import xlwt
import re
import json
import pprint
import collections
from change_file_encode2utf8_py3 import explore

def handle_ret(handle_dir):
    # 遍历保存结果的文件夹，将唤醒结果存入到ret字典中
    ret = collections.OrderedDict()
    ret['scenes'] = collections.OrderedDict()
    sum_vw_time = 0.0
    sum_vw_num = 0

    explore(handle_dir, True)

    for root,dirs,files in os.walk(handle_dir):
        for file in files:
            if file.endswith('ret.txt'):

                with open(os.path.join(root, file), encoding="utf-8") as fp:
                    temp_ret = {}

                    for line in fp:
                        line = line.strip()
                        vw_match = re.match('.*SceneId-(\d).*', line)

                        if vw_match:
                            # 处理每条唤醒结果
                            vw_ret = line.split('\t')[1] if len(line.split('\t'))>1 else None

                            try:
                                temp_ret[line.split('\t')[0]] = None if not vw_ret else json.loads(re.match('.*lParam:(.*)', line.split('\t')[1]).group(1))
                            except Exception as err:
                                print(line)
                                print(err)
                                sys.exit(0)
                        elif line.startswith('vwLabel'):
                            scene_label = re.match('vwLabel：(\d).*', line).group(1)
                            vwword_label = re.match('vwLabel：(\d-.*), 尝试唤醒.*', line).group(1)
                            vwTry_time = re.match('.*尝试唤醒次数[：|:](.*), .*', line).group(1)
                            vwSuccess_time = re.match('.*唤醒成功次数[：|:](.*)，成功率.*', line).group(1)
                            vwSuccess_rate = re.match('.*成功率[：|:](.*%).*', line).group(1)
                            vwTime = re.match('.*平均唤醒响应时间[：|:](.*)', line).group(1) if '响应时间'in line else None
                            
                            if vwTime:
                                sum_vw_time += float(vwTime)
                                sum_vw_num += 1

                            if scene_label not in ret['scenes']:
                                ret['scenes'][scene_label] = {}
                            ret['scenes'][scene_label][vwword_label] = {}
                            ret['scenes'][scene_label][vwword_label]['vwRet'] = temp_ret
                            ret['scenes'][scene_label][vwword_label]['vwTry_time'] = vwTry_time
                            ret['scenes'][scene_label][vwword_label]['vwSuccess_time'] = vwSuccess_time
                            ret['scenes'][scene_label][vwword_label]['vwSuccess_rate'] = vwSuccess_rate
                            ret['scenes'][scene_label][vwword_label]['vwTime'] = vwTime

    ret['vwTime_all'] = '%.2f' % (sum_vw_time/sum_vw_num)

    return ret

if __name__ == '__main__':
    handle_dir = sys.argv[1]
    ret = handle_ret(handle_dir)

    ret_file = xlwt.Workbook()
    table_sum = ret_file.add_sheet('统计')
    table_sum.write(0, 0, '场景')
    table_sum.write(0, 1, '唤醒词')
    table_sum.write(0, 2, '音频总数')
    table_sum.write(0, 3, '唤醒次数')
    table_sum.write(0, 4, '唤醒率')
    table_sum.write(0, 5, '平均唤醒响应时间')
    table_sum.write(0, 6, '总平均唤醒响应时间')
    table_sum.write(1, 6, ret['vwTime_all'])

    sum_row = 1
    for scene in ret['scenes']:
        cur_scene = 'SceneId-'+scene

        for vw_word in ret['scenes'][scene]:
            table = ret_file.add_sheet(scene+'-'+vw_word)
            table.write(0, 0, 'id')
            table.write(0, 1, 'pcm_path')
            table.write(0, 2, 'nStartBytes')
            table.write(0, 3, 'nEndBytes')
            table.write(0, 4, 'nMvwScene')
            table.write(0, 5, 'nMvwId')
            table.write(0, 6, 'nMvwScore')
            table.write(0, 8, '尝试唤醒次数')
            table.write(0, 9, '唤醒成功次数')
            table.write(0, 10, '唤醒率')
            table.write(0, 11, '平均唤醒响应时间')
            table.write(1, 8, ret['scenes'][scene][vw_word]['vwTry_time'])
            table.write(1, 9, ret['scenes'][scene][vw_word]['vwSuccess_time'])
            table.write(1, 10, ret['scenes'][scene][vw_word]['vwSuccess_rate'])
            table.write(1, 11, ret['scenes'][scene][vw_word]['vwTime'])
            table_sum.write(sum_row, 0, cur_scene)
            table_sum.write(sum_row, 1, vw_word)
            table_sum.write(sum_row, 2, ret['scenes'][scene][vw_word]['vwTry_time'])
            table_sum.write(sum_row, 3, ret['scenes'][scene][vw_word]['vwSuccess_time'])
            table_sum.write(sum_row, 4, ret['scenes'][scene][vw_word]['vwSuccess_rate'])
            table_sum.write(sum_row, 5, ret['scenes'][scene][vw_word]['vwTime'])

            cur_row = 1
            cur_id = 1
            for pcm_path in ret['scenes'][scene][vw_word]['vwRet']:
                table.write(cur_row, 0, cur_id)
                table.write(cur_row, 1, pcm_path)
                if ret['scenes'][scene][vw_word]['vwRet'][pcm_path]:
                    table.write(cur_row, 2, ret['scenes'][scene][vw_word]['vwRet'][pcm_path]["nStartBytes"])
                    table.write(cur_row, 3, ret['scenes'][scene][vw_word]['vwRet'][pcm_path]["nEndBytes"])
                    table.write(cur_row, 4, ret['scenes'][scene][vw_word]['vwRet'][pcm_path]['nMvwScene'])
                    table.write(cur_row, 5, ret['scenes'][scene][vw_word]['vwRet'][pcm_path]['nMvwId'])
                    table.write(cur_row, 6, ret['scenes'][scene][vw_word]['vwRet'][pcm_path]['nMvwScore'])

                cur_id += 1
                cur_row += 1

            sum_row += 1

    ret_file.save(os.path.join(handle_dir, 'vw_ret_android.xls'))
