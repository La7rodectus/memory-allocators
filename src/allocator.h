// #include <stdlib.h>

int arena_alloc(size_t);
void arena_free(void);

void *mem_alloc(size_t);
void mem_free(const char *);
void *mem_realloc(const char *, size_t);
void mem_show(const char *);
