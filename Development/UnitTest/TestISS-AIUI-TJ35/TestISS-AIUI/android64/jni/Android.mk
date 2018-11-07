LOCAL_PATH := $(call my-dir)
LIB_FILE_PATH := $(LOCAL_PATH)/../../../../../../Project/07.Release/android64/lib_HardEncryption/arm64-v8a

include $(CLEAR_VARS)
LOCAL_MODULE    := vacommon_prebuilt
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../vacommon_android64/libVACommon.a
include $(PREBUILT_STATIC_LIBRARY)

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

# libmsc.so
include $(CLEAR_VARS)
LOCAL_MODULE := msc-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libmsc.so
include $(PREBUILT_SHARED_LIBRARY)

# libnamext.so
include $(CLEAR_VARS)
LOCAL_MODULE := namext-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libnamext.so
include $(PREBUILT_SHARED_LIBRARY)

# libSpWord.so
include $(CLEAR_VARS)
LOCAL_MODULE := SpWord-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libSpWord.so
include $(PREBUILT_SHARED_LIBRARY)

# libspword-jni.so
include $(CLEAR_VARS)
LOCAL_MODULE := spword-jni-prebuilt
LOCAL_SRC_FILES := $(LIB_FILE_PATH)/libspword-jni.so
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
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../gtest_android64/libgtest.a
include $(PREBUILT_STATIC_LIBRARY)


# TestISS-AIUI
include $(CLEAR_VARS)
LOCAL_MODULE := TestISS-AIUI-android64

LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
LOCAL_LDLIBS += -ldl -llog -lc -lm

LOCAL_CFLAGS += -DISS_SR_3

LOCAL_STATIC_LIBRARIES := vacommon_prebuilt \
gtest_prebuilt

LOCAL_SHARED_LIBRARIES := cata-prebuilt \
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
msc-prebuilt \
namext-prebuilt \
SearchForNli-prebuilt \
SpWord-prebuilt \
spword-jni-prebuilt \
w_ivw-prebuilt \
w_ivwgram-prebuilt

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../../Source/VACommon \
$(LOCAL_PATH)/../../../../../Source/VACommon/log \
$(LOCAL_PATH)/../../../../../Source/ISS_API \
$(LOCAL_PATH)/../../ \
$(LOCAL_PATH)/../../gtest

LOCAL_SRC_FILES := $(wildcard ../../*.cpp)

include $(BUILD_EXECUTABLE)

