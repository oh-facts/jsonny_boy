@echo off
setlocal
cd /D "%~dp0"

clang -O3 -o out/test.exe main.c
.\out\test.exe