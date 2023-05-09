#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  struct node *right_subtree;
  struct node *left_subtree;
  int val;
  uint8_t tree_height;
} AVLnode;

uint8_t get_tree_height(AVLnode *tree) {
  if (tree) {
    return tree->tree_height;
  }
  return 0;
}

int get_tree_balance_factor(AVLnode *tree) {
  return get_tree_height(tree->right_subtree) -
         get_tree_height(tree->left_subtree);
}

void set_subtree_height(AVLnode *tree) {
  uint8_t height_left = get_tree_height(tree->left_subtree);
  uint8_t height_right = get_tree_height(tree->right_subtree);

  tree->tree_height =
      (height_left > height_right ? height_left : height_right) + 1;
}

AVLnode *rotate_tree_right(AVLnode *tree) {
  AVLnode *left_subtree = tree->left_subtree;
  tree->left_subtree = left_subtree->right_subtree;
  left_subtree->right_subtree = tree;
  set_subtree_height(tree);
  set_subtree_height(left_subtree);

  return left_subtree;
}

AVLnode *rotate_tree_left(AVLnode *tree) {
  AVLnode *right_subtree = tree->right_subtree;
  tree->right_subtree = right_subtree->left_subtree;
  right_subtree->left_subtree = tree;
  set_subtree_height(tree);
  set_subtree_height(right_subtree);

  return right_subtree;
}

AVLnode *balance_tree(AVLnode *tree) {
  set_subtree_height(tree);
  if (get_tree_balance_factor(tree) == 2) {
    if (get_tree_balance_factor(tree->right_subtree) < 0) {
      tree->right_subtree = rotate_tree_right(tree->right_subtree);
    }
    return rotate_tree_left(tree);
  }
  if (get_tree_balance_factor(tree) == -2) {
    if (get_tree_balance_factor(tree->left_subtree) > 0) {
      tree->left_subtree = rotate_tree_left(tree->left_subtree);
    }
    return rotate_tree_right(tree);
  }

  return tree;
}

AVLnode *create_tree_node(AVLnode *tree_node, int value) {
  if (tree_node != NULL) {
    tree_node->val = value;
    tree_node->left_subtree = tree_node->right_subtree = NULL;
    tree_node->tree_height = 1;
  }

  return tree_node;
}

AVLnode *insert(AVLnode *tree, AVLnode *tree_node, int key) {
  if (!tree) {
    return create_tree_node(tree_node, key);
  }
  if (key < tree->val) {
    tree->left_subtree = insert(tree->left_subtree, tree_node, key);
  } else {
    tree->right_subtree = insert(tree->right_subtree, tree_node, key);
  }

  return balance_tree(tree);
}

int main(void) {
  int vertices_count = 0;
  if (!scanf("%d", &vertices_count)) {
    return 0;
  }

  if (vertices_count == 0) {
    puts("0");
    return 0;
  }

  AVLnode *tree = malloc(sizeof(AVLnode) * (size_t)vertices_count);
  if (!tree) {
    free(tree);
    return EXIT_SUCCESS;
  }

  int tmp = 0;
  if (!scanf("%d", &tmp)) {
    free(tree);
    return EXIT_SUCCESS;
  }
  tree[0].val = tmp;
  tree[0].tree_height = 1;
  tree[0].left_subtree = tree[0].right_subtree = NULL;

  AVLnode *head = &tree[0];

  for (int i = 1; i < vertices_count; i++) {
    if (!scanf("%d", &tmp)) {
      free(tree);
      return EXIT_SUCCESS;
    }
    head = insert(head, &tree[i], tmp);
  }
  printf("%" PRIu8 "", get_tree_height(head));
  free(tree);

  return EXIT_SUCCESS;
}
