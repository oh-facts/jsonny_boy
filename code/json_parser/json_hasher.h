/* date = August 9th 2024 5:12 am */

#ifndef JSON_HASHER_H
#define JSON_HASHER_H

#define JP_HASH_SLOTS_MAX 32

struct JP_Hash_slot
{
	JP_Pair *first;
	JP_Pair *last;
};

// ===public api===

YK_API JP_Value jp_value_from_keys(JP_Object *obj, u32 num_keys, ...);

// ===============

// ===internal===
// djb2
function unsigned long jp_hash(Str8 str);
function JP_Value jp_value_from_key(Str8 key);
function JP_Pair *jp_pair_from_hash(JP_Object *obj, u64 hash);
function void jp_add_pair_to_hash(JP_Object *obj, JP_Pair *pair);
// ===============

#endif //JSON_HASHER_H