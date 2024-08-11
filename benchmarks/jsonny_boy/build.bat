@echo off
setlocal
cd /D "%~dp0"

if not exist "out" mkdir "out"
clang -O3 -Wno-writable-strings -lout/jsonny_boy main.cpp -o out/test.exe
.\out\test.exe