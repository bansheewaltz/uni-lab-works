#include "encoding.h"

#include <assert.h>

#include "binutils.h"
#include "encoding_bitstring.h"
#include "queue.h"
#include "utils.h"

void encoding(CodingInfo *codingInfo, FILE *input, FILE *output)
{
  scan_chars_frequencies_from_input(codingInfo->chars_info_dictionary, input);

  size_t alph_size = count_alphabet_size(codingInfo->chars_info_dictionary);
  if (alph_size == 0)
    return;

  codingInfo->alphabet_size = alph_size;

  CharInfo **chars_info_dictionary = codingInfo->chars_info_dictionary;
  CharInfo **chars_info_array = get_chars_info_consistent(chars_info_dictionary, alph_size);
  TreeNode *tree = build_huffman_tree(chars_info_array, alph_size);

  codingInfo->huffman_tree = tree;
  codingInfo->chars_info_array = chars_info_array;

  scan_codes_from_huffman_tree(tree, chars_info_dictionary);
  encode_input(tree, codingInfo, input, output);
}

void encode_input(TreeNode *tree, CodingInfo *codingInfo, FILE *input, FILE *output)
{
  size_t rc = 0;

  uint32_t input_file_length = 0;
  for (size_t i = 0; i < codingInfo->alphabet_size; ++i) {
    input_file_length += codingInfo->chars_info_array[i]->freq;
  }
  rc = fwrite(&input_file_length, sizeof(uint32_t), 1, output);
  assert(rc != -1);

  huffman_tree_write_encoded(tree, output);
  write_encoded_input(codingInfo->chars_info_dictionary, input, output);
}

void write_binary_array_to_bin(uint8_t *array, size_t code_len, FILE *output)
{
  for (int i = 0; i < code_len; ++i) {
    writebit_buffered(array[i], false, output);
  }
}

void write_encoded_input(CharInfo **chars_info_dictionary, FILE *input, FILE *output)
{
  uchar buffer[BUFFER_SIZE];
  size_t chars_read = 0;
  while ((chars_read = fread(buffer, sizeof(uchar), BUFFER_SIZE, input)) > 0) {
    for (size_t i = 0; i < chars_read; ++i) {
      CharInfo *char_info = chars_info_dictionary[buffer[i]];
      uint8_t *code = char_info->huffman_code;
      write_binary_array_to_bin(code, char_info->code_len, output);
    }
  }
  writebit_buffered(0, true, output);
}

void huffman_tree_preorder_traversal_writing(TreeNode *root, FILE *output)
{
  if (root == NULL)
    return;

  if (!is_node_leaf(root)) {
    writebit_buffered(INTERNAL_NODE, CONTINUE, output);
    huffman_tree_preorder_traversal_writing(root->left, output);
    huffman_tree_preorder_traversal_writing(root->right, output);
  }
  if (is_node_leaf(root)) {
    writebit_buffered(LEAF_NODE, CONTINUE, output);
    serialize_char(root->character, output);
  }
}

void huffman_tree_write_encoded(TreeNode *tree, FILE *output)
{
  if (tree == NULL)
    return;
  if (tree->left == NULL && tree->right == NULL) {
    writebit_buffered(LEAF_NODE, CONTINUE, output);
    serialize_char(tree->character, output);
    // goto flush_byte;
    return;
  }

  huffman_tree_preorder_traversal_writing(tree, output);

  // flush_byte:
  // writebit_buffered(0, FLUSH_BYTE, output);
}
