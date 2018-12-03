#coding=utf-8

'''
author: hcliu
version: 1.0

输入欲监控进程名作为参数。
支持跨平台，需安装psutil。
'''

import psutil
import time
import sys
import os

pname = sys.argv[1]
nowtime = time.strftime('%Y-%m-%d %H-%M-%S', time.localtime(time.time()))
os_type = os.name

ret_file_name = pname+'_'+nowtime+'.txt'
fp = open(ret_file_name, 'w')


def get_proc_by_id(pid):
    return psutil.Process(pid)
	
def get_proc_by_name(pname):
    """ get process by name
    
    return the first process if there are more than one
    """
    for proc in psutil.process_iter():
        try:
            if proc.name().lower() == pname.lower():
                return proc  # return if found one
        except psutil.AccessDenied:
            pass
        except psutil.NoSuchProcess:
            pass
    return None
	
if '__main__' == __name__:  
    while True:
        try:
            proc = get_proc_by_name(pname)
            mem = proc.memory_info()
            cpu = proc.cpu_percent(interval=None) if os_type=='posix' else proc.cpu_percent(interval=None) / psutil.cpu_count()

            fp.write('mem\t'+'%.2f' % (float(mem[0])/1024/1024)+'\tcpu\t'+str(cpu)+'\n')
            print('mem','%.2f' % (float(mem[0])/1024/1024),'cpu',cpu)
        except Exception:
            fp.write('mem\t'+'-1\tcpu\t-1\n')
            print('mem',-1,'cpu',-1)

        time.sleep(1)
