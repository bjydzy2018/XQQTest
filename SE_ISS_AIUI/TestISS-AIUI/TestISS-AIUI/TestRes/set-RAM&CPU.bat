del /q/a/f .\log\*.log
del /q/a/f .\sr\batch\ret\sr_time_chinese_local.csv
del /q/a/f ..\..\Active\SRRes\ResMandarin\log\*.log
del /q/a/f .\win32-output\sr\speed\*
del /q/a/f ..\performance_TestISS-AIUI.exe.csv
C:\Python34\python.exe set_TestISS-AIUI_test_to_run.py TestISS-AIUI.cfg sr_batch:1
C:\Python34\python.exe set_TestISS-AIUI_cfg.py TestISS-AIUI.cfg sr_time_ch_local
cd ..\
start /b TestISS-AIUI.exe
C:\Python34\python.exe D:\WorkDir\getPerformInfo_Win\getPerformInfo_Win.py TestISS-AIUI.exe 1
cd TestRes
move /y .\log\*.log .\win32-output\sr\speed\
move /y .\sr\batch\ret\sr_time_chinese_local.csv .\win32-output\sr\speed\
move /y ..\..\Active\SRRes\ResMandarin\log\*.log .\win32-output\sr\speed\
move /y ..\performance_TestISS-AIUI.exe.csv .\win32-output\sr\speed\


del /q/a/f .\log\*.log
del /q/a/f .\sr\consistent\ret\sr_consistent.txt
del /q/a/f ..\..\Active\SRRes\ResMandarin\log\*.log
del /q/a/f .\win32-output\sr\local\*
del /q/a/f ..\performance_TestISS-AIUI.exe.csv
C:\Python34\python.exe set_TestISS-AIUI_test_to_run.py TestISS-AIUI.cfg sr_consistent:1
C:\Python34\python.exe set_TestISS-AIUI_cfg_sr_consistent.py TestISS-AIUI.cfg sr_consistent_ch_local
cd ..\
start /b TestISS-AIUI.exe
C:\Python34\python.exe D:\WorkDir\getPerformInfo_Win\getPerformInfo_Win.py TestISS-AIUI.exe 1
cd TestRes
move /y .\log\*.log .\win32-output\sr\local\
move /y .\sr\consistent\ret\sr_consistent.txt .\win32-output\sr\local\
move /y ..\..\Active\SRRes\ResMandarin\log\*.log .\win32-output\sr\local\
move /y ..\performance_TestISS-AIUI.exe.csv .\win32-output\sr\local\


del /q/a/f .\log\*.log
del /q/a/f .\sr\consistent\ret\sr_consistent.txt
del /q/a/f ..\..\Active\SRRes\ResMandarin\log\*.log
del /q/a/f .\win32-output\sr\cloud\*
del /q/a/f ..\performance_TestISS-AIUI.exe.csv
C:\Python34\python.exe set_TestISS-AIUI_cfg_sr_consistent.py TestISS-AIUI.cfg sr_consistent_ch_cloud
cd ..\
start /b TestISS-AIUI.exe
C:\Python34\python.exe D:\WorkDir\getPerformInfo_Win\getPerformInfo_Win.py TestISS-AIUI.exe 1
cd TestRes
move /y .\log\*.log .\win32-output\sr\cloud\
move /y .\sr\consistent\ret\sr_consistent.txt .\win32-output\sr\cloud\
move /y ..\..\Active\SRRes\ResMandarin\log\*.log .\win32-output\sr\cloud\
move /y ..\performance_TestISS-AIUI.exe.csv .\win32-output\sr\cloud\


del /q/a/f .\log\*.log
del /q/a/f .\sr\consistent\ret\sr_consistent.txt
del /q/a/f ..\..\Active\SRRes\ResMandarin\log\*.log
del /q/a/f .\win32-output\sr\mix\*
del /q/a/f ..\performance_TestISS-AIUI.exe.csv
C:\Python34\python.exe set_TestISS-AIUI_cfg_sr_consistent.py TestISS-AIUI.cfg sr_consistent_ch_mix
cd ..\
start /b TestISS-AIUI.exe
C:\Python34\python.exe D:\WorkDir\getPerformInfo_Win\getPerformInfo_Win.py TestISS-AIUI.exe 1
cd TestRes
move /y .\log\*.log .\win32-output\sr\mix\
move /y .\sr\consistent\ret\sr_consistent.txt .\win32-output\sr\mix\
move /y ..\..\Active\SRRes\ResMandarin\log\*.log .\win32-output\sr\mix\
move /y ..\performance_TestISS-AIUI.exe.csv .\win32-output\sr\mix\
pause
