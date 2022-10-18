#include <stdio.h>

#include "allocator.h"
#include "allocator_impl.h"
#include "block.h"

static void
buf_fill(char *c, size_t size)
{
    while (size--)
        *c++ = (char)rand();
}

static void *
buf_alloc(size_t size)
{
    void *ptr;

    ptr = mem_alloc(size);
    if (ptr != NULL)
        buf_fill(ptr, size);
    return ptr;
}

int
main(void)
{
    buf_alloc(1);
    mem_show("mem_alloc(1)");
    buf_alloc(20);
    buf_alloc(20);
    mem_show("mem_alloc(20)");
}
