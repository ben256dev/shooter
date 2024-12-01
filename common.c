#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include <SDL3/SDL.h>

#include "common.h"

void _die(const char *fmt, ...) {
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

