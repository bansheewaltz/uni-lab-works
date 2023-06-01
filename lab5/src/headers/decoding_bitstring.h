#ifndef DECODING_BITSTRING_H_
#define DECODING_BITSTRING_H_

#include <stdint.h>
#include <stdio.h>
#include <tree.h>

int read_alphabet_size_checked(FILE *input);
uint32_t read_file_size_checked(FILE *input);
TreeNode *read_huffman_tree_text_form(int alph_size, FILE *input);
uchar read_char_binary_text_form(FILE *input);
void preorder_traversal_read_text_form(TreeNode *root, int alph_size,
                                       FILE *input, int depth);

#endif  // DECODING_BITSTRING_H_
