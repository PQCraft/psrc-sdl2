#!/bin/bash

cd build || exit 1

TOOLSET=i686-w64-mingw32

export CC="$TOOLSET-gcc -static-libgcc"

make -j
