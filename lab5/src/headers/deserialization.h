#ifndef DESERIALIZATION_H_
#define DESERIALIZATION_H_

#include <stdlib.h>

#include "stdio.h"
#include "tree.h"

TreeNode *deserialize_huffman_tree(int alph_size, FILE *input,
                                   uchar **characters_array);
uchar deserialize_char(FILE *input);
void preorder_traversal_deserialization(TreeNode *root, int alph_size,
                                        FILE *input, uchar *characters_array,
                                        int depth);

#endif  // DESERIALIZATION_H_
