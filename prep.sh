#!/bin/bash

mkdir -p build
cd build

PREFIX=/usr
TOOLSET=i686-w64-mingw32

export CC="$TOOLSET-gcc -static-libgcc"

../configure --target="$TOOLSET" --host="$TOOLSET" --build=x86_64-linux --prefix="$PREFIX/$TOOLSET"
