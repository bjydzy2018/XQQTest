include ../../../Source/GNUMakeConfig/defines.drc4.mk

TARGET := TestISS-AIUI-linux64

INCLUDE = -I ../../../Source/VACommon \
-I ../../../Source/VACommon/log \
-I ../../../Source/ISS_API \
-I ./gtest \
-I ./

STATIC_LIBS = ./vacommon_linux64/libVACommon.a \
./gtest_linux64/gtest.a

SHARED_LIBS = -lpthread \
-L../../../../Project/07.Release/linux64/bin \
-lAitalkOne -lcata -lcataIndex -liFlyNLI -liFlyPResBuild -lissauth  \
-lisscata -lissmvw -lisssr -lisstts -livw -livwgram -lmsc -lnamext -lSpWord

CPPFLAGS += -DISS_SR_3
CFLAGS += -DISS_SR_3
CXXFLAGS += -DISS_SR_3

all: Makefile.linux64.mk $(TARGET)
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

include ../../../Source/GNUMakeConfig/rules.mk
