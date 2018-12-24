call D:\QNX650\QNX650-env.bat
make -C ./ -f Makefile.filesystem.qnx650.mk clean
make -C ./ -f Makefile.filesystem.qnx650.mk

make -C ./ -f Makefile.system.qnx650.mk clean
make -C ./ -f Makefile.system.qnx650.mk

make -C ./ -f Makefile.thread.qnx650.mk clean
make -C ./ -f Makefile.thread.qnx650.mk
pause