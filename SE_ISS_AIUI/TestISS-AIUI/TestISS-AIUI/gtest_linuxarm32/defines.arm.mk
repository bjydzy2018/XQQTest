#PREFIX = arm-none-linux-gnueabi-
PREFIX = arm-linux-gnueabihf-
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++

AR = $(PREFIX)ar
LD = $(PREFIX)g++
RANLIB = $(PREFIX)ranlib
STRIP = $(PREFIX)strip

RM = rm
LN = ln

TARGET_CFLAGS := \
    -fpic \
    -ffunction-sections \
    -funwind-tables \
    -fstack-protector \
    -no-canonical-prefixes

TARGET_LDFLAGS := -no-canonical-prefixes
#armeabi-v7a
TARGET_CFLAGS_v7a = $(TARGET_CFLAGS) -march=armv7-a -mfpu=vfpv3-d16 -mfloat-abi=softfp -Wl,--fix-cortex-a8
TARGET_LDFLAGS_v7a = $(TARGET_LDFLAGS) -march=armv7-a -Wl,--fix-cortex-a8
#armeabi-v7a-hard
TARGET_CFLAGS_v7a-hard = $(TARGET_CFLAGS) -march=armv7-a -mfpu=vfpv3-d16 -mhard-float -D_NDK_MATH_NO_SOFTFP=1
TARGET_LDFLAGS_v7a-hard = $(TARGET_LDFLAGS) -march=armv7-a -Wl,--fix-cortex-a8 -Wl,--no-warn-mismatch 
#armeabi-v5te
TARGET_CFLAGS_v5te = $(TARGET_CFLAGS) -march=armv5te -mtune=xscale -msoft-float
TARGET_LDFLAGS_v5te = $(TARGET_LDFLAGS) 

#armeabi-v7a neon
TARGET_CFLAGS_v7a_neon = $(TARGET_CFLAGS) -march=armv7-a -mfpu=neon -mfloat-abi=softfp -Wl,--fix-cortex-a8
#armeabi-v7a-hard neon
TARGET_CFLAGS_v7a-hard_neon = $(TARGET_CFLAGS) -march=armv7-a -mfpu=neon -mhard-float -D_NDK_MATH_NO_SOFTFP=1

TARGET_arm_release_CFLAGS :=  -O2 \
                              -g \
                              -DNDEBUG \
                              -fomit-frame-pointer \
                              -fstrict-aliasing    \
                              -funswitch-loops     \
                              -finline-limit=300

TARGET_thumb_release_CFLAGS := -mthumb \
                               -Os \
                               -g \
                               -DNDEBUG \
                               -fomit-frame-pointer \
                               -fno-strict-aliasing \
                               -finline-limit=64

TARGET_arm_debug_CFLAGS := $(TARGET_arm_release_CFLAGS) \
                           -O0 \
                           -UNDEBUG \
                           -fno-omit-frame-pointer \
                           -fno-strict-aliasing

TARGET_thumb_debug_CFLAGS := $(TARGET_thumb_release_CFLAGS) \
                             -O0 \
                             -UNDEBUG \
                             -marm \
                             -fno-omit-frame-pointer

#-DIFLYTEK_LOG_ISSSR -DIFLYTEK_LOG_ISSMVW -DIFLYTEK_LOG_ISSTTS
							 
#CPPFLAGS = $(INCLUDE) $(TARGET_CFLAGS_v7a_neon) $(TARGET_thumb_release_CFLAGS)
CPPFLAGS = $(INCLUDE) $(TARGET_CFLAGS_v7a-hard_neon) $(TARGET_thumb_release_CFLAGS)
CFLAGS = $(CPPFLAGS)
CXXFLAGS = $(CPPFLAGS) -fexceptions -frtti

#LDFLAGS = $(TARGET_LDFLAGS_v7a) -rdynamic -Wl,-R,'$$ORIGIN',--enable-new-dtags
LDFLAGS = $(TARGET_LDFLAGS_v7a-hard) -rdynamic -Wl,-R,'$$ORIGIN',--enable-new-dtags

SRCS_C = $(wildcard *.c)
OBJS_C = $(patsubst %c,%o,$(SRCS_C))
SRCS_CXX = $(wildcard *.cpp)
OBJS_CXX = $(patsubst %cpp,%o,$(SRCS_CXX))

SRCS = $(SRCS_C)
SRCS += $(SRCS_CXX)
OBJS = $(OBJS_C)
OBJS += $(OBJS_CXX)

MAKEDEPEND_C = $(CC) -MM $(CFLAGS) -o $*.d $<
MAKEDEPEND_CXX = $(CXX) -MM $(CXXFLAGS) -o $*.d $<

PCH=$(PCH_H).gch
PCH_CXX=$(PCH_CXX_H).gch
