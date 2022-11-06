#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "allocator.h"

size_t strhash(const char *ptr)
{
    size_t size = strlen(ptr);
    printf("%ld str hash size", size);
    size_t hash = 0;
    const size_t prime = 31;
    for (size_t i = 0; i < size; ++i) {
        hash = ptr[i] + (hash * prime);
    }
    return hash;
}

static void
test_fill(char *c, size_t size)
{
    char *str[size+1];
    size_t i = 0;

    while (size--) {
        int r = rand();
        *c++ = (char)r;
        *str[i] = (char)r;
        i++;
    }
    printf("%ld\n", strhash(c));
}

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

void test(void) 
{
    printf("\n************************\n");

    mem_show("test start");
    void *ptr_10 = buf_alloc(12);
    if (buf_alloc(12) != NULL) {
        mem_show("buf_alloc(12)");
        strhash(ptr_10);
    }
    
}
