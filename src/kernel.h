#include <stdlib.h>
#define MAP_ANONYMOUS 0x20

void *kernel_mem_alloc(size_t);
void kernel_mem_free(char *, size_t);
