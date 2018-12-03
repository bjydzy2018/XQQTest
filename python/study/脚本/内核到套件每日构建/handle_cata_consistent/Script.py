#-*- conding:utf-8 -*-
'''author:puliu
   date:2017/4/20
   version:python2.7
'''

from Lib import Config
from Lib import Log
import sys
import time
import os
import xlwt
import json
import copy

#===============================================================================================
#父类：申请公共模块实例,获取公共数据,实现公共方法
#===============================================================================================
class Parent:
	CF = Config.Config()
	paramDict = CF.read_config()
	LG = Log.Log("./Log",level=int(paramDict["logLevel"]))
	
	#函数计时，装饰器
	@staticmethod
	def deco(log):
		def _deco(func):
			def __deco(*args,**kwargs):
				beginTime = time.time()
				ret = func(*args,**kwargs)
				endTime = time.time()
				log.write_log(1,"\tFunction '%s' run time is : %s s"%(func.__doc__,endTime-beginTime))
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
	
	#对比引擎和套件结果
	#返回两个列表rateList:一致率列表[{时间: ,门限值: ,搜索文本: , 套件结果: ,引擎结果: , 是否一致: ,错误类型: },...]
	#detailList:明细列表[{时间: , 文本总数: ,一致数: ,一致率: }]
	@Parent.deco(Parent.LG)
	def get_compare_result(self,suitResultList,recResultList):
		'''get_compare_result(suitResultList,recResultList)'''
		rightNum = 0
		self.rateList = [{"时间":Parent.LG.asctime}]
		self.detailList = []
		for i in range(len(suitResultList)):
			tempDict = {"时间":Parent.LG.asctime,"门限值":str(Parent.paramDict["threshold"])}
			suitResultDict = suitResultList[i]
			suitName = suitResultDict["name"]
			suitResult = suitResultDict["result"]
			tempDict["搜索文本"] = suitName
			tempDict["套件结果"] = suitResult
			Parent.LG.write_log(1,"%s : "%suitName)
			if i < len(recResultList):
				recResultDict = recResultList[i]
				recName = recResultDict["name"]
				recResult = recResultDict["result"]
				tempDict["引擎结果"] = recResult
				if suitName == recName:
					ret,res = self.__compare(suitResult,recResult)
					if ret:
						rightNum += 1
					tempDict["是否一致"] = str(ret)
					tempDict["错误类型"] = res
				else:
					Parent.LG.write_log(1,"name not matching \n suit : '%s' >> '%s' \n rec  : '%s' >> '%s'"%(suitName,suitResult,recName,recResult))
					tempDict["是否一致"] = "False"
					tempDict["错误类型"] = "引擎搜索文本为:%s"%recName
			else:
				tempDict["引擎结果"] = ""
				tempDict["是否一致"] = "False"
				tempDict["错误类型"] = "没有匹配的引擎文本"
			Parent.LG.write_log(1,"")
			self.detailList.append(tempDict)
		self.rateList[0]["文本总数"] = str(len(suitResultList))
		self.rateList[0]["一致数"] = str(rightNum)
		self.rateList[0]["一致率"] = str(float(rightNum)/float(len(suitResultList))) if len(suitResultList) != 0 else 0.0
		
	#分步骤对比：字符串对比，json格式对比，错误码对比
	def __compare(self,suitResult,recResult):
		ret = self.__str_compare(suitResult, recResult)
		if ret == True:
			return True,""
		else:
			ret,res = self.__json_compare(suitResult,recResult)
			if ret == True:
				return ret,res
			else:
				if res != "":
					return ret,res
				else:
					ret,res = self.__error_code_compare(suitResult,recResult)
					return ret,res
			return False,"string not matching"
	
	#纯字符串对比
	def __str_compare(self,suitResult,recResult):
		if suitResult.strip() == recResult.strip():
			return True
		else:
			return False
			
	#json格式对比：找出'得分'大于门限值(配置项)的结果，先对比结果个数(个数不同返回错误，个数相同进入下一步)，再对比结果内容
	def __json_compare(self,suitResult,recResult):
		tryRet = True
		if suitResult == "null" or recResult == "null":
			return False,""
		
		try:
			suitDict = json.loads(suitResult)
		except Exception,e:
			Parent.LG.write_log(1,"suit result : '%s' , %s . "%(suitResult,e))
			tryRet = False
		try:
			recDict = json.loads(recResult)
		except Exception,e:
			Parent.LG.write_log(1,"rec result : '%s' , %s . "%(recResult,e))
			tryRet = False
		if tryRet == False:
			return False,""
			
		if "FIRST" in suitDict:
			if  "FIRST" in recDict:
				suitFIRSTList = suitDict["FIRST"]
				recFIRSTList = recDict["FIRST"]
				if len(suitFIRSTList) == len(recFIRSTList):
					suitOverThresholdList  = self.__get_over_threshold(suitFIRSTList)
					recOverThresholdList  = self.__get_over_threshold(recFIRSTList)
					if len(suitOverThresholdList) > 0 and len(recOverThresholdList) > 0:
						if len(suitOverThresholdList) == len(recOverThresholdList):
							ret,res = self.__list_compare(suitOverThresholdList, recOverThresholdList)
							return ret,res
						else:
							return False,"大于门限条目数不一致"
					else:
						return False,""
				else:
					Parent.LG.write_log(1,"suitResult len : %s , recResult len : %s \n suitResult : '%s' \n recResult  : '%s'"%(len(suitFIRSTList),len(recFIRSTList),suitResult,recResult))
					return False,"总条目数不一致"
			else:
				Parent.LG.write_log(1,"FIRST not in '%s'"%recResult)
				return False,"FIRST not in recResult"
		else:
			Parent.LG.write_log(1,"FIRST not in '%s'"%suitResult)
			return False,"FIRST not in suitResult"
		
	#获取得分大于指定门限的结果·
	def __get_over_threshold(self,result):
		overThresholdList = []
		for li in result:
			if "score" in li:
				if float(li["score"]) >= int(Parent.paramDict["threshold"]):
					overThresholdList.append(li)
		return overThresholdList
	
	#结果内容对比
	def __list_compare(self,suitList,recList):
		errorStr = ""
		for i in range(len(suitList)):
			suitDict = suitList[i]
			recDict = recList[i]
			for key in suitDict:
				if key not in recDict:
					errorStr = self.__set_errorStr(errorStr, "内容不一致")
					continue
				if key == "score":
					if suitDict[key] != recDict[key]:
						errorStr = self.__set_errorStr(errorStr, "得分不一致")
					continue
				if (type(suitDict[key]) == str or type(suitDict[key]) == unicode) and (type(recDict[key]) == str or type(recDict[key]) == unicode):
					if suitDict[key] != recDict[key]:
						errorStr = self.__set_errorStr(errorStr, "内容不一致")
				else:
					if len(suitDict[key]) != len(recDict[key]):
						errorStr = self.__set_errorStr(errorStr, "内容不一致")
					for j in range(len(suitDict[key])):
						if suitDict[key][j] != recDict[key][j]:
							errorStr = self.__set_errorStr(errorStr, "内容不一致")
		if errorStr != "":
			return False,errorStr
		else:
			return True,""
		
	def __set_errorStr(self,errorStr,error):
		if error == errorStr or error in errorStr:
			return errorStr
		else:
			if errorStr == "":
				errorStr += error
			else:
				errorStr += "," + error
			return errorStr
		
	#当结果为错误码时，进行对比
	def __error_code_compare(self,suitResult,recResult):
		if "err=" in suitResult and "err=" in recResult:
			suitErrorCode = suitResult[suitResult.find("err=")+len("err="):]
			recErrorCode = recResult[recResult.find("err=")+len("err="):]
			return True,"错误码"
		else:
			return True,""
		
	
