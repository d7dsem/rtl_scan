#!/bin/bash

clear
BUILD_DIR="../build/tests"
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR" || exit 1

cmake ../../ -DCMAKE_BUILD_TYPE=Debug || exit 1
cmake --build . --target tests_rtl || exit 1

if [ -f ./bin/tests_rtl ]; then
    clear
    # cd 
    ./bin/tests_rtl
else
    echo "Executable not found!"
    exit 1
fi
