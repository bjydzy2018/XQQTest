LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := gtest

LOCAL_SRC_FILES := ../../gtest_src/src/gtest-all.cc

LOCAL_C_INCLUDES := \
	../../gtest_src/ \
	../../

include $(BUILD_STATIC_LIBRARY)
