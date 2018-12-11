cd %~dp0

python.exe ./aiui_eff.py TestISS-AIUI_android.log isssr_android.log speed_result_on_android.csv

python.exe ./aiui_eff.py TestISS-AIUI_win32.log isssr_win32.log speed_result_on_win32.csv

pause.exe
