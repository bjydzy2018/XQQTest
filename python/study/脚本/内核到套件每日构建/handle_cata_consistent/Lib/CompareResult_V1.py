#!bin/python
#coding=gbk
import os
import sys
import re
import csv
#excel操作的库文件
from xlwt import Workbook
import  xlwt


def judge_pure_english(keyword):
          return all(ord(c) < 128 for c in keyword)

def SetStyle(bold_style,height_style,alignment_style,groundcolor_style):
          styleone = xlwt.XFStyle()
          word_style = xlwt.Font()
          pos_style = xlwt.Alignment()
          border_style = xlwt.Borders()
          background_style = xlwt.Pattern()
          
         
          word_style.name = u"楷体"
          border_style.right=1
          border_style.left=1
          border_style.top=1
          border_style.bottom=1
          pos_style.vert=pos_style.VERT_CENTER
          
          #可设定值
          word_style.bold = bold_style
          word_style.height = height_style
          pos_style.horz = alignment_style
          background_style.pattern = background_style.SOLID_PATTERN
          background_style.pattern_fore_colour = groundcolor_style
          styleone.font = word_style
          styleone.borders = border_style
          styleone.alignment = pos_style
          styleone.pattern = background_style
          return styleone

def GetResultDict(inFile):
          dict_retrun = {}
          f = open(r"%s"%inFile,"r")
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
                                                  dict_value += lines_rec[h].strip().split("^^")[0].replace("。","").lower().replace(".","").replace(",","").replace("，","").replace("?","").replace("。","").replace("，","").replace("？","").replace("。","").replace("！","")
                              dict_retrun.setdefault(dict_key,dict_value.split("vad:\t"))
                              #print dict_value
          return dict_retrun
def begainCompare(inFile_rec,inFile_taojian):
          #准备表格
          
          dict_rec = {}
          dict_taojian = {}
          dict_rec = GetResultDict(inFile_rec)
          dict_taojian = GetResultDict(inFile_taojian)
          result_count = 0
          vad_bg = 0
          vad_ed = 0
          whole_right = 0
          
          '''for eachkey in dict_rec:
                    print dict_rec
          for onekey in dict_taojian:
                    print dict_taojian[onekey]'''
          flag_bg = 2
          list_err = []
          for eachkey in dict_rec:
                    right_flag = 1
                    if (eachkey in dict_taojian):
                              if (dict_taojian[eachkey][0] == dict_rec[eachkey][0]):
                                        result_count += 1
                              else:
                                        '''use_sheet.write(flag_bg,0,eachkey,StyleOne)
                                        use_sheet.write(flag_bg,1,dict_rec[eachkey][0].decode("GBK"),StyleOne)
                                        use_sheet.write(flag_bg,4,dict_taojian[eachkey][0].decode("GBK"),StyleOne)
                                        flag_bg += 1'''
                                        right_flag = 0
                                        list_err.append(eachkey)
                              if (dict_taojian[eachkey][-1].split("|")[0] == dict_rec[eachkey][-1].split("|")[0]):
                                        vad_bg += 1
                              else:
                                        '''
                                        use_sheet.write(flag_bg,0,eachkey,StyleOne)
                                        use_sheet.write(flag_bg,2,dict_rec[eachkey][-1].split("|")[0].decode("GBK"),StyleOne)
                                        use_sheet.write(flag_bg,5,dict_taojian[eachkey][-1].split("|")[0].decode("GBK"),StyleOne)
                                        flag_bg += 1'''
                                        right_flag = 0
                                        list_err.append(eachkey)
                              if (dict_taojian[eachkey][-1].split("|")[-1] == dict_rec[eachkey][-1].split("|")[-1]):
                                        vad_ed += 1
                              else:
                                        '''
                                        use_sheet.write(flag_bg,0,eachkey,StyleOne)
                                        use_sheet.write(flag_bg,3,dict_rec[eachkey][-1].split("|")[-1].decode("GBK"),StyleOne)
                                        use_sheet.write(flag_bg,6,dict_taojian[eachkey][-1].split("|")[-1].decode("GBK"),StyleOne)
                                        flag_bg += 1
                                        '''
                                        right_flag = 0
                                        list_err.append(eachkey)
                              if right_flag == 1:
                                        whole_right += 1
                    else:
                              pass
          #print "结果一致性：%f"%(float(result_count)/len(dict_rec))
          #print "Vad前端点：%f"%(float(vad_bg)/len(dict_rec))
          #print "Vad后端点：%f"%(float(vad_ed)/len(dict_rec))
          
          #for i in xrange(len(sorted(list(set(list_err))))):
          '''
          for eachlist in sorted(list(set(list_err))):
                    use_sheet.write(flag_bg,0,eachlist,StyleOne)
                    use_sheet.write(flag_bg,1,dict_rec[eachlist][0].decode("GBK"),StyleOne)
                    use_sheet.write(flag_bg,4,dict_taojian[eachlist][0].decode("GBK"),StyleOne)
                    use_sheet.write(flag_bg,2,dict_rec[eachlist][-1].split("|")[0].decode("GBK"),StyleOne)
                    use_sheet.write(flag_bg,5,dict_taojian[eachlist][-1].split("|")[0].decode("GBK"),StyleOne)
                    use_sheet.write(flag_bg,3,dict_rec[eachlist][-1].split("|")[-1].decode("GBK"),StyleOne)
                    use_sheet.write(flag_bg,6,dict_taojian[eachlist][-1].split("|")[-1].decode("GBK"),StyleOne)
                    flag_bg += 1 
          '''
          return dict_rec,dict_taojian,list_err,result_count,vad_bg,vad_ed,whole_right
          
if __name__=="__main__":
          '''
          StyleOne = SetStyle(0,250,1,15)
          #StyleOne = SetStyle(0,250,2,15)
          use_book = Workbook()
          use_sheet  = use_book.add_sheet("ErrList")
          use_sheet.write_merge(0,0,1,3,"引擎".decode("GBK"),SetStyle(1,300,2,1))
          use_sheet.write_merge(0,0,4,6,"套件".decode("GBK"),SetStyle(1,300,2,1))
          use_sheet.write_merge(0,1,0,0,"音频名".decode("GBK"),SetStyle(1,300,2,1))
          use_sheet.write(1,1,"识别结果".decode("GBK"),SetStyle(1,300,2,1))
          use_sheet.write(1,2,"前端点".decode("GBK"),SetStyle(1,300,2,1))
          use_sheet.write(1,3,"后端点".decode("GBK"),SetStyle(1,300,2,1))
          use_sheet.write(1,4,"识别结果".decode("GBK"),SetStyle(1,300,2,1))
          use_sheet.write(1,5,"前端点".decode("GBK"),SetStyle(1,300,2,1))
          use_sheet.write(1,6,"后端点".decode("GBK"),SetStyle(1,300,2,1))
          '''
          begainCompare(sys.argv[1],sys.argv[2])
          '''
          use_sheet.col(0).width = 10000
          use_sheet.col(1).width = 10000
          use_sheet.col(4).width = 10000
          use_book.save("compareResult.xls")
          '''
