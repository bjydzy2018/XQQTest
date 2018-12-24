del /q/a/f .\log\*.log
del /q/a/f ..\..\Active\SRRes\ResMandarin\log\*.log
del /q/a/f .\win32-api\sr\*
del /q/a/f ..\TestApiSR.xml
C:\Python34\python.exe set_TestISS-AIUI_test_to_run.py TestISS-AIUI.cfg sr_api:1
cd ..\
TestISS-AIUI.exe
cd TestRes
move /y .\log\*.log .\win32-api\sr\
move /y ..\TestApiSR.xml .\win32-api\sr\
move /y ..\..\Active\SRRes\ResMandarin\log\*.log .\win32-api\sr\

pause
