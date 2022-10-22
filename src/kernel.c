#include <stdlib.h>
#include <sys/mman.h>
#include "kernel.h"

void *
kernel_mem_alloc(size_t size)
{  
    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, (__off_t)0); //| MAP_ANONYMOUS
}

void
kernel_mem_free(char *ptr, size_t size)
{
    munmap(ptr, size);
}
