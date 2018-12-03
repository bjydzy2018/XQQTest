#coding=utf-8
'''author:puliu
   date:2017/4/17
   version:python2.7
'''
from Lib import Config
from Lib import Log
from Lib import CompareResult_V1 as CompareResult
from Lib import GetRecConsistResult
from Lib import json_handle
import sys
import time
import os
import xlwt

#===============================================================================================
#父类：申请公共模块实例,获取公共数据,实现公共方法
#===============================================================================================
class Parent:
	CF = Config.Config()
	paramDict = CF.read_config()
	LG = Log.Log("./Log",level=int(paramDict["logLevel"]))
	
	#函数计时，装饰器
	@staticmethod
	def deco(LG):
		def _deco(func):
			def __deco(*args,**kwargs):
				beginTime = time.time()
				ret = func(*args,**kwargs)
				endTime = time.time()
				LG.write_log(1,"\tFunction '%s' run time is : %s s"%(func.__doc__,endTime-beginTime))
				return ret
			return __deco
		return _deco
	
	#生成目录
	@staticmethod
	def make_path(sDir):
		sPath = os.path.dirname(sDir)
		if os.path.exists(sPath) == False:
			os.makedirs(sPath)

#===============================================================================================
#一致性对比模块
#===============================================================================================
class Compare(Parent):
	def __init__(self):
		pass
	
	@Parent.deco(Parent.LG)
	def get_compare_result(self,recTempPath,suitTempPath):
		'''get_compare_result(recPath,suitPath)'''
		Parent.LG.write_log(1,"CompareResult.begainCompare(%s, %s)"%(recTempPath,suitTempPath))
		self.recDict,self.suitDict,self.errList,self.resultCount,self.vadBegin,self.vadEnd,self.whole = CompareResult.begainCompare(recTempPath, suitTempPath)
		Parent.LG.write_log(1,"result same num : %s"%self.resultCount)
		Parent.LG.write_log(1,"vad begin same num : %s"%self.vadBegin)
		Parent.LG.write_log(1,"vad end same num : %s"%self.vadEnd)
		Parent.LG.write_log(1,"whole same num : %s"%self.whole)
		self.audioNum = len(self.recDict)
		self.vadBeginRate = float(self.vadBegin)/float(self.audioNum) if self.audioNum != 0 else 0.0
		self.vadEndRate = float(self.vadEnd)/float(self.audioNum) if self.audioNum != 0 else 0.0
		self.resultRate = float(self.resultCount)/float(self.audioNum) if self.audioNum != 0 else 0.0
		self.wholeRate = float(self.whole)/float(self.audioNum) if self.audioNum != 0 else 0.0
		
		
	
#===============================================================================================
#写结果到excel中
#===============================================================================================
class WriteResult(Parent):
	def __init__(self):
		self.rateHead = ["时间","版本","模式","音频个数","vad前端点一致数","vad前端点一致率","vad后端点一致数","vad后端点一致率",
		                 "识别结果一致数","识别结果一致率","总体一致数","总体一致率"]
		self.detailHead = ["音频名称","套件结果_vad前端点","套件结果_vad后端点","套件结果_识别结果","引擎结果_vad前端点",
		                   "引擎结果_vad后端点","引擎结果_识别结果","不一致类型"]
	
	@Parent.deco(Parent.LG)
	def write_result(self,resultDir,CP):
		'''write_result(resultDir,Compare())'''
		Parent.make_path(resultDir)
		
		workbook = xlwt.Workbook()
		sheetRate = workbook.add_sheet("一致性".decode("utf-8"))
		sheetDetail = workbook.add_sheet("对比结果".decode("utf-8"))
		self.__write_rate_sheet(sheetRate, CP)
		self.__write_detail_sheet(sheetDetail, CP)
		workbook.save(resultDir)
		
	@Parent.deco(Parent.LG)	
	def __write_rate_sheet(self,sheet,CP):
		'''__write_rate_sheet(rateSheet,Compare())'''
		for i in range(len(self.rateHead)):
			sheet.write(0,i,self.rateHead[i].decode("utf-8"))
			
		sheet.write(1,0,Parent.LG.asctime)
		sheet.write(1,1,Parent.paramDict["version"].encode("gbk"))
		sheet.write(1,2,Parent.paramDict["mode"].encode("gbk"))
		sheet.write(1,3,CP.audioNum)
		sheet.write(1,4,CP.vadBegin)
		sheet.write(1,5,CP.vadBeginRate)
		sheet.write(1,6,CP.vadEnd)
		sheet.write(1,7,CP.vadEndRate)
		sheet.write(1,8,CP.resultCount)
		sheet.write(1,9,CP.resultRate)
		sheet.write(1,10,CP.whole)
		sheet.write(1,11,CP.wholeRate)
		
	@Parent.deco(Parent.LG)	
	def __write_detail_sheet(self,sheet,CP):
		'''__write_detail_sheet(detailSheet,Compare())'''
		for i in range(len(self.detailHead)):
			sheet.write(0,i,self.detailHead[i].decode("utf-8"))
			
		j = 1
		for audio in CP.recDict:
			sheet.write(j,0,audio)
			suitVadF,suitVadE,suitResult = self.__get_detail(CP.suitDict, audio)
			recVadF,recVadE,recResult = self.__get_detail(CP.recDict, audio)
			sheet.write(j,1,suitVadF)
			sheet.write(j,2,suitVadE)
			sheet.write(j,3,suitResult)
			sheet.write(j,4,recVadF)
			sheet.write(j,5,recVadE)
			sheet.write(j,6,recResult)
			sheet.write(j,7,self.__get_deffrent_type(suitVadF, suitVadE, suitResult, 
			                                         recVadF, 
			                                         recVadE, 
			                                         recResult))
			j += 1
			
	def __get_detail(self,sDict,audio):
		vadF = sDict[audio][1].split("|")[0] if audio in sDict else ""
		vadE = sDict[audio][1].split("|")[1] if audio in sDict else ""
		result = sDict[audio][0].decode("gbk") if audio in sDict else ""
		return vadF,vadE,result
	
	def __get_deffrent_type(self,suitVadF,suitVadE,suitResult,recVadF,recVadE,recResult):
		dType = ""

		if suitVadF != recVadF:
			dType += "vad前端点"
		
		if suitVadE != recVadE:
			dType += ",vad后端点"
				
		if suitResult != recResult:
			dType += ",识别结果"
			
		if dType != "":
			dType = dType[1:] if dType[0] == "," else dType
		return dType.decode("utf-8")
		
