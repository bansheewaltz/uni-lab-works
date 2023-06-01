#include "output.h"

#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>

#include "bitutils.h"
#include "main.h"

void preorder_traversal_printing(TreeNode *root, FILE *output,
                                 size_t *bits_printed)
{
  if (root == NULL) {
    return;
  }

  if (!is_node_leaf(root)) {
    putc(bit_to_char(INTERNAL_NODE), output);
    *bits_printed += 1;
    preorder_traversal_printing(root->left, output, bits_printed);
    preorder_traversal_printing(root->right, output, bits_printed);
  }
  if (is_node_leaf(root)) {
    putc(bit_to_char(LEAF_NODE), output);
    *bits_printed += 1;
    print_char_in_binary(root->character, SAME_LINE, output);
    *bits_printed += CHAR_BIT;
  }
}

size_t print_huffman_tree_to_text(TreeNode *tree, FILE *output)
{
  if (tree == NULL) {
    return 0;
  }

  size_t bits_printed = 0;
  preorder_traversal_printing(tree, output, &bits_printed);

  return bits_printed;
}

void print_byte_in_binary(uchar byte, bool formatting, FILE *output)
{
  for (int i = CHAR_BIT - 1; i >= 0; --i) {
    fprintf(output, "%d", (byte >> i) & 1 ? 1 : 0);
  }
  if (formatting == NEW_LINE) {
    fprintf(output, "\n");
  }
}

void print_char_in_binary(uchar character, bool formatting, FILE *output)
{
  print_byte_in_binary(character, formatting, output);
}

void print_bits(void *ptr, size_t bytes_count, FILE *output)
{
  uint8_t *byte_array = (uint8_t *)ptr;

  for (size_t i = bytes_count - 1; i < bytes_count; --i) {
    for (unsigned int j = CHAR_BIT - 1; j < CHAR_BIT; --j) {
      unsigned int bit = ((unsigned int)byte_array[i] >> j) & 0x1U;
      int rc = fprintf(output, "%" PRIu8, (uint8_t)bit);
      assert(rc != -1);
    }
  }
}

void array_int_print(int array[], int len, FILE *output)
{
  for (int i = 0; i < len; ++i) {
    fprintf(output, "%d", array[i]);
  }
  fprintf(output, "\n");
}

void array_char_print(uchar array[], int len, FILE *output)
{
  for (int i = 0; i < len; ++i) {
    fprintf(output, "%d ", array[i]);
  }
  fprintf(output, "\n");
}

void print_code_array(CharInfo *charInfo, FILE *output)
{
  for (size_t i = 0; i < charInfo->code_len; ++i) {
    fprintf(output, "%" PRIu8, charInfo->huffman_code[i]);
  }
}

size_t print_encoded_file_to_text(CharInfo **chars_info_dictionary,
                                  FILE *input, FILE *output)
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

int huffman_tree_get_character_by_code(TreeNode *node, uchar *code,
                                       int code_len, int current_level)
{
  if (current_level != code_len) {
    if (char_to_bit(code[current_level]) == TREE_LEFT_CHILD_BIT)
      huffman_tree_get_character_by_code(node->left, code, code_len,
                                         ++current_level);
    if (char_to_bit(code[current_level]) == TREE_RIGHT_CHILD_BIT)
      huffman_tree_get_character_by_code(node->right, code, code_len,
                                         ++current_level);
  }
  if (node->character == INTERNAL_NODE_SYMBOL)
    return -1;

  return node->character;
}

void print_decoded_file_to_text(TreeNode *tree, FILE *input, FILE *output)
{
  uchar buffer[BUFFER_SIZE];
  uchar current_code[MAX_TREE_HEIGHT];
  int current_code_len = 0;

  size_t chars_read = 0;
  while ((chars_read = fread(buffer, sizeof(uchar), BUFFER_SIZE, input)) > 0) {
    for (size_t i = 0; i < chars_read; ++i) {
      current_code[current_code_len++] = buffer[i];
      int symbol = huffman_tree_get_character_by_code(tree, current_code,
                                                      current_code_len, 0);

      if (symbol == -1)
        continue;
      int rc = putc(symbol, output);
      assert(rc != -1);
    }
  }
}

void print_code(CharInfo *charInfo, FILE *output)
{
  fprintf(output, "%d : ", charInfo->character);
  for (size_t i = 0; i < charInfo->code_len; ++i) {
    fprintf(output, "%" PRIu8, charInfo->huffman_code[i]);
  }
  fprintf(output, "\n");
}

void print_codes_lexicographically(CharInfo **chars_info_array,
                                   size_t alphabet_size, FILE *output)
{
  for (size_t i = 0; i < alphabet_size; ++i) {
    print_code(chars_info_array[i], output);
  }
}

void print_coding_stats(CharInfo **chars_info_array, size_t alphabet_size,
                        FILE *output)
{
  int file_size = 0;
  int code_length = 0;
  double information_capacity = 0;

  for (size_t i = 0; i < alphabet_size; ++i) {
    file_size += chars_info_array[i]->freq;
    code_length += chars_info_array[i]->code_len * chars_info_array[i]->freq;
  }
  for (size_t i = 0; i < alphabet_size; ++i) {
    information_capacity +=
        (double)chars_info_array[i]->freq / file_size *
        log2((double)file_size / chars_info_array[i]->freq);
  }
  double redundancy =
      1.0 - (double)file_size / code_length * information_capacity;

  fprintf(output, "Information capacity = %.6f\n", information_capacity);
  fprintf(output, "Input file size = %d\n", file_size);
  fprintf(output, "Code length = %d\n", code_length);
  fprintf(output, "Redundancy = %.6f\n", redundancy);
}
