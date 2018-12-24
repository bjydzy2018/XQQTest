#coding=utf-8
#2016-06-27
#by qqxiong
import sys
import json

org_dict_file_name = "中文-top1.5w.txt"
out_dict_file_name = "musicDict-500.txt"
out_dict_count = 500

# 读取命令行参数
if(len(sys.argv) != 1 and len(sys.argv) != 4):
    print('Invalid Arg! len(sys.argv)=%d' % (len(sys.argv)))
    print('Uasge: %s <org_dict_file_name> <out_dict_file_name> <out_dict_count>' % (sys.argv[0]))
    sys.exit(1)
if(len(sys.argv) == 4):
    org_dict_file_name = sys.argv[1]
    out_dict_file_name = sys.argv[2]
    out_dict_count = int(sys.argv[3])

dictcontant = []
Dictlist = { "dictname": "music", "dictcontant": dictcontant }
f = open(org_dict_file_name, 'r', encoding="utf-8")
Count=0
for eachLine in f:
    line_part = eachLine.strip('\r\n').split('\t')
    dictcontant_part = {"artist": "", "song": "", "album": ""}
    dictcontant_part["artist"]=line_part[2]
    dictcontant_part["song"]=line_part[1]
    dictcontant_part["album"]=line_part[3]
    dictcontant.append(dictcontant_part)
    Count = Count+1
    if(Count>=out_dict_count):
        break

out=open(out_dict_file_name,'w+', encoding="utf-8")
out.writelines(json.dumps(Dictlist,ensure_ascii=False, indent=4))
out.close()
print("input: %s, count: %d"%(org_dict_file_name, out_dict_count))
print("Output: %s"%(out_dict_file_name))