#===============================================================================================
#写结果到excel中
#===============================================================================================
class WriteResult(Parent):
	def __init__(self):
		self.rateHead = ["时间","文本总数","一致数","一致率"]
		self.detailHead = ["时间","搜索文本","门限值","套件结果","引擎结果","是否一致","错误类型"]
	
	#写结果到excel中
	@Parent.deco(Parent.LG)
	def write_result(self,resultDir,CP):
		'''write_result(resultDir,Compare())'''
		Parent.make_path(resultDir)
		
		workbook = xlwt.Workbook()
		sheetRate = workbook.add_sheet("一致性".decode("utf-8"))
		sheetDetail = workbook.add_sheet("对比结果".decode("utf-8"))
		self.__write_sheet(sheetRate, CP.rateList,self.rateHead)
		self.__write_sheet(sheetDetail, CP.detailList,self.detailHead)
		workbook.save(resultDir)
		
	#根据head列表写结果到指定的sheet，head列表与结果字典的key一一对应
	#dataList:[{key:value,...},...]
	@Parent.deco(Parent.LG)
	def __write_sheet(self,sheet,dataList,headList):
		'''__write_rate_sheet(sheet,dataList,headList)'''
		for i in range(len(headList)):
			sheet.write(0,i,headList[i].decode("utf-8"))
			
		for j in range(len(dataList)):
			for i in range(len(headList)):
				sheet.write(j+1,i,dataList[j][headList[i]].decode("utf-8"))

