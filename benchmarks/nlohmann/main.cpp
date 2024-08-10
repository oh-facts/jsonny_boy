#define _CRT_SECURE_NO_WARNINGS
#include "nlohmann.h"

#include "../common.h"

#include <iostream>
#include <fstream>
#include <sstream>

using json = nlohmann::json;
int main()
{
    printf("\nThis is nlohmann\n");
    std::string app_dir = os_get_app_dir();
    const std::string file_path = app_dir + "/../../../tests/nes6502/00.json";

    std::ifstream file(file_path);

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string file_contents = buffer.str();

    BEGIN_TIMED_BLOCK(PARSE);
    json j = json::parse(file_contents);
    END_TIMED_BLOCK(PARSE);
    int test_num = get_random_num();
    BEGIN_TIMED_BLOCK(TESTS);
    json json_test = j[test_num];
    test_big test;

    test.name = json_test["name"].get<std::string>().c_str();

    json initial_obj = json_test["initial"];
    test.initial.pc = initial_obj["pc"].get<uint64_t>();
    test.initial.s = initial_obj["s"].get<uint64_t>();
    test.initial.a = initial_obj["a"].get<uint64_t>();
    test.initial.x = initial_obj["x"].get<uint64_t>();
    test.initial.y = initial_obj["y"].get<uint64_t>();
    test.initial.p = initial_obj["p"].get<uint64_t>();

    json ram_array = initial_obj["ram"];
    for (int i = 0; i < 6; i++)
    {
        json ram_pair = ram_array[i];
        test.initial.ram[i][0] = ram_pair[0].get<uint64_t>();
        test.initial.ram[i][1] = ram_pair[1].get<uint64_t>();
    }

    json cycles_array = json_test["cycles"];
    for (int i = 0; i < 7; i++)
    {
        json cycle_obj = cycles_array[i];
        test.cycles[i].a = cycle_obj[0].get<uint64_t>();
        test.cycles[i].b = cycle_obj[1].get<uint64_t>();
        test.cycles[i].op = cycle_obj[2].get<std::string>().c_str();
    }

    END_TIMED_BLOCK(TESTS);

    tcxt_print_debug_counters();
    print_test_big(&test, test_num);
    return 0;
}