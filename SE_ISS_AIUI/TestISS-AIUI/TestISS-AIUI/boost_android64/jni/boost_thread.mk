include $(CLEAR_VARS)
LOCAL_MODULE := boost_thread

LOCAL_C_INCLUDES += ../../

LOCAL_SRC_FILES += \
	../../libs/thread/src/pthread/once.cpp \
	../../libs/thread/src/pthread/thread.cpp

include $(BUILD_STATIC_LIBRARY)