#===============================================================================================		
#规整引擎和套件的测试结果，使得两方结果格式一致
#===============================================================================================		
class SetResult(Parent):
	def __init__(self):
		pass	
		
	#规整引擎的两个结果，得到固定格式的结果文件
	@Parent.deco(Parent.LG)
	def set_rec_result(self):
		'''set_rec_result()'''
		if "recPath" in Parent.paramDict and "recTempPath" in Parent.paramDict:
			suitPath1,suitPath2 = self.__get_recPath(Parent.paramDict["recPath"])
			if suitPath1 != "" and suitPath2 != "":
				Parent.make_path(Parent.paramDict["recTempPath"])
				Parent.LG.write_log(1,"GetRecConsistResult.CovResultType(%s, %s,%s)"%(suitPath1,suitPath2,Parent.paramDict["recTempPath"]))
				ret = GetRecConsistResult.CovResultType(suitPath1, suitPath2, Parent.paramDict["recTempPath"])
				if os.path.exists(Parent.paramDict["recTempPath"]) == False:
					Parent.LG.write_log(4,"set rec result fail!")
					return -1
				else:
					Parent.LG.write_log(2,"set rec result success!")
					return Parent.paramDict["recTempPath"]
		else:
			Parent.LG.write_log(4,"Config 'suitPath' or 'suitTempPath' not exists!")
			return -1
		
	def __get_recPath(self,recPath):
		recPathList = recPath.split(",")
		if len(recPathList) == 2:
			return recPathList[0],recPathList[1]
		else:
			Parent.LG.write_log(4,"Config 'recPath' must takes two!")
			return "",""
		
	#规整套件结果.csv,得到固定格式的结果文件
	@Parent.deco(Parent.LG)
	def set_suit_result(self):
		'''set_suit_result()'''
		if "suitPath" in Parent.paramDict and "suitTempPath" in Parent.paramDict:
			suitPath = Parent.paramDict["suitPath"]
			suitTempPath = Parent.paramDict["suitTempPath"]
			Parent.make_path(suitTempPath)
			if os.path.exists(suitPath):
				Parent.LG.write_log(1,"json_handle.fun(%s, %s)"%(suitPath,suitTempPath))
				ret = json_handle.fun(suitPath, suitTempPath)
				if os.path.exists(suitTempPath):
					Parent.LG.write_log(2,"set suit result success!")
					return suitTempPath
				else:
					Parent.LG.write_log(4,"set suit result success!")
					return -1
			else:
				Parent.LG.write_log(4,"Config 'suitPath' not exists!")
				return -1
	

#===============================================================================================		
#Main Class
#===============================================================================================
class MainEx(Parent):
	def __init__(self):
		self.SR = SetResult()
		self.CP = Compare()
		self.WR = WriteResult()
		
	@Parent.deco(Parent.LG)
	def main(self):
		'''main()'''
		Parent.LG.write_log(2,"set rec result")
		recTempPath = self.SR.set_rec_result()
		if recTempPath == -1:
			return
		
		Parent.LG.write_log(2,"set suit result")
		suitTempPath = self.SR.set_suit_result()
		if suitTempPath == -1:
			return
		
		Parent.LG.write_log(2,"begin compare")
		self.CP.get_compare_result(recTempPath, suitTempPath)
		Parent.LG.write_log(2,"end compare")
		resultDir  = os.path.join(Parent.paramDict["result"].encode("gbk"),"%s_%s.xls"%(Parent.paramDict["version"].encode("gbk"),
		                                                                                 Parent.paramDict["mode"].encode("gbk")))
		Parent.LG.write_log(2,"write result : %s"%(resultDir))
		self.WR.write_result(resultDir, self.CP)
		

#===============================================================================================
if __name__ == "__main__":
	ME = MainEx()
	ME.main()
	
	