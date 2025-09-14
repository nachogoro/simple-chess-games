# CMake toolchain file for Android x86_64 (64-bit Intel)
# Requires Android NDK to be installed and ANDROID_NDK_ROOT environment variable set

if(NOT DEFINED ENV{ANDROID_NDK_ROOT})
    message(FATAL_ERROR "ANDROID_NDK_ROOT environment variable must be set to the Android NDK installation directory")
endif()

set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_SYSTEM_VERSION 21) # Android API level 21 (Android 5.0)
set(CMAKE_ANDROID_ARCH_ABI x86_64)
set(CMAKE_ANDROID_NDK $ENV{ANDROID_NDK_ROOT})
set(CMAKE_ANDROID_STL_TYPE c++_static)

# Disable position independent code for Android
set(CMAKE_POSITION_INDEPENDENT_CODE OFF)