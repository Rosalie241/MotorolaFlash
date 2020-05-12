#!/usr/bin/env bash
set -e
publish_dir="publish"
arch="$1"

mkdir -p "$publish_dir"

# Build
./scripts/build_msys2.sh Release

# Copy build artifacts
cp build/MotorolaFlash.exe "$publish_dir/MotorolaFlash.exe"
cp build/ThirdParty/fastboot/AdbWinApi.dll \
    "$publish_dir/AdbWinApi.dll"
cp build/ThirdParty/fastboot/AdbWinUsbApi.dll \
    "$publish_dir/AdbWinUsbApi.dll"

if [ "$arch" = "x86" ]
then
    libdir="/mingw32/bin"
else
    libdir="/mingw64/bin"
fi

# Bundle dependencies
./scripts/bundle_dlls.sh "$publish_dir/" "$publish_dir/MotorolaFlash.exe" "$libdir"

# Ship Qt
windeployqt "$publish_dir/MotorolaFlash.exe" "$publish_dir"
