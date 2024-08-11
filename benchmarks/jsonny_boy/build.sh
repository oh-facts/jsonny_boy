#!/bin/bash
cd "${0%/*}"

if [ ! -d "out" ]; then
    mkdir "out"
fi
clang -O3 -Wno-writable-strings main.cpp -o out/test -Lout -ljsonny_boy -Wno-format
./out/test