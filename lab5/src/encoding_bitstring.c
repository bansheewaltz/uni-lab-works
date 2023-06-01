#include <assert.h>
#include <limits.h>
#include <stdlib.h>

#include "bitstringutils.h"
#include "main.h"
#include "output.h"
#include "utils.h"

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

void scan_chars_frequencies_from_input(CharInfo **dictionary, FILE *input)
{
  // initialize info for every char in an alphabet to minimize number of
  // actions during iteration over input
  for (int i = 0; i < CHARSET_SIZE; ++i) {
    dictionary[i] = create_and_initialize_char_info(i);
  }

  uchar buffer[BUFFER_SIZE];

  size_t chars_read = 0;
  while ((chars_read = fread(buffer, sizeof(uchar), BUFFER_SIZE, input)) > 0) {
    for (size_t i = 0; i < chars_read; ++i) {
      ++dictionary[buffer[i]]->freq;
    }
  }

  // free info for chars that are never encountered
  for (int i = 0; i < CHARSET_SIZE; ++i) {
    if (dictionary[i]->freq == 0) {
      free(dictionary[i]);
      dictionary[i] = NULL;
    }
  }

  // set input pointer at the beginning
  int rc = fseek(input, 1, SEEK_SET);
  assert(rc == 0);
}

size_t get_alphabet_size(CharInfo **dictionary)
{
  size_t alph_size = 0;
  for (size_t i = 0; i < CHARSET_SIZE; ++i) {
    if (dictionary[i] != NULL) {
      ++alph_size;
    }
  }
  return alph_size;
}
