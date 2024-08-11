<p>
<h1 align="center">JSonny Boy</h2>
<p align="center">JSON parser for C++</p>
<p align="center">
<img width="400"src="https://github.com/user-attachments/assets/7ee7d368-0b47-4dda-9650-a31f11ec7557">
</p>
</p>

## Table of Contents

1. [About](#about)
2. [Build](#build)
   - [Windows](#windows)
   - [Linux / Mac](#linux--mac)
3. [Benchmarks](#benchmarks)
4. [Contributions](#contributions)
5. [Project Structure](#project-structure)

## About
This is a lightweight and high performance json parser for C++. It has a very simple (albeit ugly) api.

```
struct boy
{
    Str8 name;
    uint64_t age;
    float weight;
    b32 is_dead;
    u8 data[100];
};

JP_Root root = jp_parse_from_buffer((u8 *)buffer, file_size);

boy boy = {};
boy.name = (char *)jp_value_from_keys(last_test, 1, str8_lit("name")).str;
boy.age = jp_value_from_keys(initial, 1, str8_lit("age")).number._u64;
boy.weight = jp_value_from_keys(initial, 1, str8_lit("weight")).number._f64;
boy.is_dead = jp_value_from_keys(initial, 1, str8_lit("is_dead")).b;
boy.data = jp_value_from_keys(initial, 1, str8_lit("data")).array->values;
```

There isn't much that can be done, given that this is C syled. Perhaps some helper functions that convert to specific types. However, an api that uses [ ] overloads is in the works.

Also, look at `code/sandbox/main.cpp` for a much more comprehensive example.

One reason why its so lightweight is because there aren't many features. Only parsing a json file, retrieving a value from a key, and printing them. I hope to achieve feature parity with other json parsers. (Also, it is open source because I hoped it would get some attention and contributions)

**One of its biggest problems** is that it likely doesn't meet JSON standards. I was able to parse several json files while using it for my purposes, but I haven't sat through the JSON standard and run specific tests. Still, it is close enough for now, and I still think it is worth sharing in its current state.

I would spend more time on this but I am left with very little time since I am balancing an internship and some coursework.

## Build
The following args can be passed - clean, debug, release, sandbox

### Windows
```
./build.bat release
```

### Linux / Mac
```
./build.sh release
```

This compiles a static library. This can then be linked with your project. Headers are in `include`

## Benchmarks
```
This is cjson
parse: 524558528
tests: 570912

This is nlohmann
parse: 707370816
tests: 83296

This is jsonny boy
parse: 450850784
tests: 7040
```

The numbers are # of clock cycles it took to execute. The test file is a 5mb json file from an nes 6502 test suite.
`parse` is the the time taken to parse the file and `tests` is the time taken to index into one of the 6502 tests and grab data from it.

You can look at `benchmarks/xxx/main.x` for more details.

You can run the `benchmark` script to look at the data yourself. The test isn't nearly as comprehensive as one would like but I think the difference is still astonishing, and I intend to make the tests better.

## Contributions
Open an issue first. We can talk about it, and then you can submit a pr. Feel free to ask any questions, suggestions, etc. you might have.

## Project structure

`benchmarks/` : Benchmark against cjson and nlohmann, two very popular json parsers.

`code/base` : Base layer, utility functions

`code/json_parser` : All of jsonny_boy's code

`code/os` : OS abstraction. Need it for memory alloc functions

`code/sandbox` : This is where the library is tested

`stb/stb_sprintf.h` : sprintf alternative. My string library uses it but since the parser doesn't make any actual use of it, its basically unused for now.

`include/` : Include folder for the library

`tests/` : Some sample json files.
