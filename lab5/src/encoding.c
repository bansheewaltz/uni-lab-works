#include <limits.h>

#include "bitutils.h"
#include "main.h"
#include "output.h"

void encode_input_text_form(TreeNode *tree, CodingInfo *codingInfo,
                            FILE *input, FILE *output)
{
  size_t bits_printed = 0;
  uint8_t alphabet_size = (uint8_t)codingInfo->alphabet_size;
  uint32_t input_file_length = 0;

  for (size_t i = 0; i < codingInfo->alphabet_size; ++i) {
    input_file_length += codingInfo->chars_info_array[i]->freq;
  }

#ifdef DEBUG
  fprintf(output, "%17s", "input file size: ");
#endif
  print_bits(&input_file_length, sizeof(uint32_t), output);
  bits_printed += CHAR_BIT * sizeof(uint32_t);
#ifdef DEBUG
  fprintf(output, "\n%17s", "alphabet size: ");
#endif
  print_bits(&alphabet_size, sizeof(uint8_t), output);
  bits_printed += CHAR_BIT * sizeof(uint8_t);
#ifdef DEBUG
  fprintf(output, "\n%17s", "huffman tree: ");
#endif
  bits_printed = print_huffman_tree_to_text(tree, output);
  fill_last_byte(&bits_printed, output);
#ifdef DEBUG
  fprintf(output, "\n%17s", "encoded input: ");
#endif
  bits_printed = print_encoded_file_to_text(codingInfo->chars_info_dictionary,
                                            input, output);
  fill_last_byte(&bits_printed, output);
  (void)bits_printed;
}
