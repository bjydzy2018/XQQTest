include ../../../../Source/GNUMakeConfig/defines.arm.mk

TARGET := libboost_filesystem.a

INCLUDE = -I../

SRCS_CXX += \
../libs/filesystem/v3/src/operations.cpp \
../libs/filesystem/v3/src/path.cpp \
../libs/filesystem/v3/src/path_traits.cpp \
../libs/filesystem/v3/src/portability.cpp \
../libs/filesystem/v3/src/unique_path.cpp \
../libs/filesystem/v2/src/v2_operations.cpp \
../libs/filesystem/v2/src/v2_path.cpp

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