#ifndef DECODING_BITSTRING_H_
#define DECODING_BITSTRING_H_

#include <stdint.h>
#include <stdio.h>
#include <tree.h>

int read_alphabet_size_bitstring(FILE *input);
uint32_t read_file_size_bitstring(FILE *input);
TreeNode *read_huffman_tree_text_form(int alph_size, FILE *input);
uchar read_char_binary_text_form(FILE *input);
void preorder_traversal_read_text_form(TreeNode *root, int alph_size,
                                       FILE *input, int depth);
void decoding_bitstring(CodingInfo *codingInfo, FILE *input, FILE *output);
void print_bitstring_decoded_file(TreeNode *tree_root, size_t file_size,
                                  FILE *input, FILE *output);

#endif  // DECODING_BITSTRING_H_
