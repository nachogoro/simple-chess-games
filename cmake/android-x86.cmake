# CMake toolchain file for Android x86 (32-bit Intel)
# Requires Android NDK to be installed and ANDROID_NDK_ROOT environment variable set

if(NOT DEFINED ENV{ANDROID_NDK_ROOT})
    message(FATAL_ERROR "ANDROID_NDK_ROOT environment variable must be set to the Android NDK installation directory")
endif()

set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_SYSTEM_VERSION 21) # Android API level 21 (Android 5.0)
set(CMAKE_ANDROID_ARCH_ABI x86)
set(CMAKE_ANDROID_NDK $ENV{ANDROID_NDK_ROOT})
set(CMAKE_ANDROID_STL_TYPE c++_static)

# Disable position independent code for Android
set(CMAKE_POSITION_INDEPENDENT_CODE OFF)

# Android devices may use 16 KB memory pages, and the loader rejects shared libraries whose LOAD
# segments are aligned to less than the page size. Recent NDKs emit this alignment by default, but
# it is requested explicitly so the output does not depend on which NDK the build host provides.
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,max-page-size=16384")
