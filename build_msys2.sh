#!/usr/bin/env bash
set -e

# We *need* clang
export CC=clang
export CXX=clang++

mkdir -p build && cd build

cmake -G"MSYS Makefiles" -DCMAKE_BUILD_TYPE=${1:-Debug} ..

make -j$(nproc 2>/dev/null || echo 1)
