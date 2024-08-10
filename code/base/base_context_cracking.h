/* date = August 6th 2024 10:51 pm */

#ifndef BASE_CONTEXT_CRACKING_H
#define BASE_CONTEXT_CRACKING_H

#if defined(_WIN32)
#define OS_WIN32
#elif defined (__linux__)
#define OS_LINUX
#elif defined(__APPLE__)
#define OS_APPLE
#endif

#if defined(__clang__)
#define COMPILER_CLANG
#elif defined(_MSC_VER)
#define COMPILER_MSVC
#elif defined(__GNUC__) || defined(__GNUG__)
#define COMPILER_GCC
#else
#error This compiler is not supported
#endif

#endif //BASE_CONTEXT_CRACKING_H
