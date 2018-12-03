#coding=utf-8
'''author:puliu
   date:2017/4/19
   version:python2.7
'''

from Lib import Config
from Lib import Log
import sys
import time
import os
import xlwt
import json

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
	def get_compare_result(self,suitResultDict,recResultDict):
		'''get_compare_result(suitResultDict,recResultDict)'''
		self.rateList = []
		self.detailList = []
		for wakeup in suitResultDict:
			sDict = suitResultDict[wakeup]
			audioNum,beginNum,endNum,scoreNum,aWakeupNum = self.__set_detailDict(wakeup, sDict, recResultDict)
			self.__set_rateDict(wakeup,audioNum,beginNum,endNum,scoreNum,aWakeupNum)
			
		
	def __set_detailDict(self,wakeup,sDict,recResultDict):
		audioNum,beginNum,endNum,scoreNum,aWakeupNum = 0,0,0,0,0
		for audio in sDict:
			Parent.LG.write_log(1,audio)
			tempDict = {}
			defType = ""
			tempDict["音频"] = audio.decode("gbk")
			tempDict["时间"] =  Parent.LG.asctime
			tempDict["版本"] = Parent.paramDict["version"]
			tempDict["模式"] = Parent.paramDict["mode"]
			tempDict["预期唤醒词"] = wakeup
			tempDict["套件结果_前端点"] = sDict[audio]["前端点"] if "前端点" in sDict[audio] else ""
			tempDict["套件结果_后端点"] = sDict[audio]["后端点"] if "后端点" in sDict[audio] else ""
			tempDict["套件结果_得分"] = sDict[audio]["得分"] if "得分" in sDict[audio] else ""
			tempDict["套件结果_实际唤醒词"] = sDict[audio]["实际唤醒词"] if "实际唤醒词" in sDict[audio] else ""
			if audio in recResultDict:
				tempDict["引擎结果_前端点"] = recResultDict[audio]["前端点"] if "前端点" in recResultDict[audio] else ""
				tempDict["引擎结果_后端点"] = recResultDict[audio]["后端点"] if "前端点" in recResultDict[audio] else ""
				tempDict["引擎结果_得分"] = recResultDict[audio]["得分"] if "前端点" in recResultDict[audio] else ""
				tempDict["引擎结果_实际唤醒词"] = recResultDict[audio]["实际唤醒词"] if "前端点" in recResultDict[audio] else ""
				if tempDict["套件结果_前端点"] != tempDict["引擎结果_前端点"]:
					defType += "前端点"
				else:
					beginNum += 1
				if tempDict["套件结果_后端点"] != tempDict["引擎结果_后端点"]:
					defType += ",后端点"
				else:
					endNum += 1
				if tempDict["套件结果_得分"] != tempDict["引擎结果_得分"]:
					defType += ",得分"
				else:
					scoreNum += 1
				if tempDict["套件结果_实际唤醒词"] != tempDict["引擎结果_实际唤醒词"]:
					defType += ",实际唤醒词"
				else:
					aWakeupNum += 1
			else:
				tempDict["引擎结果_前端点"] = ""
				tempDict["引擎结果_后端点"] = ""
				tempDict["引擎结果_得分"] = ""
				tempDict["引擎结果_实际唤醒词"] = ""		
			tempDict["不一致类型"] = defType.decode("utf-8")
			for key in tempDict:
				#print key.decode("utf-8")
				Parent.LG.write_log(1,key)
				Parent.LG.write_log(1,tempDict[key])
			audioNum += 1
			self.detailList.append(tempDict)
		return audioNum,beginNum,endNum,scoreNum,aWakeupNum
	
	def __set_rateDict(self,wakeup,audioNum,beginNum,endNum,scoreNum,aWakeupNum):
		Parent.LG.write_log(1,(u"%s"%"-"*10).encode("gbk"))
		Parent.LG.write_log(1,wakeup)
		tempDict = {}
		tempDict["时间"] =  Parent.LG.asctime
		tempDict["版本"] = Parent.paramDict["version"]
		tempDict["模式"] = Parent.paramDict["mode"]
		tempDict["唤醒词"] = wakeup
		tempDict["前端点一致率"] = float(beginNum)/float(audioNum) if audioNum != 0 else 0.0
		tempDict["后端点一致率"] = float(endNum)/float(audioNum) if audioNum != 0 else 0.0
		tempDict["得分一致率"] = float(scoreNum)/float(audioNum) if audioNum != 0 else 0.0
		tempDict["是否唤醒一致率"] = float(aWakeupNum)/float(audioNum) if audioNum != 0 else 0.0
		for key in tempDict:
			Parent.LG.write_log(1,key)
			Parent.LG.write_log(1,tempDict[key])
		self.rateList.append(tempDict)
		
		
	
