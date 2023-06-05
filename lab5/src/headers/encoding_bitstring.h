#ifndef ENCODING_BITSTRING_H_
#define ENCODING_BITSTRING_H_

#include <stdio.h>

#include "typedefs.h"

void encode_input_text_form(TreeNode *tree, CodingInfo *codingInfo,
                            FILE *input, FILE *output);
void scan_chars_frequencies_from_input(CharInfo **dictionary, FILE *input);
size_t count_alphabet_size(CharInfo **dictionary);
void encoding_bitstring(CodingInfo *codingInfo, FILE *input, FILE *output);
void preorder_traversal_printing(TreeNode *root, FILE *output,
                                 size_t *bits_printed);
size_t print_huffman_tree_to_text(TreeNode *tree, FILE *output);
size_t print_encoded_file_to_text(CharInfo **chars_info_dictionary,
                                  FILE *input, FILE *output);

#endif  // ENCODING_BITSTRING_H_
