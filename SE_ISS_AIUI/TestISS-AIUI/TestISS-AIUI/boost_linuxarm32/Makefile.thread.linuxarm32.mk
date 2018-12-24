include ../../../../Source/GNUMakeConfig/defines.arm.mk

TARGET := libboost_thread.a

INCLUDE = -I../

SRCS_CXX += \
../libs/thread/src/pthread/once.cpp \
../libs/thread/src/pthread/thread.cpp

STATIC_LIBS = 

SHARED_LIBS = 

all: $(TARGET)

clean:
	@echo -e '\e[1;34mClean... \e[0m'
	-@rm -rf $(TARGET) $(OBJS)

rebuild:clean all

.PHONY:all clean rebuild

$(TARGET): clean $(OBJS)
	@echo -e '\e[1;34mGenerate $@...\e[0m'
	rm -rf $(TARGET)
	$(AR) -crfs $@ $(OBJS)

include ../../../../Source/GNUMakeConfig/rules.mk
