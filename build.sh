#!/bin/bash

# Default build script
# Usage: ./build.sh [Debug|Release] [--tests]

BUILD_TYPE=${1:-Debug}
BUILD_TESTS=OFF

# Check if --tests flag is provided
if [[ "$2" == "--tests" ]]; then
    BUILD_TESTS=ON
fi

echo "Building in ${BUILD_TYPE} mode with tests=${BUILD_TESTS}..."

cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DBUILD_TESTS=${BUILD_TESTS} \
    && cmake --build build -j$(nproc)
