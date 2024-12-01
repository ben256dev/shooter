#pragma once

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef float f32;
typedef double f64;
typedef size_t usize;
typedef intptr_t iptr;
typedef uintptr_t uptr;

#define arrlen(arr) (sizeof(arr) / sizeof((arr)[0]))

#ifdef NDEBUG
#define ISDBG 1
#else
#define ISDBG 0
#endif

#define NONNULL(...) __attribute__((nonnull(__VA_ARGS__)))

void _die(const char* fmt, ...);

// Exits program and prints the error
#define die(fmt, ...)                                                                              \
    _die("%s:%d \x1b[1;31merror: %s(): \x1b[0m" fmt "\n", __FILE__, __LINE__, __func__,            \
        ##__VA_ARGS__)
#define pdie(fmt, ...)                                                                             \
    _die("%s:%d \x1b[1;31merror: %s(): \x1b[0m" fmt ": \x1b[0m%s\n", __FILE__, __LINE__, __func__, \
        ##__VA_ARGS__, strerror(errno))
#define sdldie(fmt, ...)                                                                           \
    _die("%s:%d \x1b[1;31merror: %s(): \x1b[0m" fmt ": \x1b[0m%s\n", __FILE__, __LINE__, __func__, \
        ##__VA_ARGS__, SDL_GetError())

void* xmalloc(size_t size);
NONNULL(1) void* xrealloc(void* ptr, size_t size);
NONNULL(1, 2) FILE* xfopen(const char* file_path, const char* mode);
