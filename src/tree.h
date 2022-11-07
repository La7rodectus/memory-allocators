#include <stddef.h>
#include "allocator_impl.h"

#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))

struct node
{
  size_t key;
  int height;
  void *ptr;
  struct node *left;
  struct node *right;
  struct node *depth;
};

#define NODE_STRUCT_SIZE ROUND_BYTES(sizeof(struct node))

void test_tree();
struct node* avl_insert(struct node*, size_t, void*);
struct node* avl_get_free(struct node*, size_t);
struct node* avl_remove_node(struct node*, size_t);
void avl_print(struct node*);

static inline int
node_get_height(struct node *n)
{
  return n == NULL ? 0 : n->height;
}

static inline struct node* 
node_init(size_t key, void* ptr)
{
  struct node* node = (struct node*)malloc(NODE_STRUCT_SIZE);
  node->key = key;
  node->left = NULL;
  node->right = NULL;
  node->depth = NULL;
  node->height = 1;
  node->ptr = ptr;
  return node;
}

static inline void
node_print(struct node *n)
{
  printf("+++++++ \n key = %ld \n left = %p \n right = %p \n depth = %p \n height = %d \n ptr = %p \n+++++++ \n", 
  n->key, (void *)n->left, (void *)n->right, (void *)n->depth, n->height, (void *)n->ptr);
}

