#coding=utf-8

__author__ = 'hcliu'

from urllib.request import urlopen
import os
import shutil

def down_dll():
    root_dir = 'Engine-Dll'

    src = {
        'sr.zip': 'http://192.168.75.131/nexus/content/repositories/SE-3rdParty/TestPlatform/ASR.Car1.5-Autobuild-TEST/1.5/99999/ASR.Car1.5-AutobuildTEST-win32.zip',
        'mvw.zip': 'http://192.168.75.131/nexus/content/repositories/SE-3rdParty/TestPlatform/Ivw6.1-Autobuild-TEST/6.1/99999/Ivw6.1-AutobuildTEST-win32.zip',
        'nlu.zip': 'http://192.168.75.131/nexus/content/repositories/SE-3rdParty/TestPlatform/CheZaiTaoJian-HIPPO3.5-TEST/4.0/99999/CheZaiTaoJian-HIPPO3.5TEST-WIN32.zip',
        'cata-srh.zip': 'http://192.168.75.131/nexus/content/repositories/SE-3rdParty/TestPlatform/CATA1.0-Search-TEST/1.0/99999/CATA1.0-SearchTEST-CATA1.0IndexTESTWIN32.zip',
        'cata-index.zip': 'http://192.168.75.131/nexus/content/repositories/SE-3rdParty/TestPlatform/CATA1.0-Index-TEST/1.0/99999/CATA1.0-IndexTEST-CATA1.0IndexTESTWIN32.zip',
        'spword.zip': 'http://192.168.75.131/nexus/content/repositories/SE-3rdParty/TestPlatform/CheZaiTaoJian-SPWORD-TEST/2.0/99999/CheZaiTaoJian-SPWORDTEST-win32.zip'
    }

    if os.path.exists(root_dir):
        print('dir exists, deleting...')
        shutil.rmtree(root_dir)

    print('creating dir:', root_dir)
    os.mkdir(root_dir)

    for m in src:
        url = src[m]
        f = urlopen(url)

        print('downloading:', m)
        data = f.read() 
        with open(root_dir+'/'+m, "wb") as fp:
            fp.write(data)

def down_res():
    root_dir = 'Engine-Res'

    src = {
        'cata-res.zip': 'http://192.168.75.131/nexus/content/repositories/SE-3rdParty/TestPlatform/CATA1.0-Index-TEST/1.0/99999/CATA1.0-IndexTEST-resource.cata.zip',
        'mvw-res.zip': 'http://192.168.75.131/nexus/content/repositories/SE-3rdParty/TestPlatform/Ivw6.1-Autobuild-TEST/6.1/99999/Ivw6.1-AutobuildTEST-irf.zip'
    }

    if os.path.exists(root_dir):
        print('dir exists, deleting...')
        shutil.rmtree(root_dir)

    print('creating dir:', root_dir)
    os.mkdir(root_dir)

    for m in src:
        url = src[m]
        f = urlopen(url)

        print('downloading:', m)
        data = f.read() 
        with open(root_dir+'/'+m, "wb") as fp:
            fp.write(data)

def down_result():
    root_dir = 'Engine-Result'

    src = {
        'rslt_srh_all.txt': '\\\\192.168.57.37\\workspace\\p_CATA_cata1.0_smoke_win32_外网\\label\\192.168.57.37\\srh_out_new\\rslt_srh_all.txt',
        'cancel_rlt.txt': '\\\\192.168.57.37\\workspace\\p_Ivw6.1_内核套件一致性效果测试\\label\\192.168.57.37\\bin\\testRlt\\cancel\\cancel_rlt.txt',
        'nhyyzl_rlt.txt': '\\\\192.168.57.37\\workspace\\p_Ivw6.1_内核套件一致性效果测试\\label\\192.168.57.37\\bin\\testRlt\\nhyyzl\\nhyyzl_rlt.txt',
        '取消_rlt.txt': '\\\\192.168.57.37\\workspace\\p_Ivw6.1_内核套件一致性效果测试\\label\\192.168.57.37\\bin\\testRlt\\quxiao\\取消_rlt.txt',
        'xanh_rlt.txt': '\\\\192.168.57.37\\workspace\\p_Ivw6.1_内核套件一致性效果测试\\label\\192.168.57.37\\bin\\testRlt\\xanh\\xanh_rlt.txt',
        'result_performance_SR.txt': '\\\\192.168.57.37\\workspace\\p_SpWord1.0_spw_effect_win32\\label\\192.168.57.37\\bin\\result\\result_performance_SR.txt'
    }

    if os.path.exists(root_dir):
        print('dir exists, deleting...')
        shutil.rmtree(root_dir)

    print('creating dir:', root_dir)
    os.mkdir(root_dir)

    for m in src:
        url = src[m]
        if m == 'rslt_srh_all.txt':
            with open(url, encoding='utf-8') as f:
                print('downloading:', m)
                data = f.read() 
                with open(root_dir+'/'+m, "w", encoding='utf-8') as fp:
                    fp.write(data)
        else:
            with open(url, encoding='gbk') as f:
                    print('downloading:', m)
                    data = f.read() 
                    with open(root_dir+'/'+m, "w", encoding='gbk') as fp:
                        fp.write(data)

if __name__ == '__main__':
    down_dll()
    down_res()
    down_result()
