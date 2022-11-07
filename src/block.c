#include <stdlib.h>
#include <stdio.h>
#include "block.h"

struct block*
block_split(struct block *block1, size_t size)
{
    struct block *block2 = NULL;
    size_t size_curr;

    size_curr = block_get_size_curr(block1);
    
    printf("block_split with size %ld split to %ld\n", size_curr, size);    
    printf("Min memory for split %ld\n", size + BLOCK_STRUCT_SIZE);

    if (size_curr > size + BLOCK_STRUCT_SIZE) {
        size_curr -= size + BLOCK_STRUCT_SIZE;
        block_set_size_curr(block1, size);
        block2 = block_next(block1);
        block_init(block2);
        block_set_size_curr(block2, size_curr);
        block_set_size_prev(block2, size);
        if (block_get_flag_last(block1)) {
            block_clr_flag_last(block1);
            block_set_flag_last(block2);
        } else {
            block_set_size_prev(block_next(block2), size_curr);
        }
    }
    
    block_set_flag_busy(block1);
    return block2;
}

struct block *
block_expand(struct block *block)
{

    struct block *tmp_block_ptr = block;

    if (!block_get_flag_first(block)) {
        for (tmp_block_ptr = block_prev(block);; tmp_block_ptr = block_prev(block)) {
            if (!block_get_flag_busy(tmp_block_ptr)) {
                block = block_merge(tmp_block_ptr, block);
            } else {
                break;
            }
            if (block_get_flag_first(block)) break;
        }
    }

    if (!block_get_flag_last(block)) {
        for (tmp_block_ptr = block_next(block);; tmp_block_ptr = block_next(block)) {
            if (!block_get_flag_busy(tmp_block_ptr)) {
                block = block_merge(block, tmp_block_ptr);
            } else {
                break;
            }
            if (block_get_flag_last(block)) break;
        }
    }

    return block;
}

// Merge two neighboring blocks.
struct block *
block_merge(struct block *block1, struct block *block2)
{
//block_get_flag_busy(block1) || 
    if (block_get_flag_busy(block2)) {
        printf("Can't merge non empty block!\n");
        return block1;
    }
    
    size_t block1_size = block_get_size_curr(block1);
    size_t block2_size = block_get_size_curr(block2) + BLOCK_STRUCT_SIZE;
    size_t new_size_curr = block1_size + block2_size;

    if (block_get_flag_last(block2)) {
        block_set_flag_last(block1);
    }
    
    block_set_size_curr(block1, new_size_curr);
    struct block *new_next_block = block_next(block1);
    
    if (!block_get_flag_last(block1)) {
        block_set_size_prev(new_next_block, block_get_size_curr(block1)); 
    }

    return block1;
}