#===============================================================================================		
#获取引擎和套件结果
#===============================================================================================		
class GetResult(Parent):
	def __init__(self):
		pass	
	
	#获取套件结果保存在结果字典中
	@Parent.deco(Parent.LG)
	def get_suit_result(self):
		'''get_suit_result()'''
		resultList = []
		if "suitPath" in Parent.paramDict:
			suitPath = Parent.paramDict["suitPath"]
			if os.path.exists(suitPath):
				fp = open(suitPath)
				content = fp.read()
				fp.close()
				cList = content.split("query = ")
				for sContent in cList:
					name,result = self.__get_result(sContent)
					if name != "":
						resultList.append({"name":name,"result":result})
				return resultList
			else:
				Parent.LG.write_log(4,"Config 'suitPath' not exists!")
				return resultList
		return  resultList
		
	#获取引擎结果
	@Parent.deco(Parent.LG)	
	def get_rec_result(self):
		'''get_rec_result()'''
		resultList = []
		if "recPath" in Parent.paramDict:
			recPath = Parent.paramDict["recPath"]
			if os.path.exists(recPath):
				fp = open(recPath)
				content = fp.read()
				fp.close()
				content = content[content.find("\r\n")+len("\r\n"):] if "\r\n" in content else content[content.find("\n")+len("\n"):]
				cList = content.split("query = ")
				for sContent in cList:
					if "query=" in sContent:
						sContent = sContent[:sContent.rfind("\r\n")] if "\r\n" in sContent else sContent[:sContent.rfind("\n")]
						name,result = self.__get_result(sContent)
						if name != "":
							resultList.append({"name":name,"result":result})		
				return resultList
			else:
				Parent.LG.write_log(4,"Config 'recPath' not exists!")
				return resultList
		return resultList
			
	def __get_result(self,content):
		content = content.strip()
		cList = content.split("\r\n") if "\r\n" in content else content.split("\n")
		name,result = "",""
		
		for line in cList:
			if "query=" in line:
				line = line[line.find("query=")+len("query="):]
				name = line[:line.find(",")]
			else:
				result += line.strip()
		return name,result	
	

#===============================================================================================		
#Main Class
#===============================================================================================
class MainEx(Parent):
	def __init__(self):
		self.GR = GetResult()
		self.CP = Compare()
		self.WR = WriteResult()
		
	@Parent.deco(Parent.LG)
	def main(self):
		'''main()'''
		Parent.LG.write_log(2,"get suit result")
		suitResultList = self.GR.get_suit_result()
		if len(suitResultList) == 0:
			return
		
		Parent.LG.write_log(2,"get rec result")
		recResultList = self.GR.get_rec_result()
		if len(recResultList) == 0:
			return
		
		Parent.LG.write_log(1,"suitResultList len : %s"%(len(suitResultList)))
		Parent.LG.write_log(1,"recResultList len : %s"%(len(recResultList)))
			
		Parent.LG.write_log(2,"begin compare")
		self.CP.get_compare_result(suitResultList,recResultList)
		Parent.LG.write_log(2,"end compare")
		resultDir  = os.path.join(Parent.paramDict["result"].encode("gbk"),"result.xls")
		Parent.LG.write_log(2,"write result : %s"%(resultDir))
		self.WR.write_result(resultDir, self.CP)
		

#===============================================================================================
if __name__ == "__main__":
	ME = MainEx()
	ME.main()
	
	