#!/bin/bash

# Build script for Linux native compilation
# Usage: ./build-linux.sh [Debug|Release] [ON|OFF]
#   First argument: Build type (Debug or Release, default: Release)
#   Second argument: Build tests (ON or OFF, default: ON for native builds)

set -e  # Exit on any error

BUILD_TYPE=${1:-Release}
BUILD_TESTS=${2:-ON}
BUILD_DIR="build-linux"

echo "=============================================="
echo "Building for Linux (x86_64)"
echo "Build type: $BUILD_TYPE"
echo "Build tests: $BUILD_TESTS"
echo "=============================================="

# Clean previous build if it exists
if [ -d "$BUILD_DIR" ]; then
    echo "Cleaning previous Linux build..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory
mkdir -p "$BUILD_DIR"

# Configure with CMake
echo "Configuring with CMake..."
cmake -S . -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DBUILD_TESTS="$BUILD_TESTS"

# Build the project
echo "Building..."
cmake --build "$BUILD_DIR" --parallel $(nproc)

# Copy files to distribution directory
echo "Copying files to distribution directory..."
./scripts/copy-dist.sh "$BUILD_DIR" linux x86_64

# Run tests if enabled
if [ "$BUILD_TESTS" = "ON" ]; then
    echo ""
    echo "=============================================="
    echo "Running tests..."
    echo "=============================================="

    # Run C++ tests
    if [ -f "$BUILD_DIR/run_cpp_tests" ]; then
        echo "Running C++ tests..."
        cd "$BUILD_DIR"
        LD_LIBRARY_PATH=. ./run_cpp_tests
        cd ..
    fi

    # Run C tests
    if [ -f "$BUILD_DIR/run_c_tests" ]; then
        echo "Running C tests..."
        cd "$BUILD_DIR"
        LD_LIBRARY_PATH=. ./run_c_tests
        cd ..
    fi
fi

echo ""
echo "=============================================="
echo "Linux build completed successfully!"
echo "Output directory: dist/lib/linux/x86_64"
echo "=============================================="

# List the generated files
echo "Generated Linux binaries:"
find dist/lib/linux/x86_64 -name "*.so" -o -name "*.a" 2>/dev/null | sort || echo "No files found (check build output above)"