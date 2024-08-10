#define _CRT_SECURE_NO_WARNINGS
#include "../common.h"
#include "../../include/jsonny_boy.h"

int main()
{
    printf("\nThis is jsonny boy\n");
    char file_path[1024];
    snprintf(file_path, sizeof(file_path), "%s/../../../tests/nes6502/00.json", os_get_app_dir());

    FILE *file = fopen(file_path, "rb");
    fseek(file, 0, SEEK_END);
    u64 file_size = ftell(file);
    rewind(file);

    u8 *buffer = (u8 *)malloc(file_size);
    u64 read_size = fread(buffer, 1, file_size, file);
    if (read_size != file_size)
    {
        // Leaving it here for interesting-ish read. Change fopen to "r".
        // windows is utterly fucked.
        printf("file_size: %llu \nread_size: %llu\n", file_size, read_size);
        perror("Failed to read the whole file");
        exit(0);
    }

    fclose(file);
    BEGIN_TIMED_BLOCK(PARSE);
    JP_Root root = jp_parse_from_buffer((u8 *)buffer, file_size);
    END_TIMED_BLOCK(PARSE);

    int test_num = get_random_num();
    BEGIN_TIMED_BLOCK(TESTS);
    
    JP_Object *last_test = root.array->values[test_num].obj;
    JP_Object *initial = jp_value_from_keys(last_test, 1, str8_lit("initial")).obj;
    JP_Object *final = jp_value_from_keys(last_test, 1, str8_lit("final")).obj;

    test_big test = {};
    test.name = (char *)jp_value_from_keys(last_test, 1, str8_lit("name")).str.c;
    test.initial.pc = jp_value_from_keys(initial, 1, str8_lit("pc")).number._u64;
    test.initial.s = jp_value_from_keys(initial, 1, str8_lit("s")).number._u64;
    test.initial.a = jp_value_from_keys(initial, 1, str8_lit("a")).number._u64;
    test.initial.x = jp_value_from_keys(initial, 1, str8_lit("x")).number._u64;
    test.initial.y = jp_value_from_keys(initial, 1, str8_lit("y")).number._u64;
    test.initial.p = jp_value_from_keys(initial, 1, str8_lit("p")).number._u64;

    test.final.pc = jp_value_from_keys(final, 1, str8_lit("pc")).number._u64;
    test.final.s = jp_value_from_keys(final, 1, str8_lit("s")).number._u64;
    test.final.a = jp_value_from_keys(final, 1, str8_lit("a")).number._u64;
    test.final.x = jp_value_from_keys(final, 1, str8_lit("x")).number._u64;
    test.final.y = jp_value_from_keys(final, 1, str8_lit("y")).number._u64;
    test.final.p = jp_value_from_keys(final, 1, str8_lit("p")).number._u64;

    JP_Array *ram_initial = jp_value_from_keys(initial, 1, str8_lit("ram")).array;
    JP_Array *ram_final = jp_value_from_keys(final, 1, str8_lit("ram")).array;

    for (u32 i = 0; i < 6; i++)
    {
        for (u32 j = 0; j < 2; j++)
        {
            test.initial.ram[i][j] = ram_initial->values[i].array->values[j].number._u64;
            test.final.ram[i][j] = ram_final->values[i].array->values[j].number._u64;
        }
    }

    JP_Array *cycles = jp_value_from_keys(last_test, 1, str8_lit("cycles")).array;

    for (u32 i = 0; i < 7; i++)
    {
        test.cycles[i].a = cycles->values[i].array->values[0].number._u64;
        test.cycles[i].b = cycles->values[i].array->values[1].number._u64;
        test.cycles[i].op = (char *)cycles->values[i].array->values[2].str.c;
    }
    END_TIMED_BLOCK(TESTS);

    tcxt_print_debug_counters();
    print_test_big(&test, test_num);
    return 0;
}