#ifndef SERIALIZATION_C_
#define SERIALIZATION_C_

#include <stdio.h>

#include "tree.h"

void serialize_huffman_tree(TreeNode *tree, size_t alph_size, FILE *output);

#endif  // SERIALIZATION_C_
