#include <stdlib.h>
#include <stdio.h>

#include "kernel.h"

static _Noreturn void
failed(void)
{
    fprintf(stderr, "System call for memory free failed\n");
    exit(EXIT_FAILURE);
}

#if defined(_IS_WIN)

void *
kernel_mem_alloc(size_t size)
{
    return VirtualAlloc(NULL, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

void
kernel_mem_free(void *ptr, size_t size)
{
    (void)size;
    if (VirtualFree(ptr, 0, MEM_RELEASE) == 0)
        failed();
}

#else

void *
kernel_mem_alloc(size_t size)
{
    void *ptr;

    ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    return ptr != MAP_FAILED ? ptr : NULL;
}

void
kernel_mem_free(void *ptr, size_t size)
{
    if (munmap(ptr, size) < 0)
        failed();
}

#endif
