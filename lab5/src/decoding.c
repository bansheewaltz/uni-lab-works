#include "decoding.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "binutils.h"
#include "decoding_bitstring.h"
#include "main.h"

void decoding(CodingInfo *codingInfo, FILE *input, FILE *output)
{
  size_t file_size = (size_t)read_file_size_binary(input);
  codingInfo->file_size = file_size;
  if (file_size == 0)
    return;

  size_t alphabet_size = (size_t)read_alphabet_size_binary(input);
  codingInfo->alphabet_size = alphabet_size;
  if (alphabet_size == 0)
    return;

  TreeNode *tree = huffman_tree_read_binary(alphabet_size, input);
  codingInfo->huffman_tree = tree;
  assert(tree != NULL);

  print_decoded_file(codingInfo, input, output);
}

void print_decoded_file(CodingInfo *coding_info, FILE *input, FILE *output)
{
  size_t characteres_decoded = 0;
  TreeNode *tree_root = coding_info->huffman_tree;

  TreeNode *current_node = tree_root;
  while (characteres_decoded != coding_info->file_size) {
    bool bit = readbit_buffered(input, false);
    if (coding_info->alphabet_size == 1)
      goto print_char;

    if (bit == TREE_LEFT_CHILD_BIT)
      current_node = current_node->left;
    if (bit == TREE_RIGHT_CHILD_BIT)
      current_node = current_node->right;

  print_char:
    if (current_node->freq) {
      int rc = putc(current_node->character, output);  // NOLINT
      assert(rc != -1);
      characteres_decoded += 1;
      current_node = tree_root;
    }
  }
}

uint32_t read_file_size_binary(FILE *input)
{
  uint32_t file_size = 0;
  size_t rc = fread(&file_size, sizeof(uint32_t), 1, input);
  assert(rc != -1);
  return file_size;
}

uint8_t read_alphabet_size_binary(FILE *input)
{
  uint32_t file_size = 0;
  size_t rc = fread(&file_size, sizeof(uint8_t), 1, input);
  assert(rc != -1);
  return file_size;
}

TreeNode *huffman_tree_read_binary(int alph_size, FILE *input)
{
  TreeNode *tree = (TreeNode *)malloc(sizeof(TreeNode));
  preorder_traversal_read_binary(tree, alph_size, input, 0);
  return tree;
}

uchar deserialize_char(FILE *input)
{
  int bit_position = 1;  // in uchar, from left
  uchar character = 0x00U;

  for (; bit_position <= CHAR_BIT; ++bit_position) {
    set_bit(&character, readbit_buffered(input, false), bit_position);
  }

  return character;
}

void preorder_traversal_read_binary(TreeNode *root, int alph_size, FILE *input, int depth)
{
  if (root == NULL)
    return;

  static int recovered_characters_count = 0;

  bool bit = readbit_buffered(input, false);
  if (bit == INTERNAL_NODE) {
    root->character = '\0';

    TreeNode *left = (TreeNode *)malloc(sizeof(TreeNode));
    root->left = left;
    TreeNode *right = (TreeNode *)malloc(sizeof(TreeNode));
    root->right = right;

    preorder_traversal_read_binary(left, alph_size, input, depth + 1);
    preorder_traversal_read_binary(right, alph_size, input, depth + 1);
  }
  if (bit == LEAF_NODE) {
    root->left = NULL;
    root->right = NULL;
    root->character = deserialize_char(input);
    root->freq = 1;
    ++recovered_characters_count;
#ifdef DEBUG
    printf("%d\n", root->character);
#endif
  }
  assert(bit == LEAF_NODE || bit == INTERNAL_NODE);
  if (recovered_characters_count == alph_size) {
    return;
  }
}
