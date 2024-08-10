#!/bin/bash
cd "${0%/*}"

clang -O3 -o out/test main.c -Wno-format
./out/test