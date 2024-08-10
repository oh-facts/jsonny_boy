JP_Value jp_value_from_keys(JP_Object *obj, u32 num_keys, ...)
{
	va_list args;
	va_start(args, num_keys);
	Str8 key = va_arg(args, Str8);
	JP_Pair *pair = jp_pair_from_hash(obj, jp_hash(key));
	
	for(u32 i = 1; i < num_keys; i++)
	{
		key = va_arg(args, Str8);
		pair = jp_pair_from_hash(pair->value.obj, jp_hash(key));
	}
	va_end(args);
	return pair->value;
}

// djb2
unsigned long jp_hash(Str8 str)
{
	unsigned long hash = 5381;
	int c;
	
	for(u32 i = 0; i < str.len; i++)
	{
		c = str.c[i];
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	
	return hash;
}

JP_Value jp_value_from_key(Str8 key)
{
	JP_Value out = {};
	JP_Pair *pair = jp_pair_from_hash(jp_state.root.obj, jp_hash(key));
	
	if(pair)
	{
		out = pair->value;
	}
	
	return out;
}

JP_Pair *jp_pair_from_hash(JP_Object *obj, u64 hash)
{
	JP_Pair *out = 0;
	u64 slot = hash % JP_HASH_SLOTS_MAX;
	
	JP_Pair *cur = obj->hash_slots[slot].first;
	while(cur)
	{
		if(cur->hash == hash)
		{
			out = cur;
			break;
		}
		
		cur = cur->hash_next;
	}
	
	return out;
}

void jp_add_pair_to_hash(JP_Object *obj, JP_Pair *pair)
{
	u64 hash = jp_hash(pair->key);
	u64 slot = hash % JP_HASH_SLOTS_MAX;
	pair->hash = hash;
	
	if(!obj->hash_slots)
	{
		obj->hash_slots = push_array(jp_state.arena, JP_Hash_slot, JP_HASH_SLOTS_MAX);
	}
	
	if(obj->hash_slots[slot].last)
	{
		obj->hash_slots[slot].last = obj->hash_slots[slot].last->hash_next = pair;
	}
	else
	{
		obj->hash_slots[slot].last = obj->hash_slots[slot].first = pair;
	}
	
}