include ../../../Source/GNUMakeConfig/defines.arm.mk

TARGET := TestISS-AIUI_linuxarm32

INCLUDE = -I../../../Source/VACommon \
	-I../../../Source/ISS_API \
	-I . \
	-I ./gtest \
	-I ./boost

STATIC_LIBS = ../../../Source/VACommon/libVACommon.a \
	gtest_linuxarm32/gtest.a

SHARED_LIBS = -L../../../../Project/07.Release/linux.arm/bin/ \
	-lpthread \
	-lisstts \
	-lisscata -lm -lcata -lcataIndex \
	-lisssr -lissauth -lmsc -lrt -liFlyNLI -liFlyPResBuild -lSpWord -lAitalkOne -lnamext \
	-lissmvw -livw -livwgram

all: Makefile.arm.mk makeCommon $(TARGET)
	cp $(TARGET) ../../../../Project/07.Release/linux.arm/bin/

clean:
	@echo -e '\e[1;34mClean... \e[0m'
	-@rm -rf $(TARGET) *.o *.gch *.d *.P ../../../../Project/07.Release/linux.arm/bin/lib/$(TARGET)
	-@rm -rf ../../../../Project/07.Release/linux.arm/bin/$(TARGET)

rebuild:clean all

distclean:clean
	@$(MAKE) -C ../../../Source/VACommon -f Makefile.arm.mk clean

.PHONY:all clean rebuild distclean

$(TARGET): $(OBJS) $(STATIC_LIBS)
	@echo -e '\e[1;34mGenerate $@...\e[0m'
	-@rm -rf $(TARGET)
	$(LD) $(LDFLAGS) $(OBJS) $(STATIC_LIBS) -o $@ $(SHARED_LIBS)

makeCommon:
	@$(MAKE) -C ../../../Source/VACommon -f Makefile.arm.mk


include ../../../Source/GNUMakeConfig/rules.mk
