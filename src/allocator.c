#include <stdio.h>
#include <stdlib.h>

#include "block.h"
#include "config.h"
#include "kernel.h"

#define ARENA_SIZE (ALLOCATOR_PAGE_SIZE * ALLOCATOR_ARENA_PAGES)

static struct block *arena = NULL;

static int
arena_alloc(void)
{
    arena = kernel_mem_alloc(ARENA_SIZE);
    if (arena == NULL)
        return -1;
    arena_init(arena, ARENA_SIZE - BLOCK_STRUCT_SIZE);
    return 0;
}

void *
mem_alloc(size_t size)
{
    struct block *block;

    if (arena == NULL)
        if (arena_alloc() < 0)
            return NULL;
    
    size = ROUND_BYTES(size);    
    
    for (block = arena;; block = block_next(block)) {
        if (!block_get_flag_busy(block) && block_get_size_curr(block) >= size) {
            block_split(block, size);
            return block_to_payload(block);
        }
        if (block_get_flag_last(block))
            break;
    }
    return NULL;
}

void
mem_free(void *ptr)
{
    struct block *block = (struct block *)payload_to_block(ptr);    
    block_clr_flag_busy(block);
    
    //exit if 1 block in general
    if (block_get_flag_first(block) && block_get_flag_last(block)) return;

    block_expand(block);
}

void *
mem_realloc(void *ptr, size_t size) 
{
    struct block *block = (struct block *)payload_to_block(ptr);    
    size = ROUND_BYTES(size);

    printf("realloc block %ld to %ld\n", block_get_size_curr(block), size);
    
    if (block_get_size_curr(block) >= size) {
        block_split(block, size);
    } else {
        block = block_expand(block);
        if (block_get_size_curr(block) < size) {
            mem_free(block_to_payload(block));
            return mem_alloc(size);
        } else {
            block_split(block, size);
        }
    }
    return block_to_payload(block);

}


void
mem_show(const char *msg)
{
    const struct block *block;

    printf("%s:\n", msg);
    if (arena == NULL) {
        printf("Arena was not created\n");
        return;
    }

    printf("%-20s\t%-20s\t%-10s%s\n", "is_busy", " block_size", " prev_block_size", " is_first");

    for (block = arena;; block = block_next(block)) {
        printf("%s %30zu %28zu%s%s\n",
            block_get_flag_busy(block) ? "busy" : "free",
            block_get_size_curr(block), block_get_size_prev(block),
            block_get_flag_first(block) ? " first" : "",
            block_get_flag_last(block) ? " last" : "");
        
        if (block_get_flag_last(block)) break;
    }
}
