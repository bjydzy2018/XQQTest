include ../../../Source/GNUMakeConfig/defines.drc4.mk

TARGET := TestISS-AIUI-linux64

INCLUDE = -I ../../../Source/libSearchForNli \
-I ../../../Source/VACommon \
-I ../../../Source/VACommon/log \
-I ../../../Source/ISS_API \
-I ./gtest \
-I ./boost \
-I ./

STATIC_LIBS = ../../../Source/VACommon/libVACommon.a \
./gtest_linux64/gtest.a \
./boost_linux64/libboost_system.a \
./boost_linux64/libboost_filesystem.a \
./boost_linux64/libboost_thread.a

SHARED_LIBS = -lpthread \
-L../../../../Project/07.Release/linux64/bin \
-lAitalkOne -laiui -lcata -lcataIndex -liFlyNLI -liFlyPResBuild -lissauth  \
-lisscata -lissmvw -lisssr -lisstts -livw -livwgram -lmsc -lnamext -lSearchForNli -lSpWord

all: Makefile.linux64.mk makeCommon $(TARGET)
	cp $(TARGET) ../../../../Project/07.Release/linux64/bin/

clean:
	@echo -e '\e[1;34mClean... \e[0m'
	-@rm -rf $(TARGET) *.o *.gch *.d *.P ../../../../Project/07.Release/linux64/bin/$(TARGET)

rebuild:clean all

distclean:clean

.PHONY:all clean rebuild distclean

$(TARGET): $(OBJS) $(STATIC_LIBS)
	@echo -e '\e[1;34mGenerate $@...\e[0m'
	-@rm -rf $(TARGET)
	$(LD) $(LDFLAGS) $(OBJS) $(STATIC_LIBS) -o $@ $(SHARED_LIBS)

makeCommon:
	@$(MAKE) -C ../../../Source/VACommon -f Makefile.x64.mk

include ../../../Source/GNUMakeConfig/rules.mk
