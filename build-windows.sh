#!/bin/bash

# Build script for cross-compiling to Windows using MinGW-w64
# Usage: ./build-windows.sh [Debug|Release] [ON|OFF]
#   First argument: Build type (Debug or Release, default: Release)
#   Second argument: Build tests (ON or OFF, default: OFF for cross-compilation)

set -e  # Exit on any error

BUILD_TYPE=${1:-Release}
BUILD_TESTS=${2:-OFF}
BUILD_DIR="build-windows"

echo "=============================================="
echo "Building for Windows (x86_64)"
echo "Build type: $BUILD_TYPE"
echo "Build tests: $BUILD_TESTS"
echo "=============================================="

# Clean previous build if it exists
if [ -d "$BUILD_DIR" ]; then
    echo "Cleaning previous Windows build..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory
mkdir -p "$BUILD_DIR"

# Configure with CMake using the Windows toolchain
echo "Configuring with CMake..."
cmake -S . -B "$BUILD_DIR" \
    -DCMAKE_TOOLCHAIN_FILE=cmake/windows-cross.cmake \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DBUILD_TESTS="$BUILD_TESTS"

# Build the project
echo "Building..."
cmake --build "$BUILD_DIR" --parallel $(nproc)

# Copy files to distribution directory
echo "Copying files to distribution directory..."
./scripts/copy-dist.sh "$BUILD_DIR" windows x86_64

echo "=============================================="
echo "Windows build completed successfully!"
echo "Output directory: dist/lib/windows/x86_64"
echo "=============================================="

# List the generated files
echo "Generated Windows binaries:"
find dist/lib/windows/x86_64 -name "*.dll" -o -name "*.a" 2>/dev/null | sort || echo "No files found (check build output above)"