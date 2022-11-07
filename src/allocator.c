#include <stdio.h>
#include <stdlib.h>

#include "block.h"
#include "config.h"
#include "kernel.h"
#include "tree.h"

static struct block *arena = NULL;
static size_t arena_size = 0;
static struct node *tree = NULL;

void
arena_free() {
    kernel_mem_free((char *)arena, arena_size);
    arena = NULL;
    arena_size = 0;
}

int
arena_alloc(size_t size)
{
    if (arena) arena_free();

    size = ROUND_PAGES(size);
    arena = kernel_mem_alloc(size);
    if (arena == NULL) return -1;
    
    arena_size = size;
    arena_init(arena, size - BLOCK_STRUCT_SIZE);
    tree = avl_insert(tree, block_get_size_curr(arena), arena);
    return 0;
}


void *
mem_alloc(size_t size)
{
    struct block *block;
    struct block *block2;
    struct node *n;

    if (arena == NULL)
      if (arena_alloc(ARENA_SIZE) < 0)
        return NULL;
    
    size = ROUND_BYTES(size);    

    n = avl_get_free(tree, size);
    if (n == NULL) return NULL;
    block = (struct block*)n->ptr;

    block2 = block_split(block, size);
    tree = avl_remove_node(tree, block_get_size_curr(block));
    
    if (block2 != NULL) {
      tree = avl_insert(tree, block_get_size_curr(block2), block2);
      avl_print(tree);
    }
    return block_to_payload(block);
    // for (block = arena;; block = block_next(block)) {
    //     if (!block_get_flag_busy(block) && block_get_size_curr(block) >= size) {
    //         block_split(block, size);
    //         tree = avl_insert(tree, block_get_size_curr(block), block);
    //         return block_to_payload(block);
    //     }
    //     if (block_get_flag_last(block))
    //         break;
    // }
    return NULL;
}

void
mem_free(void *ptr)
{
    struct block *block = (struct block *)payload_to_block(ptr);    
    
    block_clr_flag_busy(block);
    block_expand(block);

    //return mem to kernel if 1 block in overall
    if (block_get_flag_first(block) && block_get_flag_last(block)) {
        arena_free();
    }
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
