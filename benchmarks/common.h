#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int get_random_num()
{
	srand(time(NULL));
	int out = rand() % 10000;
	return out;
}

#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

char *os_get_app_dir()
{
	static char buffer[256] = {0};
	DWORD len = GetModuleFileName(0, buffer, sizeof(buffer));

	char *c = buffer + len;
	while (c > buffer && *(--c) != '\\')
		*c = 0;

	return buffer;
}
#else
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>

char *os_get_app_dir()
{
	static char buffer[PATH_MAX] = {0};
	ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);

	if (len != -1)
	{
		buffer[len] = '\0'; // Null-terminate the string

		// Find the last slash '/' and terminate the string there to get the directory
		char *c = buffer + len;
		while (c > buffer && *(--c) != '/')
			*c = 0;
	}
	else
	{
		// Handle error (e.g., readlink failed)
		buffer[0] = '\0';
	}

	return buffer;
}
#endif
#define ARRAY_LEN(arr) (sizeof((arr)) / sizeof((arr)[0]))

#define Kilobytes(Value) ((uint64_t)(Value) * 1024)
#define Megabytes(Value) (Kilobytes(Value) * 1024)
#define Gigabytes(Value) (Megabytes(Value) * 1024)
#define Terabytes(Value) (Gigabytes(Value) * 1024)

#define function static
#define global static
#define local_persist static

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef double f64;

typedef int32_t b32;
enum DEBUG_CYCLE_COUNTER
{
	DEBUG_CYCLE_COUNTER_PARSE,
	DEBUG_CYCLE_COUNTER_TESTS,
	DEBUG_CYCLE_COUNTER_COUNT
};

global char *debug_cycle_to_str[DEBUG_CYCLE_COUNTER_COUNT] =
	{
		"parse",
		"tests"};

typedef struct debug_cycle_counter debug_cycle_counter;
struct debug_cycle_counter
{
	u64 cycle_count;
	u32 hit_count;
};

struct TCXT
{
	debug_cycle_counter counters[DEBUG_CYCLE_COUNTER_COUNT];
	debug_cycle_counter counters_last[DEBUG_CYCLE_COUNTER_COUNT];
};

global struct TCXT tcxt;

#define BEGIN_TIMED_BLOCK(ID)               \
	u64 start_cycle_count_##ID = __rdtsc(); \
	++tcxt.counters[DEBUG_CYCLE_COUNTER_##ID].hit_count
#define END_TIMED_BLOCK(ID) tcxt.counters[DEBUG_CYCLE_COUNTER_##ID].cycle_count += __rdtsc() - start_cycle_count_##ID

void tcxt_print_debug_counters()
{
	for (u32 i = 0; i < DEBUG_CYCLE_COUNTER_COUNT; i++)
	{
		debug_cycle_counter *counter = tcxt.counters + i;

		printf("%s: %llu\n", debug_cycle_to_str[i], counter->cycle_count);
	}
}

typedef struct cpu_state cpu_state;
struct cpu_state
{
	u64 pc, s, a, x, y, p;

	u64 ram[6][2];
};

typedef struct cycle cycle;
struct cycle
{
	u64 a, b;
	const char *op;
};

typedef struct test_big test_big;
struct test_big
{
	const char *name;
	cpu_state initial;
	cpu_state final;
	cycle cycles[7];
};

void print_cpu_state(FILE *file, const cpu_state *state)
{
	fprintf(file, "CPU State:\n");
	fprintf(file, "  PC: %llu\n", state->pc);
	fprintf(file, "  S:  %llu\n", state->s);
	fprintf(file, "  A:  %llu\n", state->a);
	fprintf(file, "  X:  %llu\n", state->x);
	fprintf(file, "  Y:  %llu\n", state->y);
	fprintf(file, "  P:  %llu\n", state->p);

	fprintf(file, "  RAM:\n");
	for (int i = 0; i < 6; ++i)
	{
		fprintf(file, "    [%d]: { %llu, %llu }\n", i, state->ram[i][0], state->ram[i][1]);
	}
}

void print_cycle(FILE *file, const cycle *c)
{
	fprintf(file, "Cycle:\n");
	fprintf(file, "  A:  %llu\n", c->a);
	fprintf(file, "  B:  %llu\n", c->b);
	fprintf(file, "  OP: %s\n", c->op ? c->op : "NULL");
}

void print_test_big(const test_big *test, int num)
{
	FILE *file = fopen("out/test_big.txt", "w");
	if (file == NULL)
	{
		perror("Failed to open file");
		return;
	}
	fprintf(file, "index: %d\n", num);
	fprintf(file, "Test Name: %s\n", test->name);

	fprintf(file, "Initial State:\n");
	print_cpu_state(file, &test->initial);

	fprintf(file, "Final State:\n");
	print_cpu_state(file, &test->final);

	fprintf(file, "Cycles:\n");
	for (int i = 0; i < 7; ++i)
	{
		fprintf(file, "  Cycle %d:\n", i);
		print_cycle(file, &test->cycles[i]);
	}

	fclose(file);
}