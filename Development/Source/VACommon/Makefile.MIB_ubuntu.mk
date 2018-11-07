include ../GNUMakeConfig/defines.MIB_ubuntu.mk

TARGET := libVACommon.a

INCLUDE = -I../rootfs/usr/include -I./

SRCS_CXX = $(wildcard $ *.cpp $ ./Path/*.cpp)

STATIC_LIBS = 

SHARED_LIBS = 

PCH_CXX_H = VAInc.h

all:SVN_REVISION $(PCH_CXX) Makefile.MIB_ubuntu.mk $(TARGET)

clean:
	@echo -e '\e[1;34mClean... \e[0m'
	-@rm -rf $(TARGET) *.o *.gch *.d *.P ./Path/*.o ./Path/*.P

rebuild:clean all

.PHONY:all clean rebuild

$(TARGET): $(OBJS)
	@echo -e '\e[1;34mGenerate $@...\e[0m'
	-@rm -rf $(TARGET)
#	$(LD) -shared $(LDFLAGS) -o $@ $(OBJS)
	@$(AR) -crfs $@ $(OBJS)

SVN_REVISION:
ifeq ($(wildcard svn_revision.h),)
#	@echo -e 'then...'
	-svnwcrev . ./svn_revision.txt ./svn_revision.h 
else
#	@echo -e 'else...'    
endif

include ../GNUMakeConfig/rules.mk
