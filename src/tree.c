// C program to avl_insert a node in AVL tree
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

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
    printf("found dup with key = %ld \n", dup->key);
    if (dup->depth == NULL) dup->depth = node_init(key, ptr);
    else {
      struct node *tmp = dup->depth;
      dup->depth = node_init(key, ptr);
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

      free(temp);
    } else {
      struct node* temp = avl_min_node(root->right);
      root->key = temp->key;
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


struct node* 
avl_remove_node(struct node* root, size_t key) {
  if (root->left == NULL && root->right == NULL && root->depth == NULL) {
    free(root);
    return NULL;
  }
  struct node* dup = avl_find(root, key);
  if (dup->depth != NULL) {
    dup->depth->left = dup->left;
    dup->depth->right = dup->right;
    struct node* tmp = dup->depth;
    *dup = *tmp;
    free(tmp);
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

  printf("current t key = %ld \n", t->key);
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
  root = avl_insert(root, 40, NULL);
  root = avl_insert(root, 40, NULL);
  // root = avl_insert(root, 21, NULL);
  // root = avl_insert(root, 22, NULL);
  // root = avl_insert(root, 23, NULL);
  // root = avl_insert(root, 24, NULL);
  // root = avl_insert(root, 25, NULL);
  // root = avl_insert(root, 25, NULL);
  // root = avl_insert(root, 25, NULL);
  // root = avl_insert(root, 25, NULL);
  // printf("root key = %ld \n", root->key);
  root = avl_remove_node(root, 40);
   avl_print(root);
  root = avl_remove_node(root, 40);
  // root = avl_insert(root, 25, NULL);

  struct node *n = avl_get_free(root, 27);
  // printf("HAVE %ld", n->key);
  printf("Preorder traversal of the constructed AVL tree is \n");
  avl_print(root);

}

