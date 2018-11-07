.PHONY:all clean rebuild

include ../GNUMakeConfig/defines.qnx.mk
 
TARGET := libVACommon.a

INCLUDE += -I./

SRCS_C += $(wildcard $ ./mem_pool/*.c)
SRCS_CXX = $(wildcard $ *.cpp $ ./Path/*.cpp)


STATIC_LIBS = 

SHARED_LIBS = 

PCH_CXX_H = VAInc.h

all:SVN_REVISION $(PCH_CXX) Makefile.qnx.mk $(TARGET)

clean:
	@echo -e '\e[1;34mClean... \e[0m'
	-@rm -f $(TARGET) *.o *.gch *.d *.P ./Path/*.o ./Path/*.P ../components/*.o ../components/*.P  ./mem_pool/*.o ./mem_pool/*.P

rebuild:clean all

$(TARGET): $(OBJS)
	@echo -e '\e[1;34mGenerate $@...\e[0m'
	-@rm -r -f $(TARGET)
#	$(LD) -shared $(LDFLAGS) -o $@ $(OBJS)
	@$(AR) $(ARFLAGS) $@ $(OBJS)

SVN_REVISION:
ifeq ($(wildcard svn_revision.h),)
#	@echo -e 'then...'
	-subwcrev . ./svn_revision.txt ./svn_revision.h 
else
#	@echo -e 'else...'    
endif

include ../GNUMakeConfig/rules.mk
