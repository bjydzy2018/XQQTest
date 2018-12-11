cd %~dp0




python3 通过工具结果和tagid_统计最终结果的响应时间.py ./android/speed-wifi/cloud/TestISS-AIUI.log vad前后端点.txt android_speed_cloud-wifi.csv
python3 通过工具结果和tagid_统计最终结果的响应时间.py ./android/speed-wifi/mix/TestISS-AIUI.log vad前后端点.txt android_speed_mix-wifi.csv
python3 通过工具结果和tagid_统计最终结果的响应时间.py ./android/speed-wifi/local/TestISS-AIUI.log vad前后端点.txt android_speed_local-wifi.csv

python3 通过工具结果和tagid_统计最终结果的响应时间.py ./linux_arm_32/cloud/TestISS-AIUI.log vad前后端点.txt linux_arm_32_speed_cloud-wifi.csv
python3 通过工具结果和tagid_统计最终结果的响应时间.py ./linux_arm_32/mix/TestISS-AIUI.log vad前后端点.txt linux_arm_32_speed_mix-wifi.csv
python3 通过工具结果和tagid_统计最终结果的响应时间.py ./linux_arm_32/local/TestISS-AIUI.log vad前后端点.txt linux_arm_32_speed_local-wifi.csv

python3 通过工具结果和tagid_统计最终结果的响应时间.py ./x86_64/cloud/TestISS-AIUI.log vad前后端点.txt x86_64_speed_cloud-wifi.csv
python3 通过工具结果和tagid_统计最终结果的响应时间.py ./x86_64/mix/TestISS-AIUI.log vad前后端点.txt x86_64_speed_mix-wifi.csv
python3 通过工具结果和tagid_统计最终结果的响应时间.py ./x86_64/local/TestISS-AIUI.log vad前后端点.txt x86_64_speed_local-wifi.csv


pause.exe
