#!/bin/bash

# Build script for cross-compiling to Android using Android NDK
# Usage: ./build-android.sh [Debug|Release] [arch1,arch2,...]
#   First argument: Build type (Debug or Release, default: Release)
#   Second argument: Comma-separated list of architectures (default: all)
#                   Available: arm64-v8a,armeabi-v7a,x86_64,x86
#
# Prerequisites:
#   - Android NDK must be installed
#   - ANDROID_NDK_ROOT environment variable must be set

set -e  # Exit on any error

BUILD_TYPE=${1:-Release}
ARCHITECTURES=${2:-"arm64-v8a,armeabi-v7a,x86_64,x86"}

echo "=============================================="
echo "Building for Android"
echo "Build type: $BUILD_TYPE"
echo "Architectures: $ARCHITECTURES"
echo "=============================================="

# Check if Android NDK is available
if [ -z "$ANDROID_NDK_ROOT" ]; then
    echo "Error: ANDROID_NDK_ROOT environment variable is not set."
    echo "Please install Android NDK and set ANDROID_NDK_ROOT to the installation directory."
    echo ""
    echo "Download Android NDK from:"
    echo "https://developer.android.com/ndk/downloads"
    echo ""
    echo "Example setup:"
    echo "export ANDROID_NDK_ROOT=\"/path/to/android-ndk-r26b\""
    exit 1
fi

if [ ! -d "$ANDROID_NDK_ROOT" ]; then
    echo "Error: Android NDK directory not found at $ANDROID_NDK_ROOT"
    echo "Please check your ANDROID_NDK_ROOT environment variable."
    exit 1
fi

echo "Using Android NDK from: $ANDROID_NDK_ROOT"

# Convert comma-separated list to array
IFS=',' read -ra ARCH_ARRAY <<< "$ARCHITECTURES"

# Build for each architecture
for ARCH in "${ARCH_ARRAY[@]}"; do
    # Trim whitespace
    ARCH=$(echo "$ARCH" | tr -d ' ')

    echo ""
    echo "=============================================="
    echo "Building for Android $ARCH"
    echo "=============================================="

    BUILD_DIR="build-android-$ARCH"

    # Clean previous build if it exists
    if [ -d "$BUILD_DIR" ]; then
        echo "Cleaning previous Android $ARCH build..."
        rm -rf "$BUILD_DIR"
    fi

    # Create build directory
    mkdir -p "$BUILD_DIR"

    # Check if toolchain file exists
    TOOLCHAIN_FILE="cmake/android-$ARCH.cmake"
    if [ ! -f "$TOOLCHAIN_FILE" ]; then
        echo "Error: Toolchain file not found: $TOOLCHAIN_FILE"
        echo "Supported architectures: arm64-v8a, armeabi-v7a, x86_64, x86"
        exit 1
    fi

    # Configure with CMake using the Android toolchain
    echo "Configuring with CMake..."
    cmake -S . -B "$BUILD_DIR" \
        -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DBUILD_TESTS=OFF

    # Build the project
    echo "Building..."
    cmake --build "$BUILD_DIR" --parallel $(nproc)

    # Copy files to distribution directory
    echo "Copying files to distribution directory..."
    ./scripts/copy-dist.sh "$BUILD_DIR" android "$ARCH"

    echo "Android $ARCH build completed successfully!"
    echo "Output directory: dist/lib/android/$ARCH"
done

echo ""
echo "=============================================="
echo "All Android builds completed successfully!"
echo "=============================================="

# List all generated files
echo "Generated Android binaries:"
for ARCH in "${ARCH_ARRAY[@]}"; do
    ARCH=$(echo "$ARCH" | tr -d ' ')
    DIST_DIR="dist/lib/android/$ARCH"
    if [ -d "$DIST_DIR" ]; then
        echo ""
        echo "$ARCH:"
        find "$DIST_DIR" -name "*.so" -o -name "*.a" 2>/dev/null | sort | sed 's/^/  /' || echo "  No files found"
    fi
done