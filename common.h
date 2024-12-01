#pragma once

#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define ISDBG 1
#else
#define ISDBG 0
#endif

#define NONNULL(...) __attribute__((nonnull(__VA_ARGS__)))

void _die(const char *fmt, ...);

// Exits program and prints the error
#define die(fmt, ...) _die("%s:%d \x1b[1;31merror: %s(): \x1b[0m" fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define pdie(fmt, ...) _die("%s:%d \x1b[1;31merror: %s(): \x1b[0m" fmt ": \x1b[0m%s\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__, strerror(errno))
#define sdldie(fmt, ...) _die("%s:%d \x1b[1;31merror: %s(): \x1b[0m" fmt ": \x1b[0m%s\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__, SDL_GetError())

void* xmalloc(size_t size);
NONNULL(1) void* xrealloc(void* ptr, size_t size);
NONNULL(1, 2) FILE* xfopen(const char* file_path, const char* mode);

