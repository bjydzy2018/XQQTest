#-*- coding: utf-8 -*-
try:
    from chardet.universaldetector import UniversalDetector
    IsAuto = True
except  ImportError:
    IsAuto = False
import os
import os.path
import glob
 
def Convert_Auto( filename,out_enc="utf-8" ):  
    ''' Re-encode text file with auto detec current encode. Need chardet Lib. 
Input Parameter:
        filename: full path and file name, e.g. c:\dir1\file.txt
        out_enc: new encode. Default as 'utf-8'
Output Parameter
        None'''
    try:  
        f=open(filename,'rb')
        b= b' '
        b+=f.read(1024)
        u=UniversalDetector()
        u.reset()
        u.feed(b)
        u.close()
        f.seek(0)
        b=f.read()
        f.close()
        in_enc=u.result['encoding']
        new_content=b.decode(in_enc, 'ignore')
        f=open(filename, 'w', encoding=out_enc)
        f.write(new_content)
        f.close()
        print ("Success: "+filename+" converted from "+ in_enc+" to "+out_enc +" !")
    except IOError: 
        print ("Error: "+filename+" FAIL to converted from "+ in_enc+" to "+out_enc+" !" )
 
def Convert_Manu( filename,in_enc='gbk', out_enc="utf-8" ):  
    ''' Re-encode text file with manual decide input text encode.
Input Parameter:
        filename: full path and file name, e.g. c:\dir1\file.txt
        in_enc:  current encode. Default as 'gbk'
        out_enc: new encode. Default as 'utf-8'
Output Parameter
        None'''
    try:  
        print ("convert " + filename)
        f=open(filename,'rb')
        b=f.read()
        f.close()
        new_content=b.decode(in_enc, 'ignore')
        f=open(filename, 'w', encoding=out_enc)
        f.write(new_content)
        f.close()
        print ("Success: "+filename+" converted from "+ in_enc+" to "+out_enc +" !")
    except IOError: 
        print ("Error: "+filename+" FAIL to converted from "+ in_enc+" to "+out_enc+" !" )
 
 
def explore(dir, IsLoopSubDIR=True):
    '''Convert files encoding.
    Input:  
        dir         : Current folder
        IsLoopSubDIR:   True -- Include files in sub folder
                        False-- Only include files in current folder
    Output:
        NONE
    '''
    if IsLoopSubDIR:
        flist=getSubFileList(dir, '.txt')
    else:
        flist=getCurrFileList(dir, '.txt')
    for fname in flist:
        if IsAuto:
            Convert_Auto(fname, 'utf-8')
        else:
            Convert_Manu(fname, 'gbk', 'utf-8')
 
     
def getSubFileList(dir, suffix=''):
    '''Get all file list with specified  suffix under current folder(Include sub folder)
    Input:  
        dir     :   Current folder
        suffix  :   default to blank, means select all files.
    Output:
        File list
    '''
    flist=[]
    for root, dirs, files in os.walk(dir):
        for name in files:
            if name.endswith(suffix):
                flist.append(os.path.join(root,  name))
    return flist
 
def getCurrFileList(dir, suffix=''):
    '''Get all file list with specified suffix under current level folder
    Input:  
        dir     :   Current folder
        suffix  :   default to blank, means select all files.
    Output:
        File list
    '''
    if suffix=='':   
        files=glob.glob('*')
    else:
        files=glob.glob('*'+suffix)
    flist=[]    
    for f in files:
        flist.append(os.path.join(dir, f))
    return flist
         
         
def main():  
    explore(os.getcwd(), True)
     
if __name__ == "__main__":  
   main() 