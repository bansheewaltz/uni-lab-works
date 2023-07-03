#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define LEFT 0
#define RIGHT 1

typedef struct Node Node_t;

enum Color {
  RED = 0,
  BLACK = 1
};

struct Node {
  int value;
  char color;
  Node_t *child[2];
};

Node_t *make_node(Node_t *n, int value, int color)
{
  n->value = value;
  n->color = color;
  n->child[LEFT] = NULL;
  n->child[RIGHT] = NULL;
  return n;
}

bool red(Node_t *node)
{
  if (node == NULL) {
    return false;
  }
  return (node->color == RED);
}

void color_flip(Node_t *node)
{
  node->color = node->color ^ 1;
  node->child[LEFT]->color = node->child[LEFT]->color ^ 1;
  node->child[RIGHT]->color = node->child[RIGHT]->color ^ 1;
}

Node_t *rotate(Node_t *node, bool dir)
{
  Node_t *temp = node->child[!dir];
  node->child[!dir] = temp->child[dir];
  temp->child[dir] = node;

  temp->color = node->color;
  node->color = RED;
  return temp;
}

Node_t *double_rotate(Node_t *node, bool dir)
{
  node->child[!dir] = rotate(node->child[!dir], !dir);
  return rotate(node, dir);
}

Node_t *insert_fix_up(Node_t *node, bool dir)
{
  // one of the children is red
  if (red(node->child[dir])) {
    // 1 : both child red
    if (red(node->child[!dir])) {
      if (red(node->child[dir]->child[dir]) || red(node->child[dir]->child[!dir])) {
        color_flip(node);
      }
    } else {
      // any child has 2 reds in a row
      if (red(node->child[dir]->child[dir])) {
        node = rotate(node, !dir);
      } else if (red(node->child[dir]->child[!dir])) {
        node = double_rotate(node, !dir);
      }
    }
  }
  return node;
}

Node_t *insert(Node_t *tree, Node_t *node)
{
  if (tree == NULL) {
    tree = node;
    return tree;
  }
  // left = 0, right = 1
  bool dir = (node->value > tree->value);

  tree->child[dir] = insert(tree->child[dir], node);

  tree = insert_fix_up(tree, dir);
  return tree;
}

int get_black_height(Node_t *node)
{
  int height = 0;
  while (node) {
    if (node->color == BLACK) {
      height++;
    }
    node = node->child[LEFT];
    if (!node) {
      height++;
    }
  }
  return height;
}

int main(void)
{
  int n = 0;
  if (scanf("%i", &n) == EOF) {
    return 0;
  }

  Node_t *tree = NULL;
  Node_t *nodes = malloc(n * sizeof(Node_t));

  for (int i = 0; i < n; ++i) {
    int value = 0;
    if (scanf("%i", &value) != 1) {
      free(nodes);
      return 0;
    }
    Node_t *n = make_node(&nodes[i], value, RED);
    tree = insert(tree, n);
    tree->color = BLACK;
  }
  int black_height = get_black_height(tree);
  printf("%i", black_height);

  free(nodes);
  return EXIT_SUCCESS;
}
