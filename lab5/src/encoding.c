#include <limits.h>

#include "bitutils.h"
#include "main.h"
#include "output.h"

void archive_file_text_form(TreeNode *tree, CodingInfo *codingInfo,
                            FILE *input, FILE *output)
{
  size_t bits_printed = 0;
  uint8_t alphabet_size = (uint8_t)codingInfo->alphabet_size;
  uint32_t input_file_length = 0;

  for (size_t i = 0; i < codingInfo->alphabet_size; ++i) {
    input_file_length += codingInfo->chars_info_consistent[i]->freq;
  }

  print_bits(&input_file_length, sizeof(uint32_t), output);
  bits_printed += CHAR_BIT * sizeof(uint32_t);
#ifdef DEBUG
  fprintf(output, "\n");
#endif
  print_bits(&alphabet_size, sizeof(uint8_t), output);
  bits_printed += CHAR_BIT * sizeof(uint8_t);
#ifdef DEBUG
  fprintf(output, "\n");
#endif
  bits_printed = print_huffman_tree_to_text(tree, output);
  fill_last_byte(&bits_printed, output);
#ifdef DEBUG
  fprintf(output, "\n");
#endif
  bits_printed = print_encoded_file_to_text(codingInfo->chars_info_dictionary,
                                            input, output);
  fill_last_byte(&bits_printed, output);
  (void)bits_printed;
}
