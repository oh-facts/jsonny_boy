u64 str8_len(char *c)
{
	u64 out = 0;
	while(*c++)
	{
		out++;
	}
	return out;
}

Str8 str8(u8 *c, u64 len)
{
	Str8 out = 
	{
		c,len
	};
	return out;
}

void str8_cpy(Str8 *dst, Str8 *src)
{
	for(u32 i = 0; i < src->len; i ++)
	{
		dst->c[i] = src->c[i];
	}
}

Str8 push_str8f(Arena *arena, char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	Str8 result = push_str8fv(arena, fmt, args);
	va_end(args);
	return(result);
}

Str8 push_str8fv(Arena *arena, char *fmt, va_list args)
{
	Str8 out = {};
	va_list args_copy;
	va_copy(args_copy, args);
	
	int bytes_req = stbsp_vsnprintf(0, 0, fmt, args) + 1;
	
	out.c = push_array(arena, u8, bytes_req);
	
	out.len = stbsp_vsnprintf((char *)out.c, bytes_req, fmt, args_copy);
	va_end(args_copy);
	
	return out;
}

b32 str8_equals(Str8 a, Str8 b)
{
	b32 res = 1;
	
	if(a.len == b.len)
	{
		for(u32 i = 0; i < a.len; i++)
		{
			if(a.c[i] != b.c[i])
			{
				res = 0;
				break;
			}
		}
	}
	else
	{
		res = 0;
	}
	
	return res;
}

Str8 str8_join(Arena *arena, Str8 a, Str8 b)
{
	Str8 out = {};
	out.c = push_array(arena, u8, a.len + b.len);
	
	mem_cpy(out.c, a.c, a.len);
	
	mem_cpy((u8*)out.c + a.len, b.c, b.len);
	//printf("%s %lu\r\n", b.c, b.len);
	
	out.len = a.len + b.len;
	return out;
}
