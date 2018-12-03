#-*-coding:utf-8-*- 
'''
Created on 2016-4-11

@author: jianyin
'''

#import json,re
#import xlwt,xlrd
#import codecs
import csv,sys
#from setuptools.command.build_ext import if_dl
from itertools import islice
#Result_content ISR_Result

def fun(infile,outfile):
    column = 0
    reader=csv.reader(open(infile, 'r'))
    for i,rows in enumerate(reader):
        if i == 0:
            row = rows
            #print(row)
            try:
                column = row.index('ISR_Result')
            except:
                column = row.index(' Result_content')
            main(column,reader,outfile)
    return 0

def main(col,reader,outfile):
    fp = open(outfile,"w")
    for items in islice(reader, 1, None):
        try:
            #print(items[0],items[col])
            if items[col]=="":
                items[col]=".."
            fp.write(items[0]+"\n"+items[col]+"\n"+"Vad:\t"+items[12]+"|"+items[13]+"\n"+"."+"\n")
        except:
            print("not Result")
            fp.write("not Result")
    fp.close()

if __name__ == '__main__':
    fun()