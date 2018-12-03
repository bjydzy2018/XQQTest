# coding=utf-8

__author__ = 'hcliu'

'''
制作制定量级的音乐词典。

完成后需要将生成词典文件中的\"的\去掉。
'''

import sys
import xlrd
import json
import XLSEarrow


def gen_dict(dictname, dictcontant):
    data = {}
    data['dictname'] = dictname

    data['dictcontant'] = []
    num = 0
    for contant in dictcontant:
        temp_dict = {}
        temp_dict['id'] = num
        temp_dict['name'] = contant

        data['dictcontant'].append(temp_dict)
        num += 1

    return data

dict_xls = sys.argv[1]
dict_sheet = sys.argv[2]
dict_nums = sys.argv[3]

dict_workbook = xlrd.open_workbook(dict_xls)
sheet = dict_workbook.sheet_by_name(dict_sheet)

blank_ret = open('blank_ret.txt', 'w')

for dict_num in dict_nums.split(','):
    dict_num = int(dict_num)

    songs = []
    singers = []
    albums = []

    songs_blank = 0
    singers_blank = 0
    albums_blank = 0

    for row in range(0, dict_num):
        value = sheet.cell(row, 0).value
        if value != '':
            songs.append(sheet.cell(row, 0).value)
        else:
            songs_blank += 1

        value = sheet.cell(row, 1).value
        if value != '':
            singers.append(sheet.cell(row, 1).value)
        else:
            singers_blank += 1

        value = sheet.cell(row, 2).value
        if value != '':
            albums.append(sheet.cell(row, 2).value)
        else:
            albums_blank += 1

    dict_music = {}
    dict_music['grm'] = []
    dict_music['grm'].append(gen_dict('songs', songs))
    dict_music['grm'].append(gen_dict('singers', singers))
    dict_music['grm'].append(gen_dict('albums', albums))

    with open(str(dict_num) + '.musicdict', 'wt', encoding='utf-8') as fp:
        json.dump(dict_music, fp, ensure_ascii=False, indent=2)

    print(str(dict_num), file=blank_ret)
    print('songs blank num:', songs_blank, file=blank_ret)
    print('singers blank num:', singers_blank, file=blank_ret)
    print('albums blank num:', albums_blank, file=blank_ret)
    print(file=blank_ret)

blank_ret.close()