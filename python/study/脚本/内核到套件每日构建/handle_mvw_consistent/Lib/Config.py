# -*-coding:utf-8 -*-
"""Config

	----------------------------------------------------------------------------------------------------------------------------------------
	|note:  |__author__:puliu
	|       |__date__:2017/1/13
	|       |__python__:2.7
	----------------------------------------------------------------------------------------------------------------------------------------
	|func:  |读取配置文件配置项，并获取命令行参数(同配置项),覆盖配置项的值，即命令行参数的优先级高于配置文件
	----------------------------------------------------------------------------------------------------------------------------------------
	|api:   |read_config(argv,configPath,configType,noteFlag,splitFlag):读取配置文件配置，获取配置项同名命令行参数并覆盖，返回配置项字典
	|	|xml_config(configPath):从xml配置文件读取所有配置项，返回配置字典
	|	|txt_config(configPath):从txt配置文件读取所有配置项，返回配置字典
	|	|read_xml(nodeName,configPath):读取xml每个node的值，返回字符串
	|	|get_cmd(argv,nodeNameList,paramDict):根据nodeNameList或paramDict的key值，获取命令行参数
	----------------------------------------------------------------------------------------------------------------------------------------
	|frame: |__init__(argv):初始化,定义一些在整个类中使用的数据，并调用其他方法(可以省去实例化类对象后，再调用方法的步骤)
	|       |       ------------------------------------------------------------------------------------------------------------------------
	|	|       |params:|~~~~~~
	|       |       ------------------------------------------------------------------------------------------------------------------------
	|	|	|var:   |self.nodeNameList:配置项名称列表
	|	|	|	|self.makePathList:需要创建目录的配置项名称列表
	|	|	------------------------------------------------------------------------------------------------------------------------   
	|	|	|calls: |read_config():读取配置文件，并获取命令行参数，覆盖配置项，生成配置项同名变量
	|	|	|	|	--------------------------------------------------------------------------------------------------------
	|	|	|	|	|params:|argv:命令行参数，由sys.argv[]获取
	|	|	|	|	|       |[configPath]:配置文件路径，默认值“config.xml”
	|	|	|	|	|       |[configType]:配置文件类型，包括'xml'和'txt'两种，默认值'xml'
	|	|	|	|	|       |[noteFlag]:txt类型配置文件的注释符号，默认值'#'
	|	|	|	|	|       |[splitFlag]:txt类型配置文件的配置项与值的分隔符，默认值'='
	|	|	|	|	|       |[addNode]:需要增加到配置字典中的特殊参数，通常这些参数不包含在配置文件中，可写多个","分隔
	|	|	|	|	--------------------------------------------------------------------------------------------------------   
	|	|	|	|	|calls: |xml_config():获取xml配置文件所有配置
	|	|	|	|	|	|       ----------------------------------------------------------------------------------------
	|       |       |       |       |       |       |params:|configPath:配置文件路径
	|       |       |       |       |       |       ----------------------------------------------------------------------------------------
	|	|	|	|	|	|       |calls: |~~~~~
	|	|	|	|	|	|       ----------------------------------------------------------------------------------------
	|	|	|	|	|	|	|return:|paramDict:{name:value,...}
	|	|	|	|	|       ================================================================================================
	|	|	|	|	|       |txt_config():读取txt配置文件所有配置
	|	|	|	|	|	|	------------------------------------------------------------ ---------------------------
	|       |       |       |       |       |       |params:|configPath:配置文件路径
	|	|	|       |       |       |       ----------------------------------------------------------------------------------------
	|	|	|	|	|	|	|calls: |_read_config_with_node():按node名称读取xml文件
	|	|	|	|	|	|	|       |       ------------------------------------------------------------------------
	|       |       |       |       |       |       |       |       |params:|~~~~~
	|	|	|	|	|	|	|	|	------------------------------------------------------------------------
	|	|	|	|	|	|	|	|	|calls: |read_xml():读取xml文件
	|	|	|	|	|	|	|	|	|	----------------------------------------------------------------
	|	|       |       |       |       |       |       |       |       |params:|nodeName:节点名称
	|	|       |       |       |       |       |       |       |       |       |configPath:配置文件路径
	|	|       |       |       |       |       |       |       |	----------------------------------------------------------------
	|	|       |       |       |       |       |       |       |       |calls: |~~~~~
	|	|       |       |       |       |       |       |       |	----------------------------------------------------------------
	|	|       |       |       |       |       |       |       |       |return:|res:节点内容
	|	|       |       |       |       |       ----------------------------------------------------------------------------------------
	|	|       |       |       |       |       |return:|paramDict:{name:value,...}
	|	|       |       |       |       ================================================================================================
	|	|       |       |       |       |get_cmd():获取命令行参数，覆盖paramDict对应配置项
	|	|       |       |       |       |       ----------------------------------------------------------------------------------------
	|	|       |       |       |       |       |params:|argv:命令行参数列表
	|	|       |       |       |       |       |       |nodeNameList:配置项名称列表
	|	|       |       |       |       |       |       |paramDict:配置项字典
	|	|       |       |       |       |       ----------------------------------------------------------------------------------------
	|	|       |       |       |       |       |calls: |~~~~~
	|	|       |       |       |       |       ----------------------------------------------------------------------------------------
	|	|       |       |       |       |       |return:|paramDict:{name:value,...}
	|	|       |       |       |       ================================================================================================
	|	|       |       |       |       |__make_path():将配置项中是目录参数，自动创建目录，防止目录不存在造成程序异常
	|	|       |       |       |       |       ----------------------------------------------------------------------------------------
	|	|       |       |       |       |       |params:|self.makePathList:目录列表
	|	|       |       |       |	|       ----------------------------------------------------------------------------------------
	|	|       |       |       |	|       |calls: |~~~~~
	|	|       |       |       |	|       ----------------------------------------------------------------------------------------
	|	|       |       |       |	|       |return:|~~~~~
	|	|       |       |       |       ================================================================================================
	|	|       |       |       |       |__set_config_to_var():将所有配置项定义为public变量，可被外部使用
	|	|       |       |       |       |       ----------------------------------------------------------------------------------------
	|	|       |       |       |       |       |params:|paramDict:配置项字典
	|	|       |       |       |	|       ----------------------------------------------------------------------------------------
	|	|       |       |       |	|       |calls: |~~~~~
	|	|       |       |       |	|       ----------------------------------------------------------------------------------------
	|	|       |       |       |	|       |return:|~~~~~
	----------------------------------------------------------------------------------------------------------------------------------------
"""
from xml.dom import minidom
import getopt
import sys

class Config:
	def __init__(self):
		self.makePathList = []
		self.configPath = ""
		self.root = None
		self.nodeNameList = []
		self.paramDict = {}
		self.addNode = ""
		self.noteFlag = "#"
		self.splitFlag = "="
		
	def read_config(self,argv=sys.argv[1:],configPath="./Config.xml",configType="xml",noteFlag="#",splitFlag="=",addNode=""):
		"""read_config:读取配置文件，并获取命令行参数，覆盖配置项，生成配置项同名变量
		
		         param:argv:命令行参数，由sys.argv[]获取
			       [configPath]:配置文件路径，默认值“config.xml”
	                       [configType]:配置文件类型，包括'xml'和'txt'两种，默认值'xml'
	                       [noteFlag]:txt类型配置文件的注释符号，默认值'#'
	                       [splitFlag]:txt类型配置文件的配置项与值的分隔符，默认值'='
			       [addNode]:需要增加到配置字典中的特殊参数，通常这些参数不包含在配置文件中，"node1,node2:value2"
			 return:paramDict:配置项字典，配置项名称为key,配置项值为value
		"""
		self.configPath = configPath
		self.noteFlag = noteFlag
		self.splitFlag = splitFlag
		self.addNode = addNode
		
		if configType == "xml":
			self.paramDict = self.xml_config(self.configPath)
		else:
			self.paramDict = self.txt_config(self.configPath)
		paramDict = self.get_cmd(argv,self.nodeNameList,self.paramDict)
		
		self.__make_path()
		self.__set_config_to_var(paramDict)
		return paramDict
	
	def xml_config(self,configPath):
		paramDict = {}
		if configPath == "":
			return paramDict
			
		fp = open(configPath)
		cList = fp.readlines()
		fp.close()
	
		for line in cList:
			line = line.strip()
			if line.find("<?") != 0 and line.find("<!--") != 0:
				if "<" in line and ">" in line:
					nodeName = line[line.find("<")+1:line.find(">")]
					nodeNameRight = "</" + nodeName + ">"
					if nodeNameRight in line:
						self.nodeNameList.append(nodeName.decode("utf-8"))
						
		paramDict = self.__read_config_with_node()
		return paramDict
	
	def txt_config(self,configPath):
		paramDict = {}
		if configPath == "":
			return paramDict
		
		fp = open(configPath)
		cList = fp.readlines()
		fp.close()
	
		for line in cList:
			line = line.strip()
			if line.find(self.noteFlag) == 0:
				continue
			else:
				if self.splitFlag in line:
					lineList = line.split(self.splitFlag)
					if len(lineList) > 1:
						name = lineList[0].strip()
						value = lineList[1].strip()
						if name != "":
							paramDict[name.decode("utf-8")] = value.decode("utf-8")
		return paramDict
        
	def __read_config_with_node(self):
		paramDict = {}		
		
		for nodeName in self.nodeNameList:
			paramDict[nodeName] = self.read_xml(nodeName,self.configPath)
		return paramDict
			
	def read_xml(self,nodeName,configPath):
		res= ""
		if configPath == "":
			return res
		else:
			if self.root == None:
				conf = minidom.parse(configPath)
				self.root = conf.documentElement

		node = self.root.getElementsByTagName(nodeName)
		if len(node) > 0:
			if len(node[0].childNodes) > 0:
				res = node[0].childNodes[0].data
		return res

	def get_cmd(self,argv,nodeNameList=[],paramDict={}):
		longoptList = []
		if len(nodeNameList) > 0:
			for nodeName in nodeNameList:
				longoptList.append("%s="%nodeName)
		else:
			if len(paramDict) > 0:
				for key in paramDict:
					longoptList.append("%s="%key)
			else:
				return paramDict
			
		if self.addNode != "":
			addNodeList = self.addNode.split(",")
			for node in addNodeList:
				if ":" in node:
					name = node.split(":")[0]
					value = node.split(":")[1]
					longoptList.append("%s="%name)
					paramDict[name] = value					
				else:
					longoptList.append("%s="%node)
					paramDict[node] = ""
	
		opts,args = getopt.getopt(argv, "h:", longopts=longoptList)
		for op,value in opts:
			name = op.replace("--","")
			if value != "":
				try:
					paramDict[name] = value.decode("gbk")
				except:
					paramDict[name] = value.decode("utf-8")
		return paramDict
	
	def __make_path(self):
		for name in self.makePathList:
			if name == "Result":
				path = os.path.dirname(self.paramDict[name])
			else:
				path = self.paramDict[name]
			if os.path.exists(path) == False:
				os.makedirs(path)
				
	def __set_config_to_var(self,paramDict):
		for key in paramDict:
			exec("self.%s=\"%s\""%(key,paramDict[key]))
		
	
if __name__ == "__main__":
	CF = Config()
	#paramDict = CF.read_config([])
	paramDict = CF.read_config([],configPath="Config.txt",configType="txt")
	print CF.chid_delay_reset_timeout
	print CF.aiui_pushnode_url
	