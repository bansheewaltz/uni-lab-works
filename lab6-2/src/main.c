#include <stdlib.h>

#include "BTree.h"

int main(void)
{
  int degree = 0;
  if (scanf("%i", &degree) != 1) {
    return 0;
  }
  int num_of_keys = 0;
  if (scanf("%i", &num_of_keys) != 1) {
    return 0;
  }

  if (num_of_keys == 0) {
    printf("0");
    return 0;
  }

  BTree_t *tree = calloc(1, sizeof(BTree_t));
  tree->degree = degree;

  Node_t *node = node_create(tree->degree);
  tree->node = node;
  int cur = 0;
  for (int i = 0; i < num_of_keys; ++i) {
    if (scanf("%i", &cur) != 1) {
      tree_destroy(tree->node);
      free(tree);
      return 0;
    }
    tree_insert(tree, cur);
  }
  printf("%i", tree->height + 1);

  tree_destroy(tree->node);
  free(tree);

  return 0;
}
