include $(CLEAR_VARS)
LOCAL_MODULE := boost_system

LOCAL_C_INCLUDES += ../../

LOCAL_SRC_FILES += ../../libs/system/src/error_code.cpp

include $(BUILD_STATIC_LIBRARY)
