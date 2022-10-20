#include <stdlib.h>
#include <sys/mman.h>
#include "kernel.h"

void *
kernel_mem_alloc(size_t size)
{

    return malloc(size);

    // mmap(NULL, );
}

// void
// kernel_mem_free(char *ptr)
// {
//     return malloc(size);
// }
