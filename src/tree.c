// C program to avl_insert a node in AVL tree
#include <stdio.h>
#include <stdlib.h>
// #include "block.h"
#include "tree.h"

// A utility function to get the height of the tree
int
height(struct node *N)
{
  if (N == NULL) return 0;
  return N->height;
}
 

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

// A utility function to get maximum of two integers
// int 
// max(int a, int b)
// {
//   return (a > b)? a : b;
// }
 
/* Helper function that allocates a new node with the given key and
    NULL left and right pointers. */
struct node* 
init_node(size_t key, void* ptr)
{
  struct node* node = (struct node*)malloc(sizeof(struct node));
  node->key = key;
  node->left = NULL;
  node->right = NULL;
  node->depth = NULL;
  node->height = 1;
  node->ptr = ptr;
  return node;
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
  y->height = MAX(height(y->left), height(y->right)) + 1;
  x->height = MAX(height(x->left), height(x->right)) + 1;

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
  x->height = MAX(height(x->left), height(x->right)) + 1;
  y->height = MAX(height(y->left), height(y->right)) + 1;

  // Return new root
  return y;
}

// Get Balance factor of node N
int
get_balance(struct node *N)
{
  if (N == NULL) return 0;
  return height(N->left) - height(N->right);
}

// Recursive function to avl_insert a key in the subtree rooted
// with node and returns the new root of the subtree.
struct node*
avl_insert(struct node* node, size_t key, void* ptr)
{
  /* 1.  Perform the normal BST avl_insertion */
  if (node == NULL) return(init_node(key, ptr));

  struct node *dup = avl_find(node, key);
  if (dup != NULL)
  {
    printf("found dup with key = %ld \n", dup->key);
    if (dup->depth == NULL) dup->depth = init_node(key, ptr);
    else {
      struct node *tmp = dup->depth;
      dup->depth = init_node(key, ptr);
      dup->depth->depth = tmp;
    }
    dup->depth->height = dup->height;
    return node;
  }

  if (key < node->key)
    node->left = avl_insert(node->left, key, ptr);
  else if (key > node->key)
    node->right = avl_insert(node->right, key, ptr);
  else // Equal keys are not allowed in BST
    return node;

  /* 2. Update height of this ancestor node */
  node->height = 1 + MAX(height(node->left), height(node->right));

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
    node->left =  left_rotate(node->left);
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

void 
avl_print(struct node* t)
{
  if (t == NULL) return;
  printf("%ld",t->key);

  if (t->left != NULL) printf("(L:%ld)",t->left->key);
  if (t->right != NULL) printf("(R:%ld)",t->right->key);
  struct node* tmp = t;
  while (tmp->depth != NULL) {
    printf("(d:%ld)", tmp->depth->key);
    tmp = tmp->depth;
  }

  printf("\n");

  avl_print(t->left);
  avl_print(t->right);
}

struct node * 
avl_min_node(struct node* node)
{
  struct node* current = node;
  /* loop down to find the lftmost leaf */
  while (current->left != NULL)
  current = current->left;
  return current;
}

struct node* 
avl_delete(struct node* root, size_t key)
{
  
  struct node* dup = avl_find(root, key);
  if (dup != NULL) {
    dup->depth->left = dup->left;
    dup->depth->right = dup->right;
    struct node* tmp = dup->depth;
    *dup = *tmp;
    free(tmp);
    return root;
  }

  if (root == NULL) return root;
  if (key < root->key) root->left = avl_delete(root->left, key);
  else if (key > root->key) root->right = avl_delete(root->right, key);
  else {
    if ((root->left == NULL) || (root->right == NULL)) {
      struct node *temp = root->left ? root->left : root->right;
      if (temp == NULL) {
        temp = root;
        root = NULL;
      } else {
        *root = *temp;
      }

      free(temp);
    } else {
      struct node* temp = avl_min_node(root->right);
      root->key = temp->key;
      root->right = avl_delete(root->right, temp->key);
    }
  }

  if (root == NULL) return root;
  root->height = 1 + MAX(height(root->left),
  height(root->right));
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
avl_best_feet(struct node *t, size_t size, struct node **best)
{
  if (t == NULL) {
    (*best) = NULL;
    return;
  }
  if ((*best) == NULL) (*best) = t;

  printf("current t key = %ld \n", t->key);
  printf("(*best)->key = %ld \n", (*best)->key);
  size_t curr_diff;
  if (t->key >= size)
  {
    curr_diff = t->key - size;
    printf("curr_diff = %ld \n", curr_diff);

    if (curr_diff == 0) {
      (*best) = t;
      printf("found best feet = %ld \n", (*best)->key);
      return;
    }

    size_t prev_diff = (*best)->key - size;
    printf("prev_diff = %ld \n", prev_diff);

    if (curr_diff < prev_diff) {
      (*best) = t;
      printf("set new best feet = %ld \n", (*best)->key);
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
  struct node *best_feet = NULL;
  avl_best_feet(t, size, &best_feet);
  if (best_feet == NULL) {
    printf("best feet with size [%ld] not found! \n", size);
    return NULL;
  } else {
    printf("best feet with size [%ld]! \n", best_feet->key);
    return best_feet;
  }
}

void 
tree()
{
  struct node *root = NULL;
 
  /* Constructing tree given in the above figure */
  root = avl_insert(root, 40, NULL);
  root = avl_insert(root, 20, NULL);
  root = avl_insert(root, 21, NULL);
  root = avl_insert(root, 22, NULL);
  root = avl_insert(root, 23, NULL);
  root = avl_insert(root, 24, NULL);
  root = avl_insert(root, 25, NULL);
  printf("root key = %ld \n", root->key);
  root = avl_insert(root, 25, NULL);
  printf("root key = %ld \n", root->key);
  root = avl_insert(root, 25, NULL);
  root = avl_insert(root, 25, NULL);
  printf("root key = %ld \n", root->key);
  root = avl_delete(root, 25);
  // root = avl_insert(root, 25, NULL);
  struct node* n = avl_find(root, 24);
  if (n == NULL) {
    printf("not found");
  } else {
    printf("found key %ld \n", n->key);
  }


  printf("Preorder traversal of the constructed AVL tree is \n");
  avl_print(root);

}

