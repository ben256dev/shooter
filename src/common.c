#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <SDL3/SDL.h>

#include "common.h"

void _die(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    SDL_LogMessageV(0, SDL_LOG_PRIORITY_CRITICAL, fmt, args);
    va_end(args);
    SDL_Quit();
}

void* xmalloc(size_t size)
{
    void* ptr = malloc(size);
    if (ptr == NULL)
        pdie("malloc(%zu)", size);
    return ptr;
}

NONNULL(1) void* xrealloc(void* ptr, size_t size)
{
    void* ptrn = realloc(ptr, size);
    if (ptrn == NULL)
        pdie("realloc(%zu)", size);
    return ptrn;
}

NONNULL(1, 2) FILE* xfopen(const char* file_path, const char* mode)
{
    FILE* fd = fopen(file_path, mode);
    if (fd == NULL)
        pdie("fopen(%s, %s)", file_path, mode);
    return fd;
}

u8* load_file_bytes(const char* path, usize* len)
{
    struct stat filestat;
    if (stat(path, &filestat) != 0)
        goto err;
    FILE* file = fopen(path, "rb");
    if (!file)
        goto err;
    u8* buf = xmalloc(filestat.st_size);
    fread(buf, 1, filestat.st_size, file);
    fclose(file);
    *len = filestat.st_size;
    return buf;
err:
    *len = 0;
    return NULL;
}

char* load_file_string(const char* path)
{
    struct stat filestat;
    if (stat(path, &filestat) != 0)
        return NULL;
    FILE* file = fopen(path, "rb");
    if (!file)
        return NULL;
    char* str = xmalloc(filestat.st_size + 1);
    fread(str, 1, filestat.st_size, file);
    str[filestat.st_size] = '\0';
    fclose(file);
    return str;
}
