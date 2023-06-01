#ifndef ENCODING_BITSTRING_H_
#define ENCODING_BITSTRING_H_

#include <stdio.h>

#include "typedefs.h"

void encode_input_text_form(TreeNode *tree, CodingInfo *codingInfo,
                            FILE *input, FILE *output);
void scan_chars_frequencies_from_input(CharInfo **dictionary, FILE *input);
size_t get_alphabet_size(CharInfo **dictionary);

#endif  // ENCODING_BITSTRING_H_
