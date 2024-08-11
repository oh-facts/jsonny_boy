#!/bin/bash
cd "${0%/*}"

if [ ! -d "out" ]; then
    mkdir "out"
fi
clang++ -O3 -I./../../code/ -o out/test main.cpp -Wno-writable-strings -Wno-format
./out/test