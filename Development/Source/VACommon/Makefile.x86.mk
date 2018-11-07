include ../GNUMakeConfig/defines.x86.mk
SRCS_C = $(wildcard *.c)
SRCS_C += $(wildcard $ ./mem_pool/*.c)
OBJS_C = $(patsubst %c,%o,$(SRCS_C))
SRCS_CXX = $(wildcard *.cpp)
OBJS_CXX = $(patsubst %cpp,%o,$(SRCS_CXX))

OBJS += $(patsubst %.c,%.o,$(wildcard ./Path/*.c)) $(patsubst %.cpp,%.o,$(wildcard ./Path/*.cpp))

TARGET := libVACommon.a

INCLUDE = 

STATIC_LIBS = 

SHARED_LIBS = 

PCH_CXX_H = VAInc.h

all:SVN_REVISION $(PCH_CXX) Makefile.x86.mk $(TARGET)

clean:
	@echo -e '\e[1;34mClean... \e[0m'
	-@rm -rf $(TARGET) svn_revision.h
	-@find ./ -name "*.o" | xargs rm -rf
	-@find ./ -name "*.gch" | xargs rm -rf
	-@find ./ -name "*.d" | xargs rm -rf
	-@find ./ -name "*.P" | xargs rm -rf


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
	-../GNUMakeConfig/svnwcrev/svnwcrev . ./svn_revision.txt ./svn_revision.h 
else
#	@echo -e 'else...'    
endif

include ../GNUMakeConfig/rules.mk
