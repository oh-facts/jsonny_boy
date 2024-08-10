#!/bin/bash
cd "${0%/*}"

clang++ -O3 -I./../../code/ -o out/test main.cpp -Wno-writable-strings -Wno-format
./out/test