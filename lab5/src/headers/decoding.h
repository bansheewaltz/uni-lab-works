#ifndef DECODING_H_
#define DECODING_H_

#include <stdlib.h>

#include "stdio.h"
#include "tree.h"
#include "typedefs.h"

void decoding(CodingInfo *codingInfo, FILE *input, FILE *output);
void print_decoded_file(CodingInfo *coding_info, FILE *input, FILE *output);
uint32_t read_file_size_binary(FILE *input);
TreeNode *huffman_tree_read_binary(FILE *input);
uchar deserialize_char(FILE *input);

#endif  // DECODING_H_
