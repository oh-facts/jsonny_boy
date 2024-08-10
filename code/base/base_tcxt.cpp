void tcxt_init()
{
	for(u32 i = 0; i < ARRAY_LEN(tcxt.arenas); i ++)
	{
		tcxt.arenas[i] = arena_create(Megabytes(10), Megabytes(64));
	}
}

void tcxt_process_debug_counters()
{
	for(u32 i = 0; i < ARRAY_LEN(tcxt.counters); i ++)
	{
		debug_cycle_counter *counter = tcxt.counters + i;
		debug_cycle_counter *counter_last = tcxt.counters_last + i;
		
		counter_last->hit_count = counter->hit_count;
		counter_last->cycle_count = counter->cycle_count;
		
		//printf("%d: %lu\n", i, counter->cycle_count);
		counter->hit_count = 0;
		counter->cycle_count = 0;
	}
}

void tcxt_print_debug_counters()
{
	for(u32 i = 0; i < ARRAY_LEN(tcxt.counters); i ++)
	{
		debug_cycle_counter *counter = tcxt.counters + i;
		
		printf("%s: %llu\n", debug_cycle_to_str[i], counter->cycle_count);
	}
}

Arena *tcxt_get_scratch(Arena **conflicts, u64 count)
{
	Arena *out = 0;
	for(u32 i = 0; i < ARRAY_LEN(tcxt.arenas); i ++)
	{
		b32 has_conflict = 0;
		for(u32 j = 0; j < count; j ++)
		{
			if(tcxt.arenas[i] == conflicts[j])
			{
				has_conflict = 1;
				break;
			}
		}
		if(!has_conflict)
		{
			out = tcxt.arenas[i];
		}
	}
	
	return out;
}