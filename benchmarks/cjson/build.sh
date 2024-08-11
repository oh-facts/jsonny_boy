#!/bin/bash
cd "${0%/*}"

if [ ! -d "out" ]; then
    mkdir "out"
fi
clang -O3 -o out/test main.c -Wno-format
./out/test