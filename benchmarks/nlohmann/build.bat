@echo off
setlocal
cd /D "%~dp0"

clang -O3 -I./../../code/ -o out/test.exe main.cpp -Wno-writable-strings
.\out\test.exe