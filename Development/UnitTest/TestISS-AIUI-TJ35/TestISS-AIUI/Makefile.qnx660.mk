include ../../../Source/GNUMakeConfig/defines.qnx.mk

TARGET := TestISS-AIUI-qnx660

INCLUDE += -I../TestISS-AIUI/ -I../../../Source/VACommon/ \
-I../../../Source/VACommon/log/ \
-I../../../Source/ISS_API/ \
-I./gtest/

STATIC_LIBS = ./vacommon_qnx660/libVACommon.a \
./gtest_qnx660/gtest.a

SHARED_LIBS = -pthread -lm \
-L../../../../Project/07.Release/qnx_arm/bin \
-lAitalkOne -lcata -lcataIndex -liFlyNLI -liFlyPResBuild -lissauth  \
-lisscata -lissmvw -lisssr -lisstts -livw -livwgram -lmsc -lnamext -lSpWord

CPPFLAGS += -DISS_SR_3
CFLAGS += -DISS_SR_3
CXXFLAGS += -DISS_SR_3

all: Makefile.qnx660.mk $(TARGET)
	cp $(TARGET) ../../../../Project/07.Release/qnx_arm/bin/

clean:
	@echo -e '\e[1;34mClean... \e[0m'
	-@rm -rf $(TARGET) *.o *.gch *.d *.P ../../../../Project/07.Release/qnx_arm/bin/$(TARGET)

rebuild:clean all

distclean:clean

.PHONY:all clean rebuild distclean

$(TARGET): $(OBJS) $(STATIC_LIBS)
	@echo -e '\e[1;34mGenerate $@...\e[0m'
	-@rm -rf $(TARGET)
	$(LD) $(LDFLAGS) $(OBJS) $(STATIC_LIBS) -o $@ $(SHARED_LIBS)

include ../../../Source/GNUMakeConfig/rules.mk
