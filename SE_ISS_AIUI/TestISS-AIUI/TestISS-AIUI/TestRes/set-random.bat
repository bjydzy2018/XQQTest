del /q/a/f .\log\*.log
del /q/a/f ..\..\Active\SRRes\ResMandarin\log\*.log
del /q/a/f .\win32-random\sr\*
del /q/a/f ..\performance_TestISS-AIUI.exe.csv
C:\Python34\python.exe set_TestISS-AIUI_test_to_run.py TestISS-AIUI.cfg sr_api_random:1
cd ..\
start /b TestISS-AIUI.exe
C:\Python34\python.exe D:\WorkDir\getPerformInfo_Win\getPerformInfo_Win.py TestISS-AIUI.exe 1
cd TestRes
move /y .\log\*.log .\win32-random\sr\
move /y ..\..\Active\SRRes\ResMandarin\log\*.log .\win32-random\sr\
move /y ..\performance_TestISS-AIUI.exe.csv .\win32-random\sr\


del /q/a/f .\log\*.log
del /q/a/f ..\..\Active\MVWRes\log\*.log
del /q/a/f .\win32-random\mvw\*
del /q/a/f ..\performance_TestISS-AIUI.exe.csv
C:\Python34\python.exe set_TestISS-AIUI_test_to_run.py TestISS-AIUI.cfg mvw_api_random:1
cd ..\
start /b TestISS-AIUI.exe
C:\Python34\python.exe D:\WorkDir\getPerformInfo_Win\getPerformInfo_Win.py TestISS-AIUI.exe 1
cd TestRes
move /y .\log\*.log .\win32-random\mvw\
move /y ..\..\Active\MVWRes\log\*.log .\win32-random\mvw\
move /y ..\performance_TestISS-AIUI.exe.csv .\win32-random\mvw\


::del /q/a/f .\log\*.log
::del /q/a/f ..\..\Active\TTSRes\log\*.log
::del /q/a/f .\win32-random\tts\*
::del /q/a/f ..\performance_TestISS-AIUI.exe.csv
::C:\Python34\python.exe set_TestISS-AIUI_test_to_run.py TestISS-AIUI.cfg tts_api_random:1
::cd ..\
::start /b TestISS-AIUI.exe
::C:\Python34\python.exe D:\WorkDir\getPerformInfo_Win\getPerformInfo_Win.py TestISS-AIUI.exe 1
::cd TestRes
::move /y .\log\*.log .\win32-random\tts\
::move /y ..\..\Active\TTSRes\log\*.log .\win32-random\tts\
::move /y ..\performance_TestISS-AIUI.exe.csv .\win32-random\tts\

del /q/a/f .\log\*.log
del /q/a/f ..\..\CataRes\log\*.log
del /q/a/f .\win32-random\cata\*
del /q/a/f ..\performance_TestISS-AIUI.exe.csv

C:\Python34\python.exe set_TestISS-AIUI_test_to_run.py TestISS-AIUI.cfg cata_api_random:1
cd ..\
start /b TestISS-AIUI.exe
C:\Python34\python.exe D:\WorkDir\getPerformInfo_Win\getPerformInfo_Win.py TestISS-AIUI.exe 1
cd TestRes
move /y .\log\*.log .\win32-random\cata\
move /y ..\..\CataRes\log\*.log .\win32-random\cata\
move /y ..\performance_TestISS-AIUI.exe.csv .\win32-random\cata\

pause
