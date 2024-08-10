/* date = August 7th 2024 7:00 am */

#ifndef PLATFORM_H
#define PLATFORM_H

enum FILE_TYPE
{
  FILE_TYPE_TEXT,
  FILE_TYPE_BINARY,
  FILE_TYPE_COUNT
};

struct File_data
{
	u8 *bytes;
	u64 size;
};

// ty pine
#if defined(OS_WIN32)
#define _file_open(file, filepath, mode) fopen_s(file, filepath, mode)
#elif defined (OS_LINUX) || defined (OS_APPLE)
#define _file_open(file, filepath, mode) *file = fopen(filepath, mode)
#endif

function File_data read_file(Arena *arena, const char *filepath, FILE_TYPE type)
{
	File_data out = {};
	FILE *file;
	
	local_persist char *file_type_table[FILE_TYPE_COUNT] = 
	{
		"r",
		"rb"
	};
	
	_file_open(&file, filepath, file_type_table[type]);
	
	fseek(file, 0, SEEK_END);
	
	out.size = ftell(file);
	//print("%d", len);
	
	fseek(file, 0, SEEK_SET);
	
	out.bytes = push_array(arena, u8, out.size);
	fread(out.bytes, sizeof(u8), out.size, file);
	
	fclose(file);
	
	return out;
}

function void write_file(const char *filepath, FILE_TYPE type, void *data, size_t size)
{
	FILE *file;
	
	local_persist char *file_type_table[FILE_TYPE_COUNT] = 
	{
		"w",
		"wb"
	};
	
	_file_open(&file, filepath, file_type_table[type]);
	
	fwrite(data, size, 1, file);
	
	fclose(file);
	
}

#endif //PLATFORM_H