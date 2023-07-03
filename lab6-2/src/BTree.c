#include "BTree.h"

Node_t *node_create(int degree)
{
  Node_t *node = malloc(sizeof(Node_t));
  node->isLeaf = 1;
  node->numOfKeys = 0;
  node->keys = malloc((2 * degree - 1) * sizeof(int));
  node->children = NULL;
  return node;
}

void move_keys(Node_t *old, Node_t *new, int indexOld, int indexNew)
{
  int index = new->numOfKeys - indexNew;
  memmove(new->keys + indexNew, old->keys + indexOld, index * sizeof(int));
  if (old->isLeaf == 0) {
    memmove(new->children + indexNew, old->children + indexOld, (index + 1) * sizeof(Node_t *));
  }
}

void split(Node_t *node, int degree, int index)
{
  Node_t *old = node->children[index];
  Node_t *new = node_create(degree);
  if (old->isLeaf == 0) {
    new->isLeaf = 0;
    new->children = malloc((2 * degree) * sizeof(Node_t *));
  }
  new->numOfKeys = degree - 1;
  move_keys(old, new, degree, 0);
  old->numOfKeys = degree - 1;
  node->numOfKeys++;
  move_keys(node, node, index, index + 1);
  node->keys[index] = old->keys[degree - 1];
  node->children[index + 1] = new;
}

int get_index(Node_t *node, int cur)
{
  int left = 0;
  int right = node->numOfKeys - 1;
  while (left <= right) {
    int middle = (left + right) / 2;
    if (cur <= node->keys[middle]) {
      right = middle - 1;
    } else {
      left = middle + 1;
    }
  }
  return right + 1;
}

void insert_in_node(Node_t *node, int cur)
{
  int index = get_index(node, cur);
  node->numOfKeys++;
  move_keys(node, node, index, index + 1);
  node->keys[index] = cur;
}

void insert_non_full(Node_t *node, int degree, int cur)
{
  if (node->isLeaf == 1) {
    insert_in_node(node, cur);
  } else {
    int index = get_index(node, cur);
    if (node->children[index]->numOfKeys == (2 * degree - 1)) {
      split(node, degree, index);
    }
    insert_non_full(node->children[get_index(node, cur)], degree, cur);
  }
}

void tree_insert(BTree_t *tree, int cur)
{
  if (tree->node->numOfKeys == 2 * (tree->degree) - 1) {
    Node_t *new_node = node_create(tree->degree);
    new_node->children = malloc(2 * (tree->degree) * sizeof(Node_t));
    new_node->isLeaf = 0;

    new_node->children[0] = tree->node;
    split(new_node, tree->degree, 0);
    tree->node = new_node;
    tree->height++;
  }
  insert_non_full(tree->node, tree->degree, cur);
}

void tree_destroy(Node_t *node)
{
  free(node->keys);
  if (node->isLeaf == 1) {
    free(node);
    return;
  }
  for (int i = 0; i <= node->numOfKeys; ++i) {
    tree_destroy(node->children[i]);
  }
  free(node->children);
  free(node);
}
