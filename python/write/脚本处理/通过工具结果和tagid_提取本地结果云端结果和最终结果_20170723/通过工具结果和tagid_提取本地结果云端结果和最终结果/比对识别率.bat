cd %~dp0

::C:\Python34\python.exe 比对识别率.py ./out_mix_result.csv 离线效果测试验收集_用于20170707语义套件一致性对比.xlsx ./out_mix_result_effect_report.csv
C:\Python34\python.exe 比对识别率.py ./out_cloud_result.csv 离线效果测试验收集_用于20170707语义套件一致性对比.xlsx ./out_cloud_result_effect_report.csv
::C:\Python34\python.exe 比对识别率.py ./out_local_result.csv 离线效果测试验收集_用于20170707语义套件一致性对比.xlsx ./out_local_result_effect_report.csv

C:\Python34\python.exe 比对识别率.py ./out_cloud_result-old.csv 离线效果测试验收集_用于20170707语义套件一致性对比.xlsx ./out_cloud_result_effect_report-old.csv

pause.exe
