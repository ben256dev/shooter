#pragma once

#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linear.h"

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

typedef struct {
    i32 x, y;
} ivec2;
typedef struct {
    i32 x, y, z;
} ivec3;
typedef struct {
    i32 x, y, z, w;
} ivec4;

#define arrlen(arr) (sizeof(arr) / sizeof((arr)[0]))

#ifdef NDEBUG
#define ISDBG 1
#else
#define ISDBG 0
#endif

#define DEG2RAD(D) ((D) / 180.0f * M_PI)

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
#define log(fmt, ...)                                                                              \
    SDL_Log("%s:%d \x1b[1;31minfo: %s(): \x1b[0m" fmt "\n", __FILE__, __LINE__, __func__,          \
        ##__VA_ARGS__)
#define warn(fmt, ...)                                                                             \
    SDL_LogWarn(0, "%s:%d \x1b[1;31minfo: %s(): \x1b[0m" fmt "\n", __FILE__, __LINE__, __func__,   \
        ##__VA_ARGS__)

u8* load_file_bytes(const char* path, usize* len);
char* load_file_string(const char* path);

void* xmalloc(size_t size);
NONNULL(1) void* xrealloc(void* ptr, size_t size);
NONNULL(1, 2) FILE* xfopen(const char* file_path, const char* mode);
