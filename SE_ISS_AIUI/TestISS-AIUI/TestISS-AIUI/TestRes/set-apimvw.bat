del /q/a/f .\log\*.log
del /q/a/f ..\..\Active\MVWRes\log\*.log
del /q/a/f .\win32-api\mvw\*
del /q/a/f ..\TestApiMVW.xml
C:\Python34\python.exe set_TestISS-AIUI_test_to_run.py TestISS-AIUI.cfg mvw_api:1
cd ..\
TestISS-AIUI.exe
cd TestRes
move /y .\log\*.log .\win32-api\mvw\
move /y ..\TestApiMVW.xml .\win32-api\mvw\
move /y ..\..\Active\MVWRes\log\*.log .\win32-api\mvw\

pause
