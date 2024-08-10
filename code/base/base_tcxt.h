/* date = July 22nd 2024 4:41 pm */

#ifndef BASE_TCXT_H
#define BASE_TCXT_H

enum DEBUG_CYCLE_COUNTER
{
	DEBUG_CYCLE_COUNTER_LEX,
	DEBUG_CYCLE_COUNTER_PARSE,
	DEBUG_CYCLE_COUNTER_TESTS,
	DEBUG_CYCLE_COUNTER_COUNT
};

global char *debug_cycle_to_str[DEBUG_CYCLE_COUNTER_COUNT] = 
{
	"lex",
	"parse",
	"tests"
};

struct debug_cycle_counter
{
	u64 cycle_count;
	u32 hit_count;
};

struct TCXT
{
	Arena *arenas[2];
	debug_cycle_counter counters[DEBUG_CYCLE_COUNTER_COUNT];
	debug_cycle_counter counters_last[DEBUG_CYCLE_COUNTER_COUNT];
};

global TCXT tcxt;

#if defined (OS_WIN32) || defined(OS_LINUX)
#define BEGIN_TIMED_BLOCK(ID) u64 start_cycle_count_##ID = __rdtsc(); ++tcxt.counters[DEBUG_CYCLE_COUNTER_##ID].hit_count
#define END_TIMED_BLOCK(ID)  tcxt.counters[DEBUG_CYCLE_COUNTER_##ID].cycle_count += __rdtsc() - start_cycle_count_##ID

#else
#define BEGIN_TIMED_BLOCK(ID)
#define END_TIMED_BLOCK(ID)
#endif

function void tcxt_init();
function void tcxt_process_debug_counters();
function void tcxt_print_debug_counters();
function Arena *tcxt_get_scratch(Arena **conflicts, u64 count);

#define scratch_begin(conflicts, count) arena_temp_begin(tcxt_get_scratch(conflicts, count))
#define scratch_end(scratch) arena_temp_end(scratch);

#endif //BASE_TCXT_H