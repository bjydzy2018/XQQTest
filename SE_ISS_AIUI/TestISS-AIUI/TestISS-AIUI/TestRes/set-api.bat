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


::del /q/a/f .\log\*.log
::del /q/a/f ..\..\Active\TTSRes\log\*.log
::del /q/a/f .\win32-api\tts\*
::del /q/a/f ..\TestApiTTS.xml
::C:\Python34\python.exe set_TestISS-AIUI_test_to_run.py TestISS-AIUI.cfg tts_api:1
::cd ..\
::TestISS-AIUI.exe
::cd TestRes
::move /y .\log\*.log .\win32-api\tts\
::move /y ..\TestApiTTS.xml .\win32-api\tts\
::move /y ..\..\Active\TTSRes\log\*.log .\win32-api\tts\


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
