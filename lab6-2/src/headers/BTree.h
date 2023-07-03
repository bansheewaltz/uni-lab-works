#ifndef BTREE
#define BTREE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  int *keys;
  struct Node **children;
  int numOfKeys;
  bool isLeaf;
} Node_t;

typedef struct BTree {
  Node_t *node;
  int degree;
  int height;
} BTree_t;

void tree_insert(BTree_t *tree, int cur);
void tree_destroy(Node_t *node);
Node_t *node_create(int degree);

#endif
