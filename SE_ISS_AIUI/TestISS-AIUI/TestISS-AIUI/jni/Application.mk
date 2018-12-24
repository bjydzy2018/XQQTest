# Build both ARMv5TE and ARMv7-A machine code.
NDK_TOOLCHAIN_VERSION=4.9
APP_PLATFORM := android-19
APP_OPTIM := release
APP_ABI := armeabi-v7a
APP_STL := gnustl_static
APP_GNUSTL_FORCE_CPP_FEATURES := exceptions rtti
APP_CFLAGS += -fdiagnostics-color=auto -fdiagnostics-color=always -DANDROID