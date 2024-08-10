/* date = August 7th 2024 9:27 am */

#ifndef JSON_PARSER_H
#define JSON_PARSER_H

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
		u64 _u64;
		s64 _s64;
		f64 _f64;
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
		b32 b;
		JP_Object *obj;
		JP_Array *array;
	};
};

struct JP_Pair
{
	JP_Pair *next;
	JP_Pair *hash_next;
	u64 hash;
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

struct JP_State
{
	b32 initialized;
	Arena *scratch_arena;
	Arena *arena;
	u32 tabs;
	JP_Root root;
};

global JP_State jp_state;

// ===public api===
YK_API JP_Root jp_parse_from_buffer(u8 *buffer, u64 len);
YK_API void jp_free();
// ===============

// ===debug api===
YK_API void jp_print_spaces();
YK_API void jp_print_tree(JP_Root *root);
YK_API void jp_print_array(JP_Array *first);
YK_API void jp_print_objects(JP_Object *first);
YK_API void jp_print_value(JP_Value *value);
// ===============

// ===internal===
function JP_Number jp_number_from_lexeme(Str8 lexeme);
function JP_Value jp_parse_value(JP_Token_chunk **chunk, u64 *i);
function JP_Array *jp_parse_array(JP_Token_chunk **chunk, u64 *i);
function JP_Object *jp_parse_object(JP_Token_chunk **chunk, u64 *i);
// ===============

#endif //JP_H