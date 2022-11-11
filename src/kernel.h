#include <stdlib.h>
#define MAP_ANONYMOUS 0x20
#include "config.h"

#if defined(_IS_WIN)
  #include <memoryapi.h>
#else
  #include <sys/mman.h>
#endif

void *kernel_mem_alloc(size_t);
void kernel_mem_free(void *, size_t);
