#!/bin/bash
cd "${0%/*}"

clang -O3 -Wno-writable-strings main.cpp -o out/test -Lout -ljsonny_boy -Wno-format
./out/test