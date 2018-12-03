#-*- conding:utf-8 -*-
import sys
   
   
def main(resouceDir,resultDir):
    f = open(resouceDir,'r')
    out=open(resultDir,'w+')
    for eachLine in f:
        line=eachLine.strip()
        #print(line)
        where1 = line.find('after SPWGetResult:') 
        if(where1>=0):     
            str = line[where1:]
            #print(str)
            where2 = str.find(': ')
            strstr = str[where2+2:]
            #print (strstr)
            out.writelines(strstr)
            out.writelines('\n')
    f.close()
    out.close()