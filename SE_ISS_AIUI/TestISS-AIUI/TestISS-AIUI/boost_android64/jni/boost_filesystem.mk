include $(CLEAR_VARS)
LOCAL_MODULE := boost_filesystem

LOCAL_C_INCLUDES += ../../

LOCAL_SRC_FILES += \
	../../libs/filesystem/v3/src/operations.cpp \
	../../libs/filesystem/v3/src/path.cpp \
	../../libs/filesystem/v3/src/path_traits.cpp \
	../../libs/filesystem/v3/src/portability.cpp \
	../../libs/filesystem/v3/src/unique_path.cpp \
	../../libs/filesystem/v2/src/v2_operations.cpp \
	../../libs/filesystem/v2/src/v2_path.cpp

include $(BUILD_STATIC_LIBRARY)
