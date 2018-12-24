del /q/a/f .\log\*.log
del /q/a/f ..\..\Active\TTSRes\log\*.log
del /q/a/f .\win32-random\tts\*
del /q/a/f ..\performance_TestISS-AIUI.exe.csv
C:\Python34\python.exe set_TestISS-AIUI_test_to_run.py TestISS-AIUI.cfg tts_api_random:1
cd ..\
start /b TestISS-AIUI.exe
C:\Python34\python.exe D:\WorkDir\getPerformInfo_Win\getPerformInfo_Win.py TestISS-AIUI.exe 1
cd TestRes
move /y .\log\*.log .\win32-random\tts\
move /y ..\..\Active\TTSRes\log\*.log .\win32-random\tts\
move /y ..\performance_TestISS-AIUI.exe.csv .\win32-random\tts\

pause
