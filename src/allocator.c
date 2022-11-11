#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "block.h"
#include "config.h"
#include "kernel.h"
#include "tree.h"

static struct block *arena = NULL;
static size_t arena_size = 0;
static struct node *tree = NULL;

void arena_free()
{
  kernel_mem_free((char *)arena, arena_size);
  avl_mem_free(tree);
  tree = NULL;
  arena = NULL;
  arena_size = 0;
}

int arena_alloc(size_t size)
{
  if (arena)
    arena_free();

  size = ROUND_PAGES(size);
  arena = kernel_mem_alloc(size);
  if (arena == NULL)
    return -1;

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
  printf("******************************************mem_alloc %ld\n", size);
  n = avl_get_free(tree, size);
  if (n == NULL)
    return NULL;
  block = (struct block *)n->ptr;

  printf("avl_remove_with_ptr with size [%ld]\n", block_get_size_curr(block));
  printf("***TREE RM NODE [%ld]*** \n", block_get_size_curr(block));
  tree = avl_remove_with_ptr(tree, block_get_size_curr(block), block);
  avl_print(tree);
  block2 = block_split(block, size);
  avl_print(tree);
  if (block2 != NULL)
  {
    tree = avl_insert(tree, block_get_size_curr(block2), block2);
    printf("***TREE ADD NEW NODE [%ld]*** \n", block_get_size_curr(block2));
    avl_print(tree);
  }

  return block_to_payload(block);
}

void mem_free(void *ptr)
{
  struct block *block = (struct block *)payload_to_block(ptr);
  if (block == NULL)
    return;
  printf("***********************************mem_free %ld\n", block_get_size_curr(block));

  block_clr_flag_busy(block);
  struct block *next_block = block_next(block);
  if (next_block != NULL && !block_get_flag_busy(next_block))
  {
    printf("***TREE REMOVE NODE [%ld]*** \n", block_get_size_curr(next_block));
    tree = avl_remove_with_ptr(tree, block_get_size_curr(next_block), next_block);
    block_expand(block);
  }

  tree = avl_insert(tree, block_get_size_curr(block), block);
  printf("***TREE ADD NODE [%ld]*** \n", block_get_size_curr(block));
  avl_print(tree);
  // return mem to kernel if 1 block in overall
  if (block_get_flag_first(block) && block_get_flag_last(block))
  {
    arena_free();
  }
}

void *
mem_realloc(void *ptr, size_t size)
{
  struct block *block = (struct block *)payload_to_block(ptr);
  size = ROUND_BYTES(size);

  printf("**************************************mem_realloc block %ld to %ld\n", block_get_size_curr(block), size);

  if (block_get_size_curr(block) >= size)
  {
    struct block *block2 = NULL;

    block2 = block_split(block, size);
    if (block2 != NULL)
    {
      tree = avl_insert(tree, block_get_size_curr(block2), block2);
      printf("***TREE ADD NEW NODE [%ld]*** \n", block_get_size_curr(block2));
      avl_print(tree);
    }
  }
  else
  {
    struct block *next_block = block_next(block);
    if (next_block != NULL && !block_get_flag_busy(next_block))
    {
      printf("***TREE REMOVE NODE [%ld]*** \n", block_get_size_curr(next_block));
      tree = avl_remove_with_ptr(tree, block_get_size_curr(next_block), next_block);
      block = block_expand(block);
    }
    if (block_get_size_curr(block) < size)
    {
      mem_free(block_to_payload(block));
      void *p = mem_alloc(size);
      memcpy(p, block_to_payload(block), block_get_size_curr(block));
      return p;
    }
  }

  return block_to_payload(block);
}

void mem_show(const char *msg)
{
  const struct block *block;

  printf("%s:\n", msg);
  if (arena == NULL)
  {
    printf("Arena was not created\n");
    return;
  }

  printf("%-20s\t%-20s\t%-10s%s\n", "is_busy", " block_size", " prev_block_size", " is_first");

  for (block = arena;; block = block_next(block))
  {
    printf("%s %30zu %28zu%s%s\n",
           block_get_flag_busy(block) ? "busy" : "free",
           block_get_size_curr(block), block_get_size_prev(block),
           block_get_flag_first(block) ? " first" : "",
           block_get_flag_last(block) ? " last" : "");

    if (block_get_flag_last(block))
      break;
  }

  printf("***TREE***\n");
  avl_print(tree);
}
