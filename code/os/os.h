/* date = August 7th 2024 7:05 am */

#ifndef OS_H
#define OS_H

global u64 total_cmt;
global u64 total_res;

function b32 os_commit(void *ptr, u64 size);
function void os_decommit(void *ptr, u64 size);
function void os_release(void *ptr, u64 size);
function u64 os_get_page_size();
function Str8 os_get_app_dir(Arena *arena);

#endif //OS_H
