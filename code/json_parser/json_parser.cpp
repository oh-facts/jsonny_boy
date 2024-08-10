JP_Root jp_parse_from_buffer(u8 *buffer, u64 len)
{
	if(!jp_state.initialized)
	{
		jp_state.initialized = 1;
		jp_state.arena = arena_create(Megabytes(128), Gigabytes(64));
	}
	
	jp_state.scratch_arena = arena_create(Megabytes(128), Gigabytes(64));
	JP_Token_chunk_list token_list = jp_lex_from_buffer(buffer, len);
	
	//jp_print_tokens(&token_list);
	//printf("\n\n\n");
	
	JP_Root root = {};
	
	JP_Token_chunk *chunk = token_list.first;
	u64 i = 0;
	
	if(jp_get_token(chunk, i)->kind == JP_TokenKind_LBrace)
	{
		JP_Object *obj = jp_parse_object(&chunk, &i);
		root.kind = JP_RootKind_Object;
		root.obj = obj;
	}
	else if(jp_get_token(chunk, i)->kind == JP_TokenKind_LSquare)
	{
		JP_Array *array = jp_parse_array(&chunk, &i);
		root.kind = JP_RootKind_Array;
		root.array = array;
	}
	
	os_release(jp_state.scratch_arena, jp_state.scratch_arena->res);
	return root;
}

void jp_free()
{
	jp_state.initialized = 0;
	os_release(jp_state.arena, jp_state.arena->res);
}

JP_Number jp_number_from_lexeme(Str8 lexeme)
{
	JP_Number out = {};
	
	b32 sign = 0;
	u32 start = 0;
	f64 decim = 0.1;
	b32 is_float = 0;
	
	if(lexeme.c[0] == '-')
	{
		out.kind = JP_NumberKind_s64;
		++start;
		sign = 1;
	}
	
	for(u32 i = start; i < lexeme.len; ++i)
	{
		if(lexeme.c[i] == '.')
		{
			is_float = 1;
			out.kind = JP_NumberKind_f64;
		}
		
		if(is_float)
		{
			out._f64 += (lexeme.c[i] - '0') * decim;
			decim *= 0.1;
		}
		else
		{
			if(sign)
			{
				out._s64 = out._s64 * 10 + lexeme.c[i] - '0';
			}
			else
			{
				out._u64 = out._u64 * 10 + lexeme.c[i] - '0';
			}
		}
	}
	
	if(sign)
	{
		if(is_float)
		{
			out._f64 *= -1;
		}
		else
		{
			out._s64 *= -1;
		}
	}
	
	return out;
}

void jp_print_spaces()
{
	for(u32 i = 0; i < jp_state.tabs; i++)
	{
		printf(" ");
	}
}

void jp_print_tree(JP_Root *root)
{
	if(root->kind == JP_RootKind_Array)
	{
		jp_print_array(root->array);
	}
	else if(root->kind == JP_RootKind_Object)
	{
		jp_print_objects(root->obj);
	}
}

void jp_print_array(JP_Array *first)
{
	jp_state.tabs++;
	printf("[\n");
	
	for(u64 i = 0; i < first->num; i++)
	{
		jp_print_spaces();
		jp_print_value(&first->values[i]);
	}
	
	jp_state.tabs--;
	jp_print_spaces();
	printf("]\n");
}

void jp_print_objects(JP_Object *first)
{
	printf("{\n");
	++jp_state.tabs;
	for(JP_Object *cur = first; cur; cur = cur->next)
	{
		for(JP_Pair *pair = cur->first; pair; pair = pair->next)
		{
			jp_print_spaces();
			
			printf("%.*s : ", str8_varg(pair->key));
			
			jp_print_value(&pair->value);
		}
	}
	--jp_state.tabs;
	jp_print_spaces();
	printf("}\n");
}

void jp_print_value(JP_Value *value)
{
	switch(value->kind)
	{
		default: {INVALID_CODE_PATH();}
		case JP_ValueKind_Null:
		{
			printf("\"Null\"\n");
		}break;
		case JP_ValueKind_String:
		{
			printf("\"%.*s\"\n", str8_varg(value->str));
		}break;
		
		case JP_ValueKind_Boolean:
		{
			if(value->b)
			{
				printf("true\n");
			}
			else
			{
				printf("false\n");
			}
		}break;
		
		case JP_ValueKind_Number:
		{
			if(value->number.kind == JP_NumberKind_u64)
			{
				printf("%llu\n", value->number._u64);
			}
			
			else if(value->number.kind == JP_NumberKind_s64)
			{
				printf("%lli\n", value->number._s64);
			}
		}break;
		
		case JP_ValueKind_Object:
		{
			jp_print_objects(value->obj);
		}break;
		
		case JP_ValueKind_Array:
		{
			jp_print_array(value->array);
		}break;
	}
}

