#ifndef TREE_H_
#define TREE_H_

#include <stdbool.h>
#include <stddef.h>

#include "typedefs.h"

struct TreeNode {
  uchar character;
  size_t freq;
  TreeNode *left, *right;
};

TreeNode *create_tree_node(uchar character, size_t freq);
TreeNode *build_huffman_tree(CharInfo **chars_info_array,
                             size_t alphabet_size);
bool is_node_leaf(TreeNode *node);
void destroy_tree(struct TreeNode *root);
void preoder_traversal_codes_scan(TreeNode *root, CharInfo **chars_info,
                                  uint8_t code[], size_t depth);
void scan_codes_from_huffman_tree(TreeNode *root, CharInfo **chars_info);

#endif  // TREE_H_
