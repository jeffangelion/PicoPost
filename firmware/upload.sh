#!/bin/sh
set -e

UF2="build/PicoBiosPostCodeReader.uf2"
if [ ! -f "$UF2" ]; then
    UF2="PicoPost.uf2"
fi

if [ ! -f "$UF2" ]; then
    echo "Error: Firmware UF2 file not found ($UF2)."
    echo "Please build it first with ./build.sh or download PicoPost.uf2 from GitHub Releases."
    exit 1
fi

if command -v picotool >/dev/null 2>&1; then
    echo "Uploading $UF2 via picotool..."
    picotool load -f "$UF2"
    picotool reboot -f
    echo "Done!"
else
    echo "picotool not found."
    echo "To flash without picotool:"
    echo "  1. Hold BOOT (BOOTSEL) on RP2040-Zero and connect USB to PC."
    echo "  2. Drag and drop $UF2 onto the mounted RPI-RP2 drive."
fi