JP_Value jp_parse_value(JP_Token_chunk **chunk, u64 *i)
{
	JP_Value out = {};
	JP_Token *token = jp_get_token(*chunk, *i);
	switch(token->kind)
	{
		default:{INVALID_CODE_PATH();}
		case JP_TokenKind_String:
		{
			out.kind = JP_ValueKind_String;
			out.str = token->lexeme;
		}break;
		
		case JP_TokenKind_Number:
		{
			out.kind = JP_ValueKind_Number;
			out.number = jp_number_from_lexeme(token->lexeme);
		}break;
		
		case JP_TokenKind_Boolean:
		{
			out.kind = JP_ValueKind_Boolean;
			if(token->lexeme.c[0] == 't')
			{
				out.b = 1;
			}
			else
			{
				out.b = 0;
			}
		}break;
		
		case JP_TokenKind_LBrace:
		{
			out.kind = JP_ValueKind_Object;
			out.obj = jp_parse_object(chunk, i);
		}break;
		
		case JP_TokenKind_LSquare:
		{
			out.kind = JP_ValueKind_Array;
			out.array = jp_parse_array(chunk, i);
		}break;
	}
	
	return out;
}

JP_Array *jp_parse_array(JP_Token_chunk **chunk, u64 *i)
{
	JP_Array *out = push_struct(jp_state.arena, JP_Array);
	
	if(!jp_state.root.array)
	{
		jp_state.root.kind = JP_RootKind_Array;
		jp_state.root.array = out;;
	}
	
	// skip [
	jp_next_token(chunk, i);
	u64 num = 0;
	u64 nest = 0;
	u64 temp_i = *i;
	JP_Token_chunk *temp_chunk = *chunk;
	
	for(;;)
	{
		
		JP_Token *token = jp_get_token(temp_chunk, temp_i);
		if(token->kind == JP_TokenKind_LBrace || token->kind == JP_TokenKind_LSquare)
		{
			++nest;
		}
		
		else if(token->kind == JP_TokenKind_RBrace || token->kind == JP_TokenKind_RSquare)
		{
			--nest;
		}
		
		jp_next_token(&temp_chunk, &temp_i);
		token = jp_get_token(temp_chunk, temp_i);
		
		if(token->kind == JP_TokenKind_Comma && nest == 0)
		{
			++num;
		}
		
		if(nest == 0 && token->kind == JP_TokenKind_RSquare)
		{
			break;
		}
	}
	
	// final member doesnt have comma
	++num;
	out->values = push_array(jp_state.arena, JP_Value, num);
	
	for(u64 j = 0; j < num; j++)
	{
		out->values[j] = jp_parse_value(chunk, i);
		out->num++;
		jp_next_token(chunk, i);
		
		if(jp_get_token(*chunk, *i)->kind == JP_TokenKind_Comma)
		{
			// skip comma
			jp_next_token(chunk, i);
		}
	}
	
	return out;
}

JP_Object *jp_parse_object(JP_Token_chunk **chunk, u64 *i)
{
	JP_Object *out = push_struct(jp_state.arena, JP_Object);
	
	if(!jp_state.root.obj)
	{
		jp_state.root.kind = JP_RootKind_Object;
		jp_state.root.obj = out;
	}
	
	// skip brace
	jp_next_token(chunk, i);
	
	for(; jp_get_token(*chunk, *i)->kind != JP_TokenKind_RBrace;)
	{
		JP_Pair *pair = push_struct(jp_state.arena, JP_Pair);
		pair->key = jp_get_token(*chunk, *i)->lexeme;
		
		// next
		jp_next_token(chunk, i);
		
		// skip colon
		jp_next_token(chunk, i);
		
		pair->value = jp_parse_value(chunk, i);
		
		// next
		jp_next_token(chunk, i);
		if(jp_get_token(*chunk, *i)->kind == JP_TokenKind_Comma)
		{
			// skip comma
			jp_next_token(chunk, i);
		}
		
		jp_add_pair_to_hash(out, pair);
		if(out->last)
		{
			out->last = out->last->next = pair;
		}
		else
		{
			out->last = out->first = pair;
		}
		
		out->count++;
		
	}
	return out;
}