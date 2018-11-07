chmod 555 TestISS-AIUI
export LD_LIBRARY_PATH=`pwd`:LD_LIBRARY_PATH

./TestISS-AIUI $1

# cpu、内存监控命令
# 需要安装iTest
# adb shell am broadcast -a monitorStart --es monitor cpu,pss --es pkg ./TestISS-AIUI --es interval 2
