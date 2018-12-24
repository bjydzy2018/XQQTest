LOCAL_PATH := $(call my-dir)
LIB_FILE_PATH := $(LOCAL_PATH)/../../../../../Project/07.Release/android/lib_NoEncryption/armeabi-v7a

include $(CLEAR_VARS)
LOCAL_MODULE    := vacommon_prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/../vacommon_android/libVACommon.a
include $(PREBUILT_STATIC_LIBRARY)

# libaiui.so
include $(CLEAR_VARS)
LOCAL_MODULE := aiui-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libaiui.so
include $(PREBUILT_SHARED_LIBRARY)

# libcata.so
include $(CLEAR_VARS)
LOCAL_MODULE := cata-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libcata.so
include $(PREBUILT_SHARED_LIBRARY)

# libcataIndex.so
include $(CLEAR_VARS)
LOCAL_MODULE := cataIndex-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libcataIndex.so
include $(PREBUILT_SHARED_LIBRARY)

# libcata-jni.so
include $(CLEAR_VARS)
LOCAL_MODULE := cata-jni-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libcata-jni.so
include $(PREBUILT_SHARED_LIBRARY)

# libiau-jni.so
include $(CLEAR_VARS)
LOCAL_MODULE := au-jni-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libiau-jni.so
include $(PREBUILT_SHARED_LIBRARY)

# libichip-jni.so
include $(CLEAR_VARS)
LOCAL_MODULE := chip-jni-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libichip-jni.so
include $(PREBUILT_SHARED_LIBRARY)

# libiFlyNLI.so
include $(CLEAR_VARS)
LOCAL_MODULE := iFlyNLI-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libiFlyNLI.so
include $(PREBUILT_SHARED_LIBRARY)

# libiFlyPResBuild.so
include $(CLEAR_VARS)
LOCAL_MODULE := iFlyPResBuild-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libiFlyPResBuild.so
include $(PREBUILT_SHARED_LIBRARY)

# libimvw-jni.so
include $(CLEAR_VARS)
LOCAL_MODULE := imvw-jni-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libimvw-jni.so
include $(PREBUILT_SHARED_LIBRARY)

# libisr-jni.so
include $(CLEAR_VARS)
LOCAL_MODULE := isr-jni-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libisr-jni.so
include $(PREBUILT_SHARED_LIBRARY)

# libissauth.so
include $(CLEAR_VARS)
LOCAL_MODULE := issauth-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libissauth.so
include $(PREBUILT_SHARED_LIBRARY)

# libitts-jni.so
include $(CLEAR_VARS)
LOCAL_MODULE := itts-jni-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libitts-jni.so
include $(PREBUILT_SHARED_LIBRARY)

# libkernel.so
include $(CLEAR_VARS)
LOCAL_MODULE := kernel-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libkernel.so
include $(PREBUILT_SHARED_LIBRARY)


# libnamext.so
include $(CLEAR_VARS)
LOCAL_MODULE := namext-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libnamext.so
include $(PREBUILT_SHARED_LIBRARY)

# libSearchForNli.so
include $(CLEAR_VARS)
LOCAL_MODULE := SearchForNli-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libSearchForNli.so
include $(PREBUILT_SHARED_LIBRARY)

# libseopt-jni.so
include $(CLEAR_VARS)
LOCAL_MODULE := seopt-jni-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libseopt-jni.so
include $(PREBUILT_SHARED_LIBRARY)

# libSpWord.so
include $(CLEAR_VARS)
LOCAL_MODULE := SpWord-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libSpWord.so
include $(PREBUILT_SHARED_LIBRARY)

# libttpy-jni.so
include $(CLEAR_VARS)
LOCAL_MODULE := ttpy-jni-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libttpy-jni.so
include $(PREBUILT_SHARED_LIBRARY)

# libw_ivw.so
include $(CLEAR_VARS)
LOCAL_MODULE := w_ivw-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libw_ivw.so
include $(PREBUILT_SHARED_LIBRARY)

# libw_ivwgram.so
include $(CLEAR_VARS)
LOCAL_MODULE := w_ivwgram-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libw_ivwgram.so
include $(PREBUILT_SHARED_LIBRARY)

# gtest
include $(CLEAR_VARS)
LOCAL_MODULE    := gtest_prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/../gtest_android/libgtest.a
include $(PREBUILT_STATIC_LIBRARY)

# chrono
include $(CLEAR_VARS)
LOCAL_MODULE    := chrono_prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/../boost_android/libboost_chrono-gcc-mt-s-1_49.a
include $(PREBUILT_STATIC_LIBRARY)

# date_time
include $(CLEAR_VARS)
LOCAL_MODULE    := date_time_prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/../boost_android/libboost_date_time-gcc-mt-s-1_49.a
include $(PREBUILT_STATIC_LIBRARY)

# filesystem
include $(CLEAR_VARS)
LOCAL_MODULE    := filesystem_prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/../boost_android/libboost_filesystem-gcc-mt-s-1_49.a
include $(PREBUILT_STATIC_LIBRARY)

# random
include $(CLEAR_VARS)
LOCAL_MODULE    := random_prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/../boost_android/libboost_random-gcc-mt-s-1_49.a
include $(PREBUILT_STATIC_LIBRARY)

# regex
include $(CLEAR_VARS)
LOCAL_MODULE    := regex_prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/../boost_android/libboost_regex-gcc-mt-s-1_49.a
include $(PREBUILT_STATIC_LIBRARY)

# system
include $(CLEAR_VARS)
LOCAL_MODULE    := system_prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/../boost_android/libboost_system-gcc-mt-s-1_49.a
include $(PREBUILT_STATIC_LIBRARY)

