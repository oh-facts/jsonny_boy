void *os_reserve(u64 size)
{
	void *out = VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);
	if (out != NULL)
	{
		total_res += size;
	}
	return out;
}

b32 os_commit(void *ptr, u64 size)
{
	if (VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE) == NULL)
	{
		printf("VirtualAlloc commit failed: %lu\r\n", GetLastError());
		return 0;
	}
	total_cmt += size;
	
	return 1;
}

void os_decommit(void *ptr, u64 size)
{
	VirtualFree(ptr, size, MEM_DECOMMIT);
}

void os_release(void *ptr, u64 size)
{
	VirtualFree(ptr, 0, MEM_RELEASE);
}

u64 os_get_page_size()
{
	SYSTEM_INFO sysinfo = {};
	GetSystemInfo(&sysinfo);
	return sysinfo.dwPageSize;
}

Str8 os_get_app_dir(Arena *arena)
{
	char buffer[256];
	DWORD len = GetModuleFileName(0, buffer, 256);
	
	char *c = &buffer[len];
	while(*(--c) != '\\')
	{
		*c = 0;
		--len;
	}
  
	u8 *str = push_array(arena, u8, len);
	mem_cpy(str, buffer, len);
	
	Str8 out = str8(str, len);
	
	return out;
}