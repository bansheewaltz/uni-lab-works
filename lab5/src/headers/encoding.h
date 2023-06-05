#ifndef ENCODING_H_
#define ENCODING_H_

#include <stdio.h>

#include "tree.h"

void encoding(CodingInfo *codingInfo, FILE *input, FILE *output);
void encode_input(TreeNode *tree, CodingInfo *codingInfo, FILE *input,
                  FILE *output);
void write_binary_array_to_bin(uint8_t *array, size_t code_len, FILE *output);
void write_encoded_input(CharInfo **chars_info_dictionary, FILE *input,
                         FILE *output);
void huffman_tree_preorder_traversal_writing(TreeNode *root, FILE *output);
void huffman_tree_write_encoded(TreeNode *tree, FILE *output);

#endif  // ENCODING_H_
