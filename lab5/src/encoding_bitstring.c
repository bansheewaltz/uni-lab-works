#include "encoding_bitstring.h"

#include <assert.h>
#include <limits.h>
#include <stdlib.h>

#include "bitstringutils.h"
#include "main.h"
#include "output.h"
#include "utils.h"

void encoding_bitstring(CodingInfo *codingInfo, FILE *input, FILE *output)
{
  scan_chars_frequencies_from_input(codingInfo->chars_info_dictionary, input);

  size_t alphabet_size = count_alphabet_size(codingInfo->chars_info_dictionary);
  assert(alphabet_size > 0);
  codingInfo->alphabet_size = alphabet_size;

  CharInfo **chars_info_dictionary = codingInfo->chars_info_dictionary;
  CharInfo **chars_info_array = get_chars_info_consistent(chars_info_dictionary, alphabet_size);
  TreeNode *tree = build_huffman_tree(chars_info_array, alphabet_size);

  codingInfo->huffman_tree = tree;
  codingInfo->chars_info_array = chars_info_array;

  scan_codes_from_huffman_tree(tree, chars_info_dictionary);
#ifdef DEBUG
  print_codes_lexicographically(chars_info_array, alphabet_size, stdout);
  print_coding_stats(chars_info_array, alphabet_size, stdout);
#endif
  encode_input_text_form(tree, codingInfo, input, output);
}

void encode_input_text_form(TreeNode *tree, CodingInfo *codingInfo, FILE *input, FILE *output)
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
  bits_printed = print_encoded_file_to_text(codingInfo->chars_info_dictionary, input, output);
  fill_last_byte(&bits_printed, output);
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

size_t count_alphabet_size(CharInfo **dictionary)
{
  size_t alph_size = 0;
  for (size_t i = 0; i < CHARSET_SIZE; ++i) {
    if (dictionary[i] != NULL) {
      ++alph_size;
    }
  }
  return alph_size;
}

void preorder_traversal_printing(TreeNode *root, FILE *output, size_t *bits_printed)
{
  if (root == NULL)
    return;

  if (!is_node_leaf(root)) {
    (void)putc(bit_to_char(INTERNAL_NODE), output);
    *bits_printed += 1;
    preorder_traversal_printing(root->left, output, bits_printed);
    preorder_traversal_printing(root->right, output, bits_printed);
  }
  if (is_node_leaf(root)) {
    (void)putc(bit_to_char(LEAF_NODE), output);
    *bits_printed += 1;
    print_char_in_bitstring(root->character, SAME_LINE, output);
    fprintf(output, "\n%17s", "");
    *bits_printed += CHAR_BIT;
  }
}

size_t print_huffman_tree_to_text(TreeNode *tree, FILE *output)
{
  if (tree == NULL)
    return 0;
  if (tree->left == NULL && tree->right == NULL) {
    (void)putc(bit_to_char(LEAF_NODE), output);
    return 1;
  }

  size_t bits_printed = 0;
  preorder_traversal_printing(tree, output, &bits_printed);

  return bits_printed;
}

size_t print_encoded_file_to_text(CharInfo **chars_info_dictionary, FILE *input, FILE *output)
{
  uchar buffer[BUFFER_SIZE];

  size_t chars_read = 0;
  size_t bits_printed = 0;
  while ((chars_read = fread(buffer, sizeof(uchar), BUFFER_SIZE, input)) > 0) {
    for (size_t i = 0; i < chars_read; ++i) {
      print_code_array(chars_info_dictionary[buffer[i]], output);
      bits_printed += chars_info_dictionary[buffer[i]]->code_len;
    }
  }

  return bits_printed;
}
