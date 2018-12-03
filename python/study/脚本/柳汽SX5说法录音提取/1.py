#coding=utf-8

import xlrd
import xlwt3
import sys
import re

def get_real_pcm_path(path):
    match = re.match('.*6000_Enc_NoWavHead\\\\(.*)\\.pcm', path)

    pcm_dir = '\\\\192.168.24.29\\车载测试集建设\\识别测试集\\6000原始录音_高保真\\' + match.group(1).split('\\')[0] + '\\'
    pcm_file = match.group(1).split('\\')[1].split('_')

    return pcm_dir, pcm_file

def get_label_pcms():
    # 获取音频集
    data = xlrd.open_workbook('6000录音对应表_coder.xlsx')
    table = data.sheets()[0]

    values = {}
    for i in range(1, table.nrows):
        label = table.cell(i, 2).value
        pcm = table.cell(i, 0).value

        if not label in values:
            values[label] = []
        values[label].append(pcm)

    return values

def fun(values):
    ret_file = xlwt3.Workbook()
    table_in = ret_file.add_sheet('集内说法')
    table_out = ret_file.add_sheet('集外说法')

    in_row_num = 0
    out_row_num = 0

    with open('柳汽SX5说法.txt', encoding='utf-8') as fp:
        for line in fp:
            line = line.strip()

            if line in values:
                table_in.write(in_row_num, 0, line)
                pcm_num = 1
                for pcm in values[line]:
                    table_in.write(in_row_num, pcm_num, pcm)
                    pcm_num += 1
                in_row_num += 1
            else:
                table_out.write(out_row_num, 0, line)
                out_row_num += 1

    ret_file.save('柳汽SX5说法录音.xls')

if __name__ == '__main__':
    fun(get_label_pcms())
    # print(get_real_pcm_path('\\StorageCard\\PcmRes\\isr\\6000_Enc_NoWavHead\\23traffic\\23traffic_212_F029_coder.wav.pcm'))
