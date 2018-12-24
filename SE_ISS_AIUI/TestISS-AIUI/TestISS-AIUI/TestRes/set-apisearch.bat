del /q/a/f .\log\*.log
del /q/a/f .\win32-api\search\*
del /q/a/f ..\TestApiSearch.xml
del /q/a/f ..\..\Active\SRRes\SearchRes\log\*.log
C:\Python34\python.exe set_TestISS-AIUI_test_to_run.py TestISS-AIUI.cfg search_api:1
cd ..\
TestISS-AIUI.exe
cd TestRes
move /y .\log\*.log .\win32-api\search\
move /y ..\TestApiSearch.xml .\win32-api\search\
move /y ..\..\Active\SRRes\SearchRes\log\*.log .\win32-api\search\

pause
