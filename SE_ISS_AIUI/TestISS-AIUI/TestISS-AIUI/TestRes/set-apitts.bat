del /q/a/f .\log\*.log
del /q/a/f ..\..\Active\TTSRes\log\*.log
del /q/a/f .\win32-api\tts\*
del /q/a/f ..\TestApiTTS.xml
C:\Python34\python.exe set_TestISS-AIUI_test_to_run.py TestISS-AIUI.cfg tts_api:1
cd ..\
TestISS-AIUI.exe
cd TestRes
move /y .\log\*.log .\win32-api\tts\
move /y ..\TestApiTTS.xml .\win32-api\tts\
move /y ..\..\Active\TTSRes\log\*.log .\win32-api\tts\

pause
