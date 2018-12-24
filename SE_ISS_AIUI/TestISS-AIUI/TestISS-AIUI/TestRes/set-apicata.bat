del /q/a/f .\log\*.log
del /q/a/f .\win32-api\cata\*
del /q/a/f ..\TestApiCATA.xml
del /q/a/f ..\..\CataRes\log\*.log
C:\Python34\python.exe set_TestISS-AIUI_test_to_run.py TestISS-AIUI.cfg cata_api:1
cd ..\
TestISS-AIUI.exe
cd TestRes
move /y .\log\*.log .\win32-api\cata\
move /y ..\TestApiCATA.xml .\win32-api\cata\
move /y ..\..\CataRes\log\*.log .\win32-api\cata\

pause
