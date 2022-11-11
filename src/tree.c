// C program to avl_insert a node in AVL tree
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "block.h"
#include "kernel.h"

struct node* 
avl_find(struct node* n, size_t key)
{
  if(n == NULL) return NULL;
  if(key < n->key)
    return avl_find(n->left, key);
  else if(key > n->key)
    return avl_find(n->right, key);
  else return n;
}

struct node*
avl_node_find_ptr(struct node* n, void *ptr) {
  struct node* res_node = n;
  while (res_node != NULL && res_node->ptr != ptr) {
    node_print(n);
    res_node = res_node->depth;
  }
  return res_node;
}
 
// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct node *
right_rotate(struct node *y)
{
  struct node *x = y->left;
  struct node *T2 = x->right;

  // Perform rotation
  x->right = y;
  y->left = T2;

  // Update heights
  y->height = MAX(node_get_height(y->left), node_get_height(y->right)) + 1;
  x->height = MAX(node_get_height(x->left), node_get_height(x->right)) + 1;

  // Return new root
  return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct node *
left_rotate(struct node *x)
{
  struct node *y = x->right;
  struct node *T2 = y->left;

  // Perform rotation
  y->left = x;
  x->right = T2;

  //  Update heights
  x->height = MAX(node_get_height(x->left), node_get_height(x->right)) + 1;
  y->height = MAX(node_get_height(y->left), node_get_height(y->right)) + 1;

  // Return new root
  return y;
}

// Get Balance factor of node N
int
get_balance(struct node *N)
{
  if (N == NULL) return 0;
  return node_get_height(N->left) - node_get_height(N->right);
}

struct node*
_avl_insert(struct node* node, size_t key, void* ptr)
{
  if (key < node->key)
    node->left = avl_insert(node->left, key, ptr);
  else if (key > node->key)
    node->right = avl_insert(node->right, key, ptr);
  else // Equal keys are not allowed in BST
    return node;

  /* 2. Update height of this ancestor node */
  node->height = 1 + MAX(node_get_height(node->left), node_get_height(node->right));

  /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        unbalanced */
  int balance = get_balance(node);

  // If this node becomes unbalanced, then
  // there are 4 cases

  // Left Left Case
  if (balance > 1 && key < node->left->key)
    return right_rotate(node);

  // Right Right Case
  if (balance < -1 && key > node->right->key)
    return left_rotate(node);

  // Left Right Case
  if (balance > 1 && key > node->left->key)
  {
    node->left = left_rotate(node->left);
    return right_rotate(node);
  }

  // Right Left Case
  if (balance < -1 && key < node->right->key)
  {
    node->right = right_rotate(node->right);
    return left_rotate(node);
  }

  /* return the (unchanged) node pointer */
  return node;
}
// Recursive function to avl_insert a key in the subtree rooted
// with node and returns the new root of the subtree.
struct node*
avl_insert(struct node* node, size_t key, void* ptr)
{
  /* 1.  Perform the normal BST avl_insertion */
  if (node == NULL) return(node_init(key, ptr));

  struct node *dup = avl_find(node, key);
  if (dup != NULL)
  {
    struct node *new_node = node_init(key, ptr);
    new_node->depth = dup;
    new_node->left = dup->left;
    new_node->right = dup->right;
    new_node->height = dup->height;
    node = new_node;
  }
  return _avl_insert(node, key, ptr);
}

void 
avl_print(struct node* t)
{
  if (t == NULL) return;
  printf("%ld", t->key);

  if (t->left != NULL) printf("(L:%ld)",t->left->key);
  if (t->right != NULL) printf("(R:%ld)",t->right->key);
  if (t->ptr != NULL) printf("(ptr:%ld)", ((struct block*)t->ptr)->size_curr);
  struct node* tmp = t;
  while (tmp->depth != NULL) {
    if (tmp->ptr) printf("(d:%ld|ptr:%ld)", tmp->depth->key, ((struct block*)tmp->ptr)->size_curr);
    else printf("(d:%ld)", tmp->key);
    tmp = tmp->depth;
  }

  printf("\n");

  avl_print(t->left);
  avl_print(t->right);
}

void
avl_mem_free(struct node* root) {
  if (root == NULL) return;

  avl_mem_free(root->left);
  avl_mem_free(root->right);

  struct node* tmp = NULL;
 
  do {
    tmp = root->depth;
    kernel_mem_free(root, NODE_STRUCT_SIZE);
    root = tmp;
  } while (root != NULL);
  
}

struct node * 
avl_min_node(struct node* node)
{
  struct node* current = node;
  while (current->left != NULL)
  current = current->left;
  return current;
}

struct node* 
_avl_delete(struct node* root, size_t key)
{
  if (root == NULL) return root;
  if (key < root->key) root->left = _avl_delete(root->left, key);
  else if (key > root->key) root->right = _avl_delete(root->right, key);
  else {
    if ((root->left == NULL) || (root->right == NULL)) {
      struct node *temp = root->left ? root->left : root->right;
      if (temp == NULL) {
        temp = root;
        root = NULL;
      } else {
        *root = *temp;
      }

      kernel_mem_free((void*)temp, NODE_STRUCT_SIZE);
    } else {
      struct node* temp = avl_min_node(root->right);
      root->key = temp->key;
      root->ptr = temp->ptr;
      root->right = _avl_delete(root->right, temp->key);
    }
  }

  if (root == NULL) return root;
  root->height = 1 + MAX(node_get_height(root->left),
  node_get_height(root->right));
  int balance = get_balance(root);
  if (balance > 1 && get_balance(root->left) >= 0) return right_rotate(root);

  if (balance > 1 && get_balance(root->left) < 0) {
    root->left = left_rotate(root->left);
    return right_rotate(root);
  }

  if (balance < -1 && get_balance(root->right) <= 0) return left_rotate(root);
  if (balance < -1 && get_balance(root->right) > 0) {
    root->right = right_rotate(root->right);
    return left_rotate(root);
  }

  return root;
}

void
_avl_rm_dup(struct node* root, struct node *ptr) {
  if (root == NULL) return;
  if (root->depth == ptr) {
    struct node* tmp = root->depth;
    root->depth = tmp->depth;
    root->ptr = tmp->ptr;
    kernel_mem_free((void*)ptr, NODE_STRUCT_SIZE);
  } else {
    _avl_rm_dup(root->depth, ptr);
  }
}

struct node* 
avl_remove_with_ptr(struct node* root, size_t key, void* ptr) {
  if (root->left == NULL && root->right == NULL && root->depth == NULL) {
    if (root->key == key) {
      kernel_mem_free((void*)root, NODE_STRUCT_SIZE);
      return NULL;
    } else {
      return root;
    }
  }

  struct node* with_dup = avl_find(root, key);
  if (with_dup->depth != NULL) {
    struct node* curr_dup = avl_node_find_ptr(with_dup, ptr);
    _avl_rm_dup(with_dup, curr_dup);
    return root;
  } else {
    return _avl_delete(root, key);
  }
}


void
avl_best_feet(struct node *t, size_t size, struct node **best)
{
  if (t == NULL) return;
  if ((*best) == NULL) (*best) = t;

  size_t curr_diff;
  if (t->key >= size)
  {
    curr_diff = t->key - size;

    if (curr_diff == 0) {
      (*best) = t;
      return;
    }

    size_t prev_diff = (*best)->key - size;

    if (curr_diff < prev_diff) {
      (*best) = t;
    }

    avl_best_feet(t->left, size, best);
  } else 
  {
    avl_best_feet(t->right, size, best);
  }
}

struct node*
avl_get_free(struct node *t, size_t size)
{
  struct node *n = NULL;
  avl_best_feet(t, size, &n);
  if (n == NULL) {
    printf("best feet with size [%ld] not found! \n", size);
    return NULL;
  } else {
    printf("best feet with size [%ld]! \n", n->key);
    return n;
  }
}

void 
test_tree()
{
  struct node *root = NULL;
 
  /* Constructing tree given in the above figure */
  struct node *p1 = NULL;
  struct node *p2 = NULL;
  root = avl_insert(root, (size_t)40, p1);
  p1 = root;
  root = avl_insert(root, (size_t)42, NULL);
  p2 = root;
  root = avl_insert(root, (size_t)40, p2);
  avl_print(root);
  root = avl_remove_with_ptr(root, 40, p2);
  // root = avl_insert(root, 21, NULL);
  // root = avl_insert(root, 22, NULL);
  // root = avl_insert(root, 23, NULL);
  // root = avl_insert(root, 24, NULL);
  // root = avl_insert(root, 25, NULL);
  // root = avl_insert(root, 25, NULL);
  // root = avl_insert(root, 25, NULL);
  // root = avl_insert(root, 25, NULL);
  // printf("root key = %ld \n", root->key);
  // avl_print(root);
  // root = avl_insert(root, 25, NULL);

  // printf("HAVE %ld", n->key);
  printf("Preorder traversal of the constructed AVL tree is \n");
  avl_print(root);

}

