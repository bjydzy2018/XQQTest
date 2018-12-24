include ../../../../Source/GNUMakeConfig/defines.drc4.mk

TARGET := libboost_system.a

INCLUDE = -I../

SRCS_CXX += ../libs/system/src/error_code.cpp

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
