#coding=utf-8

import re
import sys
import json
import pprint
import xlrd
import xlwt3

def ret_file_to_list(ret_file):
    is_json_begin = False
    ret_dict = {}
    json_str = ''
    current_num = ''

    with open(ret_file,encoding='utf8') as fp:
        for line in fp:
            line = line.strip()

            if not is_json_begin:
                if line.startswith('navi_SR'):
                    is_json_begin = True
                    current_num = re.match('(navi_SR_\d{5}).*', line).group(1)
            else:
                if line == '':
                    is_json_begin = False
                    try:
                        ret_dict[current_num]=(json.loads(json_str))
                    except Exception:
                        ret_dict[current_num] = ''
                    json_str = ''
                elif line == 'null':
                    is_json_begin = False
                    ret_dict[current_num] = ''
                else:
                    json_str += line
    return ret_dict

def compare_dict(dict1, dict2):
    if dict1 == '' and dict2 != '':
        return False
    elif dict1 != '' and dict2 == '':
        return False
    else:
        '''
        1分以下结果不可信，因此需增加对词典分数的判断
        for key in [k for k in dict1.keys() if k!='score']:
            try:
                if dict1[key] != dict2[key]:
                    return False
            except KeyError:
                # 不按顺序比较时，可能出现两个字典键不一样的情况
                return True
        '''
        if float(dict1['score']) < 1:
            return True

        for key in [k for k in dict1.keys() if k!='score']:
            try:
                if dict1[key] != dict2[key]:
                    return False
            except KeyError:
                # 不按顺序比较时，可能出现两个字典键不一样的情况
                return True

        # 是否考虑得分
        if abs(float(dict1['score'])-float(dict2['score'])) > 1:
            return False

    return True

def compare_json(json1, json2):
    if json1!='' and json2=='':
        return False
    elif json1=='' and json2!='':
        return False
    elif json1=='' and json2=='':
        return True
    else:
        try:
            json1_first_list = json1['FIRST']
            json2_first_list = json2['FIRST']
        except KeyError:
            return False
        except TypeError:
            return False

        if len(json1_first_list) != len(json2_first_list):
            return False

        # 按顺序比较
        # for i in range(len(json1_first_list)):
        #     if not compare_dict(json1_first_list[i], json2_first_list[i]):
        #         return False

        # 不按顺序比较
        for d1 in json1_first_list:
            has_true = False

            for d2 in json2_first_list:
                if compare_dict(d1, d2):
                    has_true = True
            if not has_true:
                return False

        return True

if __name__ == '__main__':
    iss_file = sys.argv[1]
    core_file = sys.argv[2]

    json_ret_iss = ret_file_to_list(iss_file)
    json_ret_core = ret_file_to_list(core_file)

    ret_file = xlwt3.Workbook()
    table = ret_file.add_sheet('compare')

    table.write(0, 0, '序号')
    table.write(0, 1, '引擎')
    table.write(0, 2, '套件')
    table.write(0, 3, '比较结果')

    line_index = 1
    for ret in json_ret_core:
        if ret in json_ret_iss.keys():
            table.write(line_index, 0, ret)
            table.write(line_index, 1, str(json_ret_core[ret]))
            table.write(line_index, 2, str(json_ret_iss[ret]))
            table.write(line_index, 3, compare_json(json_ret_core[ret], json_ret_iss[ret]))
        else:
            table.write(line_index, 0, ret)
            table.write(line_index, 1, str(json_ret_core[ret]))
            table.write(line_index, 2, '')
            table.write(line_index, 3, 'FALSE')
        line_index += 1

    ret_file.save('cata_consistent_ret.xls')