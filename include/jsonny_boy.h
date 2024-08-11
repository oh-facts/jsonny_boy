#include <stdint.h>
#include <stdio.h>

#define str8_varg(S) (int)((S).len), ((S).c)

struct Str8
{
  uint8_t *c;
  u64 len;
};

#define str8_lit(c) Str8{(u8*)c, sizeof(c) - 1}

enum JP_NumberKind
{
	JP_NumberKind_u64,
	JP_NumberKind_s64,
	JP_NumberKind_f64,
};

struct JP_Number
{
	enum JP_NumberKind kind;
	union
	{
		uint64_t _u64;
		int64_t _s64;
		double _f64;
	};
};

enum JP_ValueKind
{
	JP_ValueKind_Null,
	JP_ValueKind_String,
	JP_ValueKind_Number,
	JP_ValueKind_Boolean,
	JP_ValueKind_Object,
	JP_ValueKind_Array,
};

struct JP_Value;
struct JP_Object;

struct JP_Array
{
	JP_Value *values;
	u64 num;
};

struct JP_Value
{
	JP_ValueKind kind;
	union
	{
		Str8 str;
		JP_Number number;
		int32_t b;
		JP_Object *obj;
		JP_Array *array;
	};
};

struct JP_Pair
{
	JP_Pair *next;
	JP_Pair *hash_next;
	uint64_t hash;
	Str8 key;
	JP_Value value;
};

struct JP_Hash_slot;

struct JP_Object
{
	JP_Object *next;
	JP_Pair *first;
	JP_Pair *last;
	u64 count;
	JP_Hash_slot *hash_slots;
};

enum JP_RootKind
{
	JP_RootKind_Array,
	JP_RootKind_Object,
};

struct JP_Root
{
	JP_RootKind kind;
	union
	{
		JP_Array *array;
		JP_Object *obj;
	};
};

struct JP_Hash_slot
{
	JP_Pair *first;
	JP_Pair *last;
};

// ===public api===
JP_Root jp_parse_from_buffer(uint8_t *buffer, uint64_t len);

// Frees all data allocated by the library
void jp_free();
JP_Value jp_value_from_keys(JP_Object *obj, uint32_t num_keys, ...);
// ===============

// ===debug api===
void jp_print_tree(JP_Root *root);
void jp_print_array(JP_Array *first);
void jp_print_objects(JP_Object *first);
void jp_print_value(JP_Value *value);
// ===============
