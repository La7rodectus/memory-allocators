#include <stdbool.h>
#include <stdlib.h>

#include "allocator_impl.h"

struct block {
    size_t size_curr;
    size_t size_prev;
    bool flag_busy;
    bool flag_first;
    bool flag_last;
};

#define BLOCK_STRUCT_SIZE ROUND_BYTES(sizeof(struct block))

void
block_split(struct block *, size_t);

struct block *
block_merge(struct block *, struct block *);

struct block *
block_expand(struct block *);

static inline void *
block_to_payload(const struct block *block)
{
    return (char *)block + BLOCK_STRUCT_SIZE;
}

static inline void *
payload_to_block(char *block_payload)
{
    return (char *)block_payload - BLOCK_STRUCT_SIZE;
}

static inline size_t
block_get_size_curr(const struct block *block)
{
    return block->size_curr;
}

static inline void
block_set_size_curr(struct block *block, size_t size)
{
    block->size_curr = size;
}

static inline size_t
block_get_size_prev(const struct block *block)
{
    return block->size_prev;
}

static inline void
block_set_size_prev(struct block *block, size_t size)
{
    block->size_prev = size;
}

static inline bool
block_get_flag_busy(const struct block *block)
{
    return block->flag_busy;
}

static inline void
block_set_flag_busy(struct block *block)
{
    block->flag_busy = true;
}

static inline bool
block_get_flag_first(const struct block *block)
{
    return block->flag_first;
}

static inline void
block_set_flag_first(struct block *block)
{
    block->flag_first = true;
}

static inline bool
block_get_flag_last(const struct block *block)
{
    return block->flag_last;
}

static inline void
block_set_flag_last(struct block *block)
{
    block->flag_last = true;
}

static inline void
block_clr_flag_last(struct block *block)
{
    block->flag_last = false;
}

static inline void
block_clr_flag_busy(struct block *block)
{
    block->flag_busy = false;
}

static inline struct block *
block_next(const struct block *block)
{
    return (struct block *)
        ((char *)block + BLOCK_STRUCT_SIZE + block_get_size_curr(block));
}

static inline struct block *
block_prev(const struct block *block)
{
    return (struct block *)
        ((char *)block - BLOCK_STRUCT_SIZE - block_get_size_prev(block));
}

static inline void
block_init(struct block *block)
{
    block->flag_busy = false;
    block->flag_first = false;
    block->flag_last = false;
}

static inline void
arena_init(struct block *block, size_t size)
{
    block_init(block);
    block->size_curr = size;
    block->size_prev = 0;
    block_set_flag_first(block);
    block_set_flag_last(block);
}

