# thread
include $(CLEAR_VARS)
LOCAL_MODULE    := thread_prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/../boost_android/libboost_thread-gcc-mt-s-1_49.a
include $(PREBUILT_STATIC_LIBRARY)



# 编译apk依赖的so版本
include $(CLEAR_VARS)
LOCAL_MODULE := AiuiToolMainFunc
LOCAL_CFLAGS += -fPIE -DAiuiToolMainFuncMacro
LOCAL_LDFLAGS += -fPIE
LOCAL_LDLIBS += -llog -lc -lm

LOCAL_CFLAGS += -DISS_AIUI

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../Source/libSearchForNli \
$(LOCAL_PATH)/../../../../Source/VACommon \
$(LOCAL_PATH)/../../../../Source/VACommon/log \
$(LOCAL_PATH)/../../../../Source/ISS_API \
$(LOCAL_PATH)/.. \
$(LOCAL_PATH)/../gtest

LOCAL_SRC_FILES := $(wildcard ../*.cpp)
LOCAL_SRC_FILES += com_iflytek_nativetool_AiuiToolMainEntry.cpp

LOCAL_STATIC_LIBRARIES := vacommon_prebuilt \
gtest_prebuilt \
system_prebuilt \
chrono_prebuilt \
date_time_prebuilt \
filesystem_prebuilt \
random_prebuilt \
regex_prebuilt \
thread_prebuilt

LOCAL_SHARED_LIBRARIES := aiui-prebuilt \
cata-prebuilt \
cataIndex-prebuilt \
cata-jni-prebuilt \
au-jni-prebuilt \
chip-jni-prebuilt \
iFlyNLI-prebuilt \
iFlyPResBuild-prebuilt \
imvw-jni-prebuilt \
isr-jni-prebuilt \
issauth-prebuilt \
itts-jni-prebuilt \
kernel-prebuilt \
namext-prebuilt \
SearchForNli-prebuilt \
seopt-jni-prebuilt \
SpWord-prebuilt \
ttpy-jni-prebuilt \
w_ivw-prebuilt \
w_ivwgram-prebuilt


include $(BUILD_SHARED_LIBRARY)


# TestISS-AIUI
include $(CLEAR_VARS)
LOCAL_MODULE := TestISS-AIUI
LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
LOCAL_LDLIBS += -ldl -llog -lc -lm

LOCAL_CFLAGS += -DISS_AIUI

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../Source/libSearchForNli \
$(LOCAL_PATH)/../../../../Source/VACommon \
$(LOCAL_PATH)/../../../../Source/VACommon/log \
$(LOCAL_PATH)/../../../../Source/ISS_API \
$(LOCAL_PATH)/.. \
$(LOCAL_PATH)/../gtest

LOCAL_SRC_FILES := $(wildcard ../*.cpp)

LOCAL_STATIC_LIBRARIES := vacommon_prebuilt \
gtest_prebuilt \
system_prebuilt \
chrono_prebuilt \
date_time_prebuilt \
filesystem_prebuilt \
random_prebuilt \
regex_prebuilt \
thread_prebuilt

LOCAL_SHARED_LIBRARIES := aiui-prebuilt \
cata-prebuilt \
cataIndex-prebuilt \
cata-jni-prebuilt \
au-jni-prebuilt \
chip-jni-prebuilt \
iFlyNLI-prebuilt \
iFlyPResBuild-prebuilt \
imvw-jni-prebuilt \
isr-jni-prebuilt \
issauth-prebuilt \
itts-jni-prebuilt \
kernel-prebuilt \
namext-prebuilt \
SearchForNli-prebuilt \
seopt-jni-prebuilt \
SpWord-prebuilt \
ttpy-jni-prebuilt \
w_ivw-prebuilt \
w_ivwgram-prebuilt


include $(BUILD_EXECUTABLE)



# 编译apk依赖的调用Java-Native层接口的so
include $(CLEAR_VARS)
LOCAL_MODULE := AiuiToolMainFuncCallJavaNativeApi
LOCAL_CFLAGS += -fPIE -DAiuiToolMainFuncCallJavaNativeApiMacro
LOCAL_LDFLAGS += -fPIE
LOCAL_LDLIBS += -llog -lc -lm

LOCAL_CFLAGS += -DISS_AIUI

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../Source/libSearchForNli \
$(LOCAL_PATH)/../../../../Source/VACommon \
$(LOCAL_PATH)/../../../../Source/VACommon/log \
$(LOCAL_PATH)/../../../../Source/ISS_API \
$(LOCAL_PATH)/.. \
$(LOCAL_PATH)/../gtest

LOCAL_SRC_FILES := $(wildcard ../*.cpp)
LOCAL_SRC_FILES += com_iflytek_nativetool_AiuiToolMainEntryCallJavaNativeApi.cpp

LOCAL_STATIC_LIBRARIES := vacommon_prebuilt \
gtest_prebuilt \
system_prebuilt \
chrono_prebuilt \
date_time_prebuilt \
filesystem_prebuilt \
random_prebuilt \
regex_prebuilt \
thread_prebuilt

LOCAL_SHARED_LIBRARIES := aiui-prebuilt \
cata-prebuilt \
cataIndex-prebuilt \
cata-jni-prebuilt \
au-jni-prebuilt \
chip-jni-prebuilt \
iFlyNLI-prebuilt \
iFlyPResBuild-prebuilt \
imvw-jni-prebuilt \
isr-jni-prebuilt \
issauth-prebuilt \
itts-jni-prebuilt \
kernel-prebuilt \
namext-prebuilt \
SearchForNli-prebuilt \
seopt-jni-prebuilt \
SpWord-prebuilt \
ttpy-jni-prebuilt \
w_ivw-prebuilt \
w_ivwgram-prebuilt


include $(BUILD_SHARED_LIBRARY)