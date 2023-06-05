#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "typedefs.h"

void preorder_traversal_printing(TreeNode *root, FILE *output,
                                 size_t *bits_printed);
void array_int_print(int array[], int len, FILE *output);
void array_char_print(uchar array[], int len, FILE *output);
void print_code_array(CharInfo *charInfo, FILE *output);
int huffman_tree_get_character_by_code(TreeNode *node, uchar *code,
                                       int code_len, int current_level);
void print_bitstring_decoded_file(TreeNode *tree, size_t file_size, FILE *input,
                                FILE *output);
void print_code(CharInfo *charInfo, FILE *output);
void print_coding_stats(CharInfo **chars_info_array, size_t alphabet_size,
                        FILE *output);
void print_codes_lexicographically(CharInfo **chars_info_array,
                                   size_t alphabet_size, FILE *output);
#endif  // OUTPUT_H_
