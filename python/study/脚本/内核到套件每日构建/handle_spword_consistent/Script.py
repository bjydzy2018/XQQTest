#coding=utf-8
'''author:puliu
   date:2017/4/18
   version:python2.7
'''
from Lib import Config
from Lib import Log
from Lib import ReadPOIXPResult
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
	def get_compare_result(self,recPath,suitTempPath):
		'''get_compare_result(recPath,suitPath)'''
		recList = open(recPath).readlines()
		suitList = open(suitTempPath).readlines()
		self.resultList,self.sameRate = self.__compare(recList, suitList)
		Parent.LG.write_log(1,"same rate : %s"%self.sameRate)
		
	def __compare(self,resList1,resList2):
		resultList = []
		sameNum,deffNum,sameRate = 0,0,0.0
		wrongNum = 0
		for i in range(len(resList1)):
			tempList = []
			line1 = resList1[i].strip()
			tempList.append(line1)
			if i < len(resList2):
				line2 = resList2[i].strip()
				tempList.append(line2)
				if line1 == line2:
					sameNum += 1
					tempList.append(1)
				else:
					deffNum += 1
					tempList.append(0)
			else:
				tempList.append("",0)
			resultList.append(tempList)
		sameRate = float(sameNum)/float(i) if i!=0 else 0.0
		return resultList,sameRate
			
		
		
	
#===============================================================================================
#写结果到excel中
#===============================================================================================
class WriteResult(Parent):
	def __init__(self):
		self.rateHead = ["时间","版本","模式","一致率"]
		self.detailHead = ["时间","版本","模式","套件结果","引擎结果","是否一致"]
	
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
		sheet.write(1,1,Parent.paramDict["version"])
		sheet.write(1,2,Parent.paramDict["mode"])
		sheet.write(1,3,CP.sameRate)
		
	@Parent.deco(Parent.LG)	
	def __write_detail_sheet(self,sheet,CP):
		'''__write_detail_sheet(detailSheet,Compare())'''
		for i in range(len(self.detailHead)):
			sheet.write(0,i,self.detailHead[i].decode("utf-8"))
			
		j = 1
		for line in CP.resultList:
			sheet.write(j,0,Parent.LG.asctime)
			sheet.write(j,1,Parent.paramDict["version"])
			sheet.write(j,2,Parent.paramDict["mode"])			
			sheet.write(j,3,line[0].decode("gbk"))
			sheet.write(j,4,line[1].decode("gbk"))
			sheet.write(j,5,line[2])
			j += 1
		
#===============================================================================================		
#规整引擎和套件的测试结果，使得两方结果格式一致
#===============================================================================================		
class SetResult(Parent):
	def __init__(self):
		pass	
	
	#规整套件结果.csv,得到固定格式的结果文件
	@Parent.deco(Parent.LG)
	def set_suit_result(self):
		'''set_suit_result()'''
		if "suitPath" in Parent.paramDict and "suitTempPath" in Parent.paramDict:
			suitDir = Parent.paramDict["suitPath"]
			suitTempPath = Parent.paramDict["suitTempPath"]
			Parent.make_path(suitTempPath)
			if os.path.exists(suitDir):
				Parent.LG.write_log(1,"ReadPOIXPResult.main(%s, %s)"%(suitDir,suitTempPath))
				ret = ReadPOIXPResult.main(suitDir, suitTempPath)
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
		Parent.LG.write_log(2,"set suit result")
		suitTempPath = self.SR.set_suit_result()
		if suitTempPath == -1:
			return
		
		if os.path.exists(Parent.paramDict["recPath"]) == False:
			Parent.LG.write_log(4,"config 'recPath' error !")
			return
			
		Parent.LG.write_log(2,"begin compare")
		self.CP.get_compare_result(Parent.paramDict["recPath"], suitTempPath)
		Parent.LG.write_log(2,"end compare")
		resultDir  = os.path.join(Parent.paramDict["result"].encode("gbk"),"%s_%s.xls"%(Parent.paramDict["version"].encode("gbk"),
	                                                                                         Parent.paramDict["mode"].encode("gbk")))
		Parent.LG.write_log(2,"write result : %s"%(resultDir))
		self.WR.write_result(resultDir, self.CP)
		

#===============================================================================================
if __name__ == "__main__":
	ME = MainEx()
	ME.main()
	
	