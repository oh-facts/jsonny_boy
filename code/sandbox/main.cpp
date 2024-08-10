#define STB_SPRINTF_IMPLEMENTATION
#include "stb/stb_sprintf.h"

#include <stdint.h>

#include <stdio.h>
#include <base/base_context_cracking.h>

#if defined(OS_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>
#include <sys/mman.h>
#endif

#include <base/base_core.h>
#include <base/base_string.h>
#include <os/os.h>
#if defined(OS_WIN32)
#include <os/os_win32.cpp>
#else
#include <os/os_linux.cpp>
#endif
#include <base/base_tcxt.h>
#include <json_parser/json_lexer.h>
#include <json_parser/json_parser.h>
#include <json_parser/json_hasher.h>

#include <base/base_core.cpp>
#include <base/base_string.cpp>
#include <base/base_tcxt.cpp>
#include <json_parser/json_lexer.cpp>
#include <json_parser/json_parser.cpp>
#include <json_parser/json_hasher.cpp>

#include "platform.h"

struct cpu_state
{
	u64 pc, s, a, x, y, p;
	
	u64 ram[6][2];
};

struct cycle
{
	u64 a,b;
	Str8 op;
};

struct test_big
{
	Str8 name;
	cpu_state initial;
	cpu_state final;
	cycle cycles[7];
};

int main()
{
	tcxt_init();
	Arena_temp temp = scratch_begin(0,0);
	
	Str8 app_dir = os_get_app_dir(temp.arena);
	Str8 test_path = str8_join(temp.arena, app_dir, str8_lit("../tests/nes6502/00.json"));

	File_data test_data = read_file(temp.arena, (char*)test_path.c, FILE_TYPE_BINARY);
	
	BEGIN_TIMED_BLOCK(PARSE);	
	JP_Root root = jp_parse_from_buffer(test_data.bytes, test_data.size);
	END_TIMED_BLOCK(PARSE);
	
	scratch_end(&temp);
	
	BEGIN_TIMED_BLOCK(TESTS);
	JP_Object *last_test = root.array->values[9999].obj;
	JP_Object *initial = jp_value_from_keys(last_test, 1, str8_lit("initial")).obj;
	JP_Object *final = jp_value_from_keys(last_test, 1, str8_lit("final")).obj;
	
	test_big test = {};
	test.name = jp_value_from_keys(last_test, 1, str8_lit("name")).str;
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
	
	for(u32 i = 0; i < 6; i++)
	{
		for(u32 j = 0; j < 2; j++)
		{
			test.initial.ram[i][j] = ram_initial->values[i].array->values[j].number._u64;
			test.final.ram[i][j] = ram_final->values[i].array->values[j].number._u64;
		}
	}
	
	JP_Array *cycles = jp_value_from_keys(last_test, 1, str8_lit("cycles")).array;
	
	for(u32 i = 0; i < 7; i++)
	{
		test.cycles[i].a = cycles->values[i].array->values[0].number._u64;
		test.cycles[i].b = cycles->values[i].array->values[1].number._u64;
		test.cycles[i].op = cycles->values[i].array->values[2].str;
	}
	END_TIMED_BLOCK(TESTS);
	
	tcxt_print_debug_counters();
	
	return 0;
}