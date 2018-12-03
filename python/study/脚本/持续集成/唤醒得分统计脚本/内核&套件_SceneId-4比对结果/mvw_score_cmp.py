#coding=utf-8

import sys
import re
import pprint

'''
运行方式：python mvw_score_cmp.py [套件处理后结果] [引擎结果1] [引擎结果2] ...

套件结果需手动把【空行】、【wav format, skip 44 bytes in the head行】、【lParam...行】、
【wav format, skip 44 bytes in the head行】删掉后再使用脚本处理。

引擎结果文件需换成utf8无BOM格式。
'''

obj_file_name = sys.argv[1]

ret = {}
ret_iss = {}
temp_lines = []

fp_ret_iss = open('iss_ret.txt','w')
fp_ret = open('ret.txt','w')

def get_mvw_ret(flag):
    global temp_lines

    try:
        if flag == 1:
            mvw_file = re.match(".*[集内词|唤醒测试集]\\\\(.*) .*", temp_lines[0]).group(1)
            mvw_file = mvw_file.replace('\\', '/')
            # mvw_file = mvw_file.decode('utf-8').encode('gbk')
            mvw_score = re.match(".*n2PassCMScore=(.*), nFrame.*", temp_lines[1]).group(1)
            ret[mvw_file] = mvw_score
        else:
            mvw_file = re.match(".*[集内词|唤醒测试集]\\\\(.*) .*", temp_lines[0]).group(1)
            mvw_file = mvw_file.replace('\\', '/')
            # mvw_file = mvw_file.decode('utf-8').encode('gbk')
            ret[mvw_file] = None
    except Exception,e:
        print '提取文件名出错,'.decode('utf-8').encode('gbk'), temp_lines[0]
        sys.exit(0)

    temp_lines = []

def handle_ret():
    global temp_lines

    if len(temp_lines) == 3:
        # 处理结果
        get_mvw_ret(1)

    elif len(temp_lines) == 1:
        # 没有结果
        get_mvw_ret(0)

def get_mvw_ret_iss(flag):
    global temp_lines

    if flag == 1:
        mvw_file = re.match(".*(SceneId-./.*)", temp_lines[0]).group(1)
        # mvw_file = mvw_file.decode('utf-8').encode('gbk')
        mvw_score = re.match(".*得分是： (.*)", temp_lines[1]).group(1)
        ret_iss[mvw_file] = mvw_score
    else:
        mvw_file = re.match(".*(SceneId-./.*)", temp_lines[0]).group(1)
        # mvw_file = mvw_file.decode('utf-8').encode('gbk')
        ret_iss[mvw_file] = None

    temp_lines = []

def handle_ret_iss():
    global temp_lines

    if len(temp_lines) == 2:
        # 处理结果
        get_mvw_ret_iss(1)

    elif len(temp_lines) == 1:
        # 没有结果
        get_mvw_ret_iss(0)

    elif len(temp_lines) == 3:
        # 唤醒多次
        mvw_score1 = int(re.match(".*得分是： (.*)", temp_lines[1]).group(1))
        mvw_score2 = int(re.match(".*得分是： (.*)", temp_lines[2]).group(1))
        if mvw_score1 > mvw_score2:
            temp_lines.remove(temp_lines[2])
        else:
            temp_lines.remove(temp_lines[1])
        get_mvw_ret_iss(1)
    
# 处理套件结果
with open(sys.argv[1]) as fp:
    for line in fp:
        line = line.strip()

        if line.startswith("E:/project"):
            handle_ret_iss()
            temp_lines.append(line)

        elif "唤醒次数" in line:
            handle_ret_iss()
            break

        elif line.startswith("Ivw success"):
            temp_lines.append(line)

    # pprint.pprint(ret_iss)
    for key in ret_iss:
        fp_ret_iss.write(key+' '+(ret_iss[key] if ret_iss[key]!=None else 'None')+'\n')
    print len(ret_iss)

temp_lines = []

if len(sys.argv) > 1:
    # 处理引擎结果
    for i in range(2, len(sys.argv)):
        with open(sys.argv[i]) as fp:
            for line in fp:
                line = line.strip()

                if line.startswith("\\\\192.168.24"):
                    handle_ret()
                    temp_lines.append(line)

                elif line.startswith("--------Wrap引擎版本"):
                    handle_ret()
                    break

                else:
                    # 唤醒结果、空行
                    temp_lines.append(line)

            # pprint.pprint(ret)
    for key in ret:
        fp_ret.write(key+' '+(ret[key] if ret[key]!=None else 'None')+'\n')
    print len(ret)

with open ('ret_diff.csv', 'w') as fp:
    fp.write('pcm,套件,引擎\n'.decode('utf-8').encode('gbk'))
    ret_num = 0.0
    ret_iss_num = 0.0
    for key in ret_iss:
        ret_iss_num += 1

        score_iss = ret_iss[key]

        if ret.has_key(key):

            score = ret[key]
            if score != score_iss:
                print key.decode('utf-8').encode('gbk').ljust(50), (str(score_iss) if score_iss!=None else 'None').ljust(4), (str(score) if score!=None else 'None').ljust(4)
                fp.write(key.decode('utf-8').encode('gbk')+','+(score_iss if score_iss!=None else 'None')+','+(score if score!=None else 'None')+'\n')
            else:
                ret_num += 1
        else:
            print key.decode('utf-8').encode('gbk').ljust(50), (str(score_iss) if score_iss!=None else 'None').ljust(4), '引擎未测试'.decode('utf-8').encode('gbk')
            fp.write(key.decode('utf-8').encode('gbk')+','+(score_iss if score_iss!=None else 'None')+','+'引擎未测试'.decode('utf-8').encode('gbk')+'\n')

    print '得分一致率：'.decode('utf-8').encode('gbk')+'%.2f' % (ret_num/ret_iss_num)
    fp.write('\n\n得分一致率：'.decode('utf-8').encode('gbk')+','+'%.2f' % (ret_num/ret_iss_num))

    