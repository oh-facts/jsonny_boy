#!/bin/bash
cd "${0%/*}"

for arg in "$@"; do eval "$arg=1"; done

if [ "$clean" == "1" ]; then
    rm -rf benchmarks/cjson/out/*
    rm -rf benchmarks/nlohmann/out/*
    rm -rf benchmarks/jsonny_boy/out/*
    echo "[cleaned]"
    exit 0
fi

./benchmarks/cjson/build.sh
./benchmarks/nlohmann/build.sh
./benchmarks/jsonny_boy/build.sh