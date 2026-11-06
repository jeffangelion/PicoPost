#!/bin/sh
set -e

# If PICO_SDK_PATH is not set, check for local pico-sdk directory
if [ -z "$PICO_SDK_PATH" ]; then
    if [ -d "pico-sdk" ]; then
        export PICO_SDK_PATH="$(realpath pico-sdk)"
    else
        echo "Error: PICO_SDK_PATH environment variable is not set and local pico-sdk/ was not found."
        echo "Please set PICO_SDK_PATH (e.g. export PICO_SDK_PATH=/path/to/pico-sdk) or clone it locally:"
        echo "  git clone --recurse-submodules https://github.com/raspberrypi/pico-sdk.git pico-sdk"
        exit 1
    fi
fi

echo "Using PICO_SDK_PATH: $PICO_SDK_PATH"
echo "Configuring with CMake..."
cmake -B build -S .
echo "Building project..."
cmake --build build -j"$(nproc 2>/dev/null || echo 4)"
echo "Build complete! Output: build/PicoBiosPostCodeReader.uf2"
