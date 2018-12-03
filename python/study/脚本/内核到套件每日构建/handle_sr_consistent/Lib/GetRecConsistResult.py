#!bin/python
#coding=GBK
import os
import re
import sys
import csv
import shutil

def CovResultType(inrecfile,inperffile,outfile):
    #将所有的识别结果放到一个字典中
    dict_rec = {}
    f = open(r"%s"%inrecfile,"r")
    lines_rec = f.readlines()
    f.close()
    for i in xrange(len(lines_rec)):
        if re.search(".wav",lines_rec[i]) or re.search(".pcm",lines_rec[i]):
            dict_key=lines_rec[i].split('\\')[-1].split('/')[-1].split('"')[0].strip()
            dict_value=""
            for h in xrange(i+1,len(lines_rec)):
                if lines_rec[h].strip() == ".":
                    break
                elif lines_rec[h].startswith("PinYin:") or lines_rec[h].startswith("<s>") or lines_rec[h].startswith("</s>") or lines_rec[h].startswith("nConfidenceScore") or lines_rec[h].startswith("NetType") or lines_rec[h].startswith("SpeechStartFrame") or lines_rec[h].startswith("SpeechStartTime") or lines_rec[h].startswith("SpeechEndFrame") or lines_rec[h].startswith("SpeechEndTime") or lines_rec[h].startswith("AudioTime") or lines_rec[h].startswith("ResultTime") or lines_rec[h].startswith("InputTime"):
                    pass
                else:
                    dict_value += lines_rec[h].strip().split("^^")[0].replace("。","").lower().replace(".","").replace(",","").replace("，","")
            dict_rec.setdefault(dict_key,dict_value)
    #将所有的效率日志抠出来放到字典中
    dict_perf = {}
    f = open(inperffile,"r")
    lines_perf = f.readlines()
    f.close()
    '''for eachkey in dict_rec:
        print eachkey'''
    for eachlist in lines_perf:
        if re.search(".wav",eachlist) or re.search(".pcm",eachlist):
            wavname = eachlist.split('\\')[-1].split('/')[-1].strip()
            vadStartFram = eachlist.split(",")[3].strip()
            vadEndFram = eachlist.split(",")[6].strip()
            #print wavname
            if wavname in dict_rec:
                dict_rec[wavname] +=  "\n"+"Vad:\t"+str(int(vadStartFram)*10)+"|"+str(int(vadEndFram)*10) + "\n" + "." + "\n"
            else:
                pass
        else:
            pass
    f = open(outfile,"w")
    for eachkey in sorted(dict_rec.keys()):
        f.write(eachkey + "\n")
        f.write(dict_rec[eachkey])
    f.close()
    return 0
if __name__ == "__main__":
    CovResultType(sys.argv[1],sys.argv[2],sys.argv[3])
    


    
