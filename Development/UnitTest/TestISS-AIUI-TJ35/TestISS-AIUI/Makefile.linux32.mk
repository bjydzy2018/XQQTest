include ../../../Source/GNUMakeConfig/defines.arm.mk

TARGET := TestISS-AIUI-linuxarm32

INCLUDE = -I ../../../Source/VACommon \
-I ../../../Source/VACommon/log \
-I ../../../Source/ISS_API \
-I ./gtest \
-I ./

STATIC_LIBS = ./vacommon_linuxarm32/libVACommon.a \
./gtest_linuxarm32/gtest.a

SHARED_LIBS = -lpthread \
-L../../../../Project/07.Release/linux_arm/bin \
-lAitalkOne -laiui -lcata -lcataIndex -lhardinfo -liFlyNLI -liFlyPResBuild -lissauth  \
-lisscata -lissmvw -lissseopt -lisssr -lisstts -livw -livwgram -llesl -lnamext -lSpWord

CPPFLAGS += -DISS_SR_3
CFLAGS += -DISS_SR_3
CXXFLAGS += -DISS_SR_3

all: Makefile.linux32.mk $(TARGET)
	cp $(TARGET) ../../../../Project/07.Release/linux_arm/bin/

clean:
	@echo -e '\e[1;34mClean... \e[0m'
	-@rm -rf $(TARGET) *.o *.gch *.d *.P ../../../../Project/07.Release/linux_arm/bin/$(TARGET)

rebuild:clean all

distclean:clean

.PHONY:all clean rebuild distclean

$(TARGET): $(OBJS) $(STATIC_LIBS)
	@echo -e '\e[1;34mGenerate $@...\e[0m'
	-@rm -rf $(TARGET)
	$(LD) $(LDFLAGS) $(OBJS) $(STATIC_LIBS) -o $@ $(SHARED_LIBS)

include ../../../Source/GNUMakeConfig/rules.mk
