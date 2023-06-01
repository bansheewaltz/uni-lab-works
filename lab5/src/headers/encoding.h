#ifndef ENCODING_H_
#define ENCODING_H_

#include <stdio.h>

#include "tree.h"

void serialize_huffman_tree(TreeNode *tree, size_t alph_size, FILE *output);
void preorder_traversal_serialization(TreeNode *root, FILE *output);

#endif  // ENCODING_H_