#===============================================================================================
#写结果到excel中
#===============================================================================================
class WriteResult(Parent):
	def __init__(self):
		self.rateHead = ["时间","版本","模式","唤醒词","前端点一致率","后端点一致率","得分一致率","是否唤醒一致率"]
		self.detailHead = ["时间","版本","模式","音频","预期唤醒词","套件结果_前端点","套件结果_后端点","套件结果_得分","套件结果_实际唤醒词",
		                   "引擎结果_前端点","引擎结果_后端点","引擎结果_得分","引擎结果_实际唤醒词","不一致类型"]
	
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
		
	@Parent.deco(Parent.LG)	
	def __write_sheet(self,sheet,dataList,headList):
		'''__write_rate_sheet(sheet,dataList,headList)'''
		
		for i in range(len(headList)):
			sheet.write(0,i,headList[i].decode("utf-8"))
			
		for j in range(len(dataList)):
			for i in range(len(headList)):
				sheet.write(j+1,i,dataList[j][headList[i]])

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
		resultDict = {}
		if "suitPath" in Parent.paramDict:
			suitPath = Parent.paramDict["suitPath"]
			if os.path.exists(suitPath):
				for parent,fileDirs,fileNames in os.walk(suitPath):
					for fileName in fileNames:
						fileDir = os.path.join(parent,fileName)
						name = fileName[:fileName.rfind(".")]
						resultDict[name] = {}
						sResultDict = self.__read_suit_txt(fileDir)
						resultDict[name] = sResultDict
				return resultDict
			else:
				Parent.LG.write_log(4,"Config 'suitPath' not exists!")
				return resultDict

			
	def __read_suit_txt(self,fileDir):
		resultDict = {}
		fp = open(fileDir)
		cList = fp.readlines()
		fp.close()
		
		for line in cList:
			line = line.strip()
			lineList = line.split("\t")
			audioName = self.__set_audio_name(lineList[0])
			if len(lineList) == 1:
				resultDict[audioName] = {}
			else:
				result = lineList[1]
				sResultDict = self.__get_suit_result(result)
				resultDict[audioName] = sResultDict
		return resultDict
				
		
	def __set_audio_name(self,audioName):
		audioName = audioName.replace("\\","/")
		if audioName[0] == "/":
			audioName = "." + audioName
		return audioName
	
	def __get_suit_result(self,result):
		resultDict= {}
		sResult = result[result.find("lParam:")+len("lParam:"):]

		lParamDict = json.loads(sResult.decode("gbk"))
		resultDict["得分"] = lParamDict["nMvwScore"] if "nMvwScore" in lParamDict else ""
		resultDict["实际唤醒词"] = lParamDict["nKeyword"] if "nKeyword" in lParamDict else ""
		resultDict["前端点"] = int(lParamDict["nStartBytes"])/32 if "nStartBytes" in lParamDict else ""
		resultDict["后端点"] = int(lParamDict["nEndBytes"])/32 if "nEndBytes" in lParamDict else ""				
		return resultDict
		
	#获取引擎结果
	@Parent.deco(Parent.LG)	
	def get_rec_result(self):
		'''get_rec_result()'''
		resultDict = {}
		if "recPath" in Parent.paramDict:
			if os.path.exists(Parent.paramDict["recPath"]):
				for parent,fileDirs,fileNames in os.walk(Parent.paramDict["recPath"]):
					for fileName in fileNames:
						fileDir = os.path.join(parent,fileName)
						sResultDict = self.__read_rec_txt(fileDir)
						resultDict.update(sResultDict)
				return resultDict
			else:
				Parent.LG.write_log(4,"Config 'suitPath' not exists!")
				return resultDict
	
	def __read_rec_txt(self,fileDir):
		resultDict = {}
		rAudio = ""
		fp = open(fileDir)
		cList = fp.readlines()
		fp.close()
		
		for line in cList:
			line = line.strip()
			if "{" not in line and "}" not in line:
				if line not in resultDict:
					rAudio = self.__set_audio_name(line)
					resultDict[rAudio] = {}
					
			else:
				print line
				lineDict = json.loads(line.decode("gbk").replace("\\","/"))
				if "rlt" in lineDict:
					sDict = lineDict["rlt"][0]
					resultDict[rAudio]["得分"] = sDict["ncm"] if "ncm" in sDict else ""
					resultDict[rAudio]["实际唤醒词"] = sDict["keyword"] if "keyword" in sDict else ""
					resultDict[rAudio]["前端点"] = int(sDict["istart"])*10 if "istart" in sDict else ""
					resultDict[rAudio]["后端点"] = (int(sDict["istart"])+int(sDict["iduration"]))*10 if "iduration" in sDict and "istart" in sDict else ""
		return resultDict
		
	def __set_audio_name(self,audioName):
		audioName = audioName.replace("\\","/")
		if audioName[0] == "/":
			audioName = "." + audioName
		return audioName	
	

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
		suitResultDict = self.GR.get_suit_result()
		if len(suitResultDict) == 0:
			return
		
		Parent.LG.write_log(2,"get rec result")
		recResultDict = self.GR.get_rec_result()
		if len(recResultDict) == 0:
			return
			
		Parent.LG.write_log(2,"begin compare")
		self.CP.get_compare_result(suitResultDict,recResultDict)
		Parent.LG.write_log(2,"end compare")
		resultDir  = os.path.join(Parent.paramDict["result"].encode("gbk"),"%s_%s.xls"%(Parent.paramDict["version"].encode("gbk"),
	                                                                                         Parent.paramDict["mode"].encode("gbk")))
		Parent.LG.write_log(2,"write result : %s"%(resultDir))
		self.WR.write_result(resultDir, self.CP)
		

#===============================================================================================
if __name__ == "__main__":
	ME = MainEx()
	ME.main()
	
	