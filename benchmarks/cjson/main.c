#include "cJSON.h"
#include "cJSON.c"

#include "../common.h"

int main()
{
    printf("\nThis is cjson\n");
    char file_path[1024];
    snprintf(file_path, sizeof(file_path), "%s/../../../tests/nes6502/00.json", os_get_app_dir());

    FILE *file = fopen(file_path, "r");
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *buffer = malloc(file_size);
    fread(buffer, 1, file_size, file);

    fclose(file);

    BEGIN_TIMED_BLOCK(PARSE);
    cJSON *json = cJSON_ParseWithLength(buffer, file_size);
    END_TIMED_BLOCK(PARSE);

    BEGIN_TIMED_BLOCK(TESTS);
    int test_num = get_random_num();
    cJSON *json_test = cJSON_GetArrayItem(json, test_num);
    test_big test;

    test.name = cJSON_GetObjectItem(json_test, "name")->valuestring;

    cJSON *initial_obj = cJSON_GetObjectItem(json_test, "initial");
    test.initial.pc = cJSON_GetObjectItem(initial_obj, "pc")->valueint;
    test.initial.s = cJSON_GetObjectItem(initial_obj, "s")->valueint;
    test.initial.a = cJSON_GetObjectItem(initial_obj, "a")->valueint;
    test.initial.x = cJSON_GetObjectItem(initial_obj, "x")->valueint;
    test.initial.y = cJSON_GetObjectItem(initial_obj, "y")->valueint;
    test.initial.p = cJSON_GetObjectItem(initial_obj, "p")->valueint;

    cJSON *ram_array = cJSON_GetObjectItem(initial_obj, "ram");
    for (int i = 0; i < 6; i++)
    {
        cJSON *ram_pair = cJSON_GetArrayItem(ram_array, i);
        test.initial.ram[i][0] = cJSON_GetArrayItem(ram_pair, 0)->valueint;
        test.initial.ram[i][1] = cJSON_GetArrayItem(ram_pair, 1)->valueint;
    }

    cJSON *cycles_array = cJSON_GetObjectItem(json_test, "cycles");
    for (int i = 0; i < 7; i++)
    {
        cJSON *cycle_obj = cJSON_GetArrayItem(cycles_array, i);
        test.cycles[i].a = cJSON_GetArrayItem(cycle_obj, 0)->valueint;
        test.cycles[i].b = cJSON_GetArrayItem(cycle_obj, 1)->valueint;
        test.cycles[i].op = cJSON_GetArrayItem(cycle_obj, 2)->valuestring;
    }

    END_TIMED_BLOCK(TESTS);

    tcxt_print_debug_counters();

    print_test_big(&test, test_num);

    return 0;
}