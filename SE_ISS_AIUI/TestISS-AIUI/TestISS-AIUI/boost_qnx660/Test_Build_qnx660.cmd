call D:\qnx660\qnx660-env.bat
make -C ./ -f Makefile.filesystem.qnx660.mk

make -C ./ -f Makefile.system.qnx660.mk

make -C ./ -f Makefile.thread.qnx660.mk
pause
