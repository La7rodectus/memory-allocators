#include <stddef.h>

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

void tree();
struct node* avl_get_free(struct node*, size_t);


