b32 jp_is_whitespace(char c)
{
	b32 out = 0;
	switch(c)
	{
		case ' ':
		case '\n':
		case '\r':
		case '\t':
		{
			out = 1;
		}break;
	}
	
	return out;
}

b32 jp_is_number(char c)
{
	b32 out = 0;
	switch(c)
	{
		case '0': case '1': case '2': case '3': case '4': 
		case '5': case '6': case '7': case '8': case '9':
		case '.':
		{
			out = 1;
		}break;
	}
	
	return out;
}

JP_Token *jp_push_token(JP_Token_chunk_list *list)
{
	JP_Token_chunk *chunk = list->last;
	
	if(!chunk || (chunk->num + 1 > TOKEN_CHUNK_SIZE))
	{
		chunk = jp_push_token_chunk(list);
	}
	
	JP_Token *out = chunk->tokens + chunk->num++;
	
	return out;
}

JP_Token_chunk *jp_push_token_chunk(JP_Token_chunk_list *list)
{
	JP_Token_chunk *out = push_struct(jp_state.scratch_arena, JP_Token_chunk);
	list->num++;
	
	if(list->last)
	{
		list->last = list->last->next = out;
	}
	else
	{
		list->last = list->first = out;
	}
	
	return out;
}

JP_Token_chunk_list jp_lex_from_buffer(u8 *buffer, u64 len)
{
	JP_Token_chunk_list out = {};
	u64 i = 0;
	for(u8 *c = buffer; i < len; )
	{
		switch(c[i])
		{
			default :
			{
				while(jp_is_whitespace(c[i]))
				{
					++i;
				}
				
			}break;
			
			case '{':
			{
				JP_Token *token = jp_push_token(&out);
				token->kind = JP_TokenKind_LBrace;
				token->lexeme.c = push_array(jp_state.arena, u8, 1);
				token->lexeme.c[0] = '{';
				token->lexeme.len = 1;
				
				++i;
			}break;
			
			case '}':
			{
				JP_Token *token = jp_push_token(&out);
				token->kind = JP_TokenKind_RBrace;
				token->lexeme.c = push_array(jp_state.arena, u8, 1);
				token->lexeme.c[0] = '}';
				token->lexeme.len = 1;
				
				++i;
			}break;
			
			case '[':
			{
				JP_Token *token = jp_push_token(&out);
				token->kind = JP_TokenKind_LSquare;
				token->lexeme.c = push_array(jp_state.arena, u8, 1);
				token->lexeme.c[0] = '[';
				token->lexeme.len = 1;
				
				++i;
			}break;
			
			case ']':
			{
				JP_Token *token = jp_push_token(&out);
				token->kind = JP_TokenKind_RSquare;
				token->lexeme.c = push_array(jp_state.arena, u8, 1);
				token->lexeme.c[0] = ']';
				token->lexeme.len = 1;
				
				++i;
			}break;
			
			case ':':
			{
				JP_Token *token = jp_push_token(&out);
				token->kind = JP_TokenKind_Colon;
				token->lexeme.c = push_array(jp_state.arena, u8, 1);
				token->lexeme.c[0] = ':';
				token->lexeme.len = 1;
				
				++i;
			}break;
			
			case ',':
			{
				JP_Token *token = jp_push_token(&out);
				token->kind = JP_TokenKind_Comma;
				token->lexeme.c = push_array(jp_state.arena, u8, 1);
				token->lexeme.c[0] = ',';
				token->lexeme.len = 1;
				
				++i;
			}break;
			
			case '\"':
			{
				JP_Token *token = jp_push_token(&out);
				token->kind = JP_TokenKind_String;
				++i;
				u8 *peek = c + i;
				u32 len = 0;
				while(peek[++len] != '\"')
				{
					
				}
				token->lexeme.c = push_array(jp_state.arena, u8, len);
				mem_cpy(token->lexeme.c, c + i, len);
				token->lexeme.len = len;
				
				i += len + 1;
				
			}break;
			
			case 't':
			{
				JP_Token *token = jp_push_token(&out);
				token->kind = JP_TokenKind_Boolean;
				token->lexeme.c = push_array(jp_state.arena, u8, 4);
				
				char true_str[5] = "true";
				mem_cpy(token->lexeme.c, true_str, 4);
				token->lexeme.len = 4;
				
				i += 4;
			}break;
			
			case 'f':
			{
				JP_Token *token = jp_push_token(&out);
				token->kind = JP_TokenKind_Boolean;
				token->lexeme.c = push_array(jp_state.arena, u8, 5);
				
				char false_str[6] = "false";
				mem_cpy(token->lexeme.c, false_str, 5);
				token->lexeme.len = 5;
				
				i += 5;
			}break;
			
			case '0': case '1': case '2': case '3': case '4': 
			case '5': case '6': case '7': case '8': case '9':
			{
				JP_Token *token = jp_push_token(&out);
				token->kind = JP_TokenKind_Number;
				u8 *peek = c + i;
				u32 len = 0;
				while(jp_is_number(peek[++len]))
				{
					
				}
				token->lexeme.c = push_array(jp_state.arena, u8, len);
				mem_cpy(token->lexeme.c, c + i, len);
				token->lexeme.len = len;
				
				i += len;
				
			}break;
		}
	}
	return out;
}

JP_Token *jp_get_token(JP_Token_chunk *chunk, u64 i)
{
	return chunk->tokens + i;
}

void jp_next_token(JP_Token_chunk **chunk, u64 *i)
{
	(*i)++;
	if(*i == TOKEN_CHUNK_SIZE)
	{
		*i = 0;
		(*chunk) = (*chunk)->next;
	}
}

void jp_print_tokens(JP_Token_chunk_list *list)
{
	JP_Token_chunk *chunk = list->first;
	for(u64 i = 0; i < list->num; i++)
	{
		for(u64 j = 0; j < chunk->num; j++)
		{
			JP_Token *cur = chunk->tokens + j;
			
			printf("%llu) %s ", j, jp_tokenkind_str[(s32)cur->kind]);
			printf("%.*s", str8_varg(cur->lexeme));
			printf("\n");
		}
		chunk = chunk->next;
	}
}