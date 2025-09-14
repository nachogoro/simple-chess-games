#!/bin/bash

# Script to copy only essential files to distribution directory
# Usage: ./scripts/copy-dist.sh <build_dir> <platform> <architecture>
# Example: ./scripts/copy-dist.sh build-linux linux x86_64

set -e

BUILD_DIR="$1"
PLATFORM="$2"
ARCH="$3"

if [ -z "$BUILD_DIR" ] || [ -z "$PLATFORM" ] || [ -z "$ARCH" ]; then
    echo "Usage: $0 <build_dir> <platform> <architecture>"
    echo "Example: $0 build-linux linux x86_64"
    exit 1
fi

# Define target directories
DIST_ROOT="dist"
LIB_DIR="$DIST_ROOT/lib/$PLATFORM/$ARCH"
INCLUDE_DIR="$DIST_ROOT/include"

echo "Copying distribution files..."
echo "Build dir: $BUILD_DIR"
echo "Platform: $PLATFORM"
echo "Architecture: $ARCH"

# Create directories
mkdir -p "$LIB_DIR"
mkdir -p "$INCLUDE_DIR"

# Copy headers only once (shared across all platforms/architectures)
if [ ! -d "$INCLUDE_DIR/cpp" ]; then
    echo "Copying C++ headers..."
    cp -r include/cpp "$INCLUDE_DIR/"
fi

if [ ! -d "$INCLUDE_DIR/c" ]; then
    echo "Copying C headers..."
    cp -r include/c "$INCLUDE_DIR/"
fi

# Copy only our libraries (filter out test libraries)
echo "Copying libraries..."

case "$PLATFORM" in
    "linux")
        # Copy shared libraries
        find "$BUILD_DIR" -maxdepth 1 -name "libsimple-chess-games.so*" -exec cp {} "$LIB_DIR/" \;
        find "$BUILD_DIR" -maxdepth 1 -name "libsimple-chess-games-c.so*" -exec cp {} "$LIB_DIR/" \;

        # Copy static libraries (only our libraries, not test ones)
        find "$BUILD_DIR" -maxdepth 1 -name "libsimple-chess-games-static.a" -exec cp {} "$LIB_DIR/" \;
        find "$BUILD_DIR" -maxdepth 1 -name "libsimple-chess-games-c-static.a" -exec cp {} "$LIB_DIR/" \;
        ;;

    "windows")
        # Copy DLL files
        find "$BUILD_DIR" -maxdepth 1 -name "libsimple-chess-games.dll" -exec cp {} "$LIB_DIR/" \;
        find "$BUILD_DIR" -maxdepth 1 -name "libsimple-chess-games-c.dll" -exec cp {} "$LIB_DIR/" \;

        # Copy import libraries
        find "$BUILD_DIR" -maxdepth 1 -name "libsimple-chess-games.dll.a" -exec cp {} "$LIB_DIR/" \;
        find "$BUILD_DIR" -maxdepth 1 -name "libsimple-chess-games-c.dll.a" -exec cp {} "$LIB_DIR/" \;

        # Copy static libraries
        find "$BUILD_DIR" -maxdepth 1 -name "libsimple-chess-games-static.a" -exec cp {} "$LIB_DIR/" \;
        find "$BUILD_DIR" -maxdepth 1 -name "libsimple-chess-games-c-static.a" -exec cp {} "$LIB_DIR/" \;
        ;;

    "android")
        # Copy shared libraries
        find "$BUILD_DIR" -maxdepth 1 -name "libsimple-chess-games.so" -exec cp {} "$LIB_DIR/" \;
        find "$BUILD_DIR" -maxdepth 1 -name "libsimple-chess-games-c.so" -exec cp {} "$LIB_DIR/" \;

        # Copy static libraries
        find "$BUILD_DIR" -maxdepth 1 -name "libsimple-chess-games-static.a" -exec cp {} "$LIB_DIR/" \;
        find "$BUILD_DIR" -maxdepth 1 -name "libsimple-chess-games-c-static.a" -exec cp {} "$LIB_DIR/" \;
        ;;

    *)
        echo "Unknown platform: $PLATFORM"
        exit 1
        ;;
esac

# Verify we copied files
if [ "$(find "$LIB_DIR" -type f | wc -l)" -eq 0 ]; then
    echo "Warning: No library files were copied to $LIB_DIR"
else
    echo "Successfully copied libraries to $LIB_DIR"
    echo "Files:"
    find "$LIB_DIR" -type f | sort | sed 's/^/  /'
fi