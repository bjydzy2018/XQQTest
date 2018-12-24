LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include $(LOCAL_PATH)/boost_filesystem.mk
include $(LOCAL_PATH)/boost_system.mk
include $(LOCAL_PATH)/boost_thread.mk

#LOCAL_PREBUILT_LIBS := boost_filesystem boost_system boost_thread