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

struct T {
    void *ptr;
    size_t size;
    unsigned int checksum;
};

static unsigned int
buf_checksum(unsigned char *c, size_t size)
{
    unsigned int checksum = 0;

    while (size--)
        checksum = (checksum << 5) ^ (checksum >> 3) ^ *c++;
    return checksum;
}


static void
tester(void)
{
    const size_t SZ_MAX = 1000;
    const size_t SZ_MIN = 10;
    const unsigned long M = 1000;
    const size_t N = 10;
    struct T t[N];
    void *ptr;
    size_t idx, size, size_min;
    unsigned int checksum;

    for (idx = 0; idx < N; ++idx)
        t[idx].ptr = NULL;
    for (unsigned long i = 0; i < M; ++i) {
        printf("\nTEST NUMBER [%ld]\n", i);
        mem_show("mem_alloc & mem_realloc----------------------------------------------");
        for (idx = 0; idx < N; ++idx)
            if (t[idx].ptr != NULL) {
                if (t[idx].checksum != buf_checksum(t[idx].ptr, t[idx].size)) {
                    printf("1. Checksum check failed at [%p]\n", t[idx].ptr);
                    return;
                }
            }
        idx = (size_t)rand() % N;
        if (t[idx].ptr == NULL) {
            size = ((size_t)rand() % (SZ_MAX - SZ_MIN)) + SZ_MIN;
            ptr = buf_alloc(size);
            if (ptr != NULL) {
                t[idx].ptr = ptr;
                t[idx].size = size;
                t[idx].checksum = buf_checksum(ptr, size);
            }
        } else {
            if (rand() & 1) {
                size = ((size_t)rand() % (SZ_MAX - SZ_MIN)) + SZ_MIN;
                size_min = size < t[idx].size ? size : t[idx].size;
                checksum = buf_checksum(t[idx].ptr, size_min);
                ptr = mem_realloc(t[idx].ptr, size);
                if (ptr != NULL) {
                    if (checksum != buf_checksum(ptr, size_min)) {
                        printf("2. Checksum check failed at [%p]\n", ptr);
                        return;
                    }
                    t[idx].ptr = ptr;
                    t[idx].size = size;
                    t[idx].checksum = buf_checksum(ptr, size);
                }
            } else {
                mem_free(t[idx].ptr);
                t[idx].ptr = NULL;
            }
        }
    }
    for (size_t idx = 0; idx < N; ++idx) {
        if (t[idx].ptr != NULL) {
            mem_show("mem_free----------------------------------------------");
            if (t[idx].checksum != buf_checksum(t[idx].ptr, t[idx].size)) {
                printf("3. Checksum check failed at [%p]\n", t[idx].ptr);
                return;
            }
            mem_free(t[idx].ptr);
            t[idx].ptr = NULL;
        }
    }
    mem_show("DONE----------------------------------------------");
}


int
main(void)
{
    mem_show("arena size");
    
    void *ptr_1 = buf_alloc(1);
    void *ptr_50_1 = buf_alloc(50);
    void *ptr_50_2 = buf_alloc(50);
    void *ptr_20 = buf_alloc(20);
    mem_show("mem_alloc(1, 50, 20)");
    
    ptr_50_1 = mem_realloc(ptr_50_1, 1);
    mem_show("mem_realloc(ptr_50_1, 1)");

    mem_free(ptr_50_2);
    mem_show("mem_free(50(1))");
    mem_free(ptr_50_1);
    mem_show("mem_free(50(2))");

    void* ptr_150 = buf_alloc(150);
    mem_show("buf_alloc(150);");

    ptr_150 = mem_realloc(ptr_150, 20);
    mem_show("mem_realloc(ptr_150, 20)");

    ptr_150 = mem_realloc(ptr_150, 170);
    mem_show("mem_realloc(ptr_150, 170)");
    
    arena_free();
    mem_show("arena_free()");

    // arena_alloc(12);
    // mem_show("arena_alloc(12)");
    
    // void *ptr_12 = buf_alloc(12);
    // mem_show("buf_alloc(12)");
    // mem_free(ptr_12);
    // mem_show("mem_free(ptr_12)");
    
    // test_tree();
    // tester();
}

