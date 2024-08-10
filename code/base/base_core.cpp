void* _arena_alloc(Arena* arena, size_t size)
{
	u64 pos_mem = AlignPow2(arena->used, arena->align);
	u64 pos_new = pos_mem + size;
	
	if(arena->res < pos_new)
	{
		// TODO(mizu): deal with reserving more (chain arenas)
		INVALID_CODE_PATH();
	}
	
	if(arena->cmt < pos_new)
	{
		u64 cmt_new_aligned, cmt_new_clamped, cmt_new_size;
		
		cmt_new_aligned = AlignPow2(pos_new, ARENA_COMMIT_SIZE);
		cmt_new_clamped = ClampTop(cmt_new_aligned, arena->res);
		cmt_new_size    = cmt_new_clamped - arena->cmt;
		os_commit((u8*)arena + arena->cmt, cmt_new_size);
		arena->cmt = cmt_new_clamped;
	}
	
	void *memory = 0;
  
  if (arena->cmt >= pos_new) 
	{
    memory = (u8*)arena + pos_mem;
    arena->used = pos_new;
	}
	
	return memory;
}

Arena_temp arena_temp_begin(Arena *arena)
{
	Arena_temp out = {
		.arena = arena,
		.pos = arena->used,
	};
	return out;
}

void arena_temp_end(Arena_temp *temp)
{
	for(u32 i = temp->pos; i < temp->arena->used; i ++)
	{
		u8 *base = (u8*)temp->arena;
		base[i] = 0;
	}
	temp->arena->used = temp->pos;
}

Arena *arena_create()
{
	return arena_create(ARENA_COMMIT_SIZE, ARENA_RESERVE_SIZE);
}

Arena *arena_create(u64 cmt, u64 res)
{
	Arena *arena = 0;
	
	void *memory = os_reserve(res);
	os_commit(memory, cmt);
	
	arena = (Arena*)memory;
	arena->used = ARENA_HEADER_SIZE;
	arena->align = DEFAULT_ALIGN;
	
	arena->cmt = AlignPow2(cmt, os_get_page_size());
	arena->res = res;
	
	return arena;
}

void mem_cpy(void *dst, void *src, size_t size)
{
	u8 *_dst = (u8*)dst;
	u8 *_src = (u8*)src;
	
	for(u32 i = 0; i < size; i ++)
	{
		_dst[i] = _src[i];
	}
}