#include <stdio.h>
#include <unistd.h>

#include "allocator.h"
#include "allocator_impl.h"
#include "block.h"
#include "tree.h"

static void
buf_fill(char *c, size_t size)
{
    while (size--) {
        int r = rand();
        *c++ = (char)r;
    }

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
    mem_show("arena size");
    
    void *ptr_1 = buf_alloc(1);
    void *ptr_50 = buf_alloc(50);
    void *ptr_20 = buf_alloc(20);
    mem_show("mem_alloc(1, 50, 20)");
    
    ptr_50 = mem_realloc(ptr_50, 1);
    mem_show("mem_realloc(ptr_40, 1)");

    mem_free(ptr_50);
    mem_show("mem_free(50(1))");
    
    mem_realloc(ptr_1, 20);
    mem_show("mem_realloc(ptr_1, 20)");

    buf_alloc(47);
    mem_show("buf_alloc(47)");
    
    arena_free();
    mem_show("arena_free()");

    arena_alloc(12);
    mem_show("arena_alloc(12)");
    
    void *ptr_12 = buf_alloc(12);
    mem_show("buf_alloc(12)");
    mem_free(ptr_12);
    mem_show("mem_free(ptr_12)");
    
    tree();
}
