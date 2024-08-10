#define STB_SPRINTF_IMPLEMENTATION
#include "stb/stb_sprintf.h"

#include <stdint.h>

#include <stdio.h>
#include <base/base_context_cracking.h>

#if defined(OS_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>
#include <sys/mman.h>
#endif

#include <base/base_core.h>
#include <base/base_string.h>
#include <os/os.h>
#if defined(OS_WIN32)
#include <os/os_win32.cpp>
#else
#include <os/os_linux.cpp>
#endif
#include <base/base_tcxt.h>
#include <json_parser/json_lexer.h>
#include <json_parser/json_parser.h>
#include <json_parser/json_hasher.h>

#include <base/base_core.cpp>
#include <base/base_string.cpp>
#include <base/base_tcxt.cpp>
#include <json_parser/json_lexer.cpp>
#include <json_parser/json_parser.cpp>
#include <json_parser/json_hasher.cpp>