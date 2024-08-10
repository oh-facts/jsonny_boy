@echo off
setlocal
cd /D "%~dp0"
for %%a in (%*) do set "%%a=1"

if "%clean%" == "1" del /q "benchmarks\cjson\out\*" && del /q "benchmarks\nlohmann\out\*" && del /q "benchmarks\jsonny_boy\out\*" && echo [cleaned] && exit /b 0

call "build.bat" release
copy "out\jsonny_boy.lib" "benchmarks\jsonny_boy\out\jsonny_boy.lib"

call "benchmarks/cjson/build.bat"
call "benchmarks/nlohmann/build.bat"
call "benchmarks/jsonny_boy/build.bat"