#!/bin/bash

# Unified build script for all supported platforms
# Usage: ./build-all.sh [Debug|Release] [platforms]
#   First argument: Build type (Debug or Release, default: Release)
#   Second argument: Comma-separated list of platforms (default: all)
#                   Available: linux,windows,android
#                   For android, you can specify specific architectures: android:arm64-v8a,armeabi-v7a
#
# Examples:
#   ./build-all.sh                                    # Build all platforms in Release mode
#   ./build-all.sh Debug                              # Build all platforms in Debug mode
#   ./build-all.sh Release linux,windows             # Build only Linux and Windows
#   ./build-all.sh Release android:arm64-v8a,x86_64  # Build only Android arm64-v8a and x86_64

set -e  # Exit on any error

BUILD_TYPE=${1:-Release}
PLATFORMS=${2:-"linux,windows,android"}

echo "=============================================="
echo "Building for Multiple Platforms"
echo "Build type: $BUILD_TYPE"
echo "Platforms: $PLATFORMS"
echo "=============================================="

# Clean dist directory
rm -fr dist/

# Convert comma-separated list to array
IFS=',' read -ra PLATFORM_ARRAY <<< "$PLATFORMS"

# Track build status
BUILD_SUCCESS=()
BUILD_FAILED=()

# Build for each platform
for PLATFORM_SPEC in "${PLATFORM_ARRAY[@]}"; do
    # Trim whitespace
    PLATFORM_SPEC=$(echo "$PLATFORM_SPEC" | tr -d ' ')

    echo ""
    echo "=============================================="

    # Parse platform specification
    if [[ "$PLATFORM_SPEC" == *":"* ]]; then
        # Platform with specific options (e.g., android:arm64-v8a,x86_64)
        PLATFORM=$(echo "$PLATFORM_SPEC" | cut -d':' -f1)
        OPTIONS=$(echo "$PLATFORM_SPEC" | cut -d':' -f2)
    else
        # Simple platform name
        PLATFORM="$PLATFORM_SPEC"
        OPTIONS=""
    fi

    echo "Building for $PLATFORM"
    if [ -n "$OPTIONS" ]; then
        echo "Options: $OPTIONS"
    fi
    echo "=============================================="

    case "$PLATFORM" in
        "linux")
            if ./build-linux.sh "$BUILD_TYPE"; then
                BUILD_SUCCESS+=("linux")
                echo "✓ Linux build completed successfully"
            else
                BUILD_FAILED+=("linux")
                echo "✗ Linux build failed"
            fi
            ;;

        "windows")
            if ./build-windows.sh "$BUILD_TYPE"; then
                BUILD_SUCCESS+=("windows")
                echo "✓ Windows build completed successfully"
            else
                BUILD_FAILED+=("windows")
                echo "✗ Windows build failed"
            fi
            ;;

        "android")
            if [ -n "$OPTIONS" ]; then
                # Build specific Android architectures
                if ./build-android.sh "$BUILD_TYPE" "$OPTIONS"; then
                    BUILD_SUCCESS+=("android:$OPTIONS")
                    echo "✓ Android build completed successfully"
                else
                    BUILD_FAILED+=("android:$OPTIONS")
                    echo "✗ Android build failed"
                fi
            else
                # Build all Android architectures
                if ./build-android.sh "$BUILD_TYPE"; then
                    BUILD_SUCCESS+=("android")
                    echo "✓ Android build completed successfully"
                else
                    BUILD_FAILED+=("android")
                    echo "✗ Android build failed"
                fi
            fi
            ;;

        *)
            echo "✗ Unknown platform: $PLATFORM"
            echo "Supported platforms: linux, windows, android"
            BUILD_FAILED+=("$PLATFORM (unknown)")
            ;;
    esac
done

echo ""
echo "=============================================="
echo "Build Summary"
echo "=============================================="

if [ ${#BUILD_SUCCESS[@]} -gt 0 ]; then
    echo "✓ Successful builds:"
    for platform in "${BUILD_SUCCESS[@]}"; do
        echo "  - $platform"
    done
fi

if [ ${#BUILD_FAILED[@]} -gt 0 ]; then
    echo ""
    echo "✗ Failed builds:"
    for platform in "${BUILD_FAILED[@]}"; do
        echo "  - $platform"
    done
fi

echo ""
echo "Distribution directory structure:"
if [ -d "dist" ]; then
    echo "Headers: dist/include/"
    echo "Libraries:"
    find dist/lib -type f \( -name "*.so" -o -name "*.dll" -o -name "*.a" \) 2>/dev/null | sort || echo "No library files found"
else
    echo "No dist directory found"
fi

# Exit with error if any builds failed
if [ ${#BUILD_FAILED[@]} -gt 0 ]; then
    echo ""
    echo "Some builds failed. Check the output above for details."
    exit 1
fi

echo ""
echo "All builds completed successfully!"
