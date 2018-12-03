# -*- coding: utf-8 -*-
"""Log

     main function:
         自定义的日志模块，提供规范格式、不同级别的日志
         
     frame:
         __class__:
             Log:日志类
             
         __import__:
             __standard library__:
                 logging:
                 os:
             
             __custom library__:
                 -----
             
     note:
         __author__:puliu
	 __date__:2017/1/13
	 __python__:2.7
"""

import logging
import os


class Log:
    """Log
    
         function:
             日志模块，提供规范格式、不同级别的日志
             
         detail:
             日志格式:
                 %(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s
                 
             日志级别:
                 1:info
                 2:error
                 3:debug
                 4:warning
                 
             logging.basicConfig:
                 filename: 指定日志文件名
                 filemode: 和file函数意义相同，指定日志文件的打开模式，'w'或'a'
                 format: 指定输出的格式和内容，format可以输出很多有用信息，如上例所示:
                     %(levelno)s: 打印日志级别的数值
                     %(levelname)s: 打印日志级别名称
                     %(pathname)s: 打印当前执行程序的路径，其实就是sys.argv[0]
                     %(filename)s: 打印当前执行程序名
                     %(funcName)s: 打印日志的当前函数
                     %(lineno)d: 打印日志的当前行号
                     %(asctime)s: 打印日志的时间
                     %(thread)d: 打印线程ID
                     %(threadName)s: 打印线程名称
                     %(process)d: 打印进程ID
                     %(message)s: 打印日志信息
                 datefmt: 指定时间格式，同time.strftime()
                 level: 设置日志级别，默认为logging.WARNING
                 stream: 指定将日志的输出流，可以指定输出到sys.stderr,sys.stdout或者文件，默认输出到sys.stderr，当stream和filename同时指定时，stream被忽略
             
         frame:
             params:
                 logPath:日志目录，不包括文件名(文件名称自动生成"当前时间_本机IP_log.txt")
                 [gRunTime]:当前时间,默认当前时间
                 [gLocalIp]:本机IP,默认本机ip
                 [bak]:是否使用备份的方式写日志,默认False,(每次生成一个新的日志文件写入本次日志，将历史日志写入自动生成的备份文件中，并删除)，True/False
                 
             calls:
                 write_log():
                     function:
                         提供给外部调用的写日志方法
                         
                     params:
                         logType:日志级别，1:info、2:error、3:debug、4:warning
                         content:日志内容
                 ===================================================================================================
                 set_bak():将历史日志写入备份文件中，并删除，只保留最新日志
             
         
    """
    def __init__(self,logPath,runTime="",localIp="",bak=False,level=2):
        levelDict = {1:logging.DEBUG,2:logging.INFO,3:logging.WARNING,4:logging.ERROR,5:logging.CRITICAL}
        if os.path.exists(logPath) == False:
            os.makedirs(logPath)
        self.logPath = logPath
            
        if runTime == "":
            import time
            self.asctime = time.strftime('%Y-%m-%d_%H-%M-%S',time.localtime(time.time()))      #脚本开始运行时间
        else:
            self.asctime = runTime
        
        if localIp == "":
            import socket
            self.localIp = socket.gethostbyname(socket.getfqdn(socket.gethostname()))          #本机IP
        else:
            self.localIp = localIp
        
        self.logName = logPath + "/" + self.asctime + "_" + self.localIp +  "_log"  + ".txt"
        
        if bak == True:
            self.logBakPath = logPath + "/bak_log"
            if os.path.exists(self.logBakPath) == False:
                os.makedirs(self.logBakPath)            
            self.set_bak()
        
        logging.basicConfig(level=logging.DEBUG,\
                            format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',\
                            datefmt='%Y-%m-%d %H:%M:%S',\
                            filename=self.logName,\
                            filemode='w')
        
        #定义一个StreamHandler，将INFO级别或更高的日志信息打印到标准错误，并将其添加到当前的日志处理对象
        #日志级别:CRITICAL > ERROR > WARNING > INFO > DEBUG,NOTSET
        console = logging.StreamHandler()
        console.setLevel(levelDict[level])
        formatter = logging.Formatter('%(asctime)-12s: %(levelname)-8s %(message)s')
        console.setFormatter(formatter)
        logging.getLogger('').addHandler(console)
        
    #写日志
    #日志级别:critical > error > warning > info > debug,notset
    def write_log(self,logType,content):
        if logType == 1:          
            logging.debug(content)
        elif logType == 2:
            logging.info(content)
        elif logType == 3:
            logging.warning(content)
        elif logType == 4:
            logging.error(content)
        elif logType == 5:
            logging.critical(content)
        else:
            logging.info(content)
            
    #写备份日志
    def set_bak(self):
        bakLogName = self._get_bakLog()
        lastLogList = self._get_lastLog()
        self._copy_lastLog_to_bak(lastLogList, bakLogName)
                
    def _get_bakLog(self):
        fileList,bakLogName,lastLog,maxAtime = [],"","",0.0  
        for parent,fileDirs,fileNames in os.walk(self.logBakPath):
            for fileName in fileNames:
                fileList.append(os.path.join(parent,fileName))
        
        if len(fileList) == 0:
            bakLogName = self.logBakPath + "/" + self.asctime + "_" + self.localIp +  "_log"  + ".txt"
        else:
            for bakLog in fileList:
                atime = os.path.getatime(bakLog)
                if atime > maxAtime:
                    maxAtime = atime
                    lastLog = bakLog
            
            size = os.path.getsize(lastLog)
            if size/1024/1024 > 10:
                bakLogName = self.logBakPath + "/" + self.asctime + "_" + self.localIp +  "_log"  + ".txt"
            else:
                bakLogName = lastLog
        return bakLogName
    
    def _get_lastLog(self):
        lastLogList = []
        pathList = os.listdir(self.logPath)
        for sPath in pathList:
            if len(sPath)>20:
                if sPath[4] == "-" and sPath[7] == "-" and sPath[10] == "_" and "_log.txt" in sPath:
                    lastLogList.append(os.path.join(self.logPath,sPath))
        return lastLogList
    
    def _copy_lastLog_to_bak(self,lastLogList,bakLogName):
        flag = 0
        if os.path.exists(bakLogName):
            flag = 1
            fa = open(bakLogName,"a+")
        else:
            fa = open(bakLogName,"w")
        for lastLog in lastLogList:
            fp = open(lastLog)
            lastLogContent = fp.read()
            fp.close()
            
            if flag == 1:
                fa.write("\n\n")
            fa.write("%s\n"%('*'*30))
            fa.write("%s%s%s\n"%('*'*10,os.path.basename(lastLog),'*'*10))
            fa.write("%s\n"%('*'*30))
            
            fa.write(lastLogContent)
            os.remove(lastLog)
            flag = 1
        fa.close()    
