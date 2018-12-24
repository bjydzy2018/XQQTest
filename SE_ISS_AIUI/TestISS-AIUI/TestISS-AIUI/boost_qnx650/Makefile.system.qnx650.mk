include ../../../../Source/GNUMakeConfig/defines.qnx.mk

TARGET := libboost_system.a

INCLUDE = -I../

SRCS_CXX += ../libs/system/src/error_code.cpp

STATIC_LIBS = 

SHARED_LIBS = 

all: $(TARGET)

clean:
	@echo -e '\e[1;34mClean... \e[0m'
	-@rm -r -f $(TARGET) $(OBJS)

rebuild:clean all


$(TARGET): clean $(OBJS)
	@echo -e '\e[1;34mGenerate $@...\e[0m'
	rm -r -f $(TARGET)
	$(AR) $(ARFLAGS) $@ $(OBJS)

include ../../../../Source/GNUMakeConfig/rules.mk

