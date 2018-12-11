cd %~dp0

python.exe ./aiui_eff.py TestISS-AIUI-android-local.log isssr-android-local.log speed_result_on_android.csv vad前后端点.txt

python.exe ./aiui_eff.py TestISS-AIUI-win32-local.log isssr-win32-local.log speed_result_on_win32.csv vad前后端点.txt


pause.exe
