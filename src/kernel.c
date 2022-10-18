#include <stdlib.h>

#include "kernel.h"

void *
kernel_mem_alloc(size_t size)
{
    return malloc(size);
}
