@echo off
setlocal
cd /D "%~dp0"

if not exist "out" mkdir "out"
clang -O3 -o out/test.exe main.c
.\out\test.exe