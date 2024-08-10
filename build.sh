#!/bin/bash
cd "${0%/*}"

for arg in "$@"; do eval "$arg=1"; done

common_flags="-std=c++17 -msse4.1 -fno-rtti -fno-exceptions -Wall -Wno-unused-function -Wno-writable-strings -Wno-write-strings -Wno-comment -Wno-misleading-indentation -Wno-unused-result"

debug_build="-O0 -g"
release_build="-O3"

[ "$cloc" == "1" ] && cloc --exclude-list-file=.clocignore "code/" && exit 0
[ "$debug" == "1" ] && build_type="$debug_build" 
[ "$release" == "1" ] && build_type="$release_build"
[ "$clang" == "1" ] && compiler_type="clang" && echo "[clang]"
[ "$gcc" == "1" ] && compiler_type="gcc" && echo "[gcc]"

[ "$clean" == "1" ] && echo "deleted /out && code/meta.h" && rm -rf "out/" && rm "code/meta.h"

[ "$build_type" == "$debug_build" ] && echo "[debug build]"
[ "$build_type" == "$release_build" ] && echo "[release build]"

[ ! -d "out" ] && mkdir "out" && echo "[created build dir]"

[ "$sandbox" == "1" ] && clang $common_flags $build_type -I./code/ code/sandbox/main.cpp -o "out/sandbox" && echo "sandbox"

[ "$sandbox" == "1" ] && ./out/sandbox

clang $common_flags $build_type -I./code/ -c code/json_parser/jsonny_boy.cpp -o out/jsonny_boy.o
ar rcs out/libjsonny_boy.a out/jsonny_boy.o