#include "decoding.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitutils.h"
#include "main.h"

int read_alphabet_size_checked(FILE *input)
{
  uint8_t alphabet_size = 0;
  alphabet_size = read_bits_ascii(input, sizeof(uint8_t));
  if (alphabet_size == 0x00U) {
    return EIGHT_BIT_CHARACTER_SET_SIZE;
  }
  return (int)alphabet_size;
}

uint32_t read_file_size_checked(FILE *input)
{
  uint32_t file_size = 0;
  file_size = read_bits_ascii(input, sizeof(uint32_t));
  assert(file_size != 0x00000000U);
  return file_size;
}

TreeNode *read_huffman_tree_text_form(int alph_size, FILE *input)
{
  TreeNode *tree = (TreeNode *)malloc(sizeof(TreeNode));
  preorder_traversal_read_text_form(tree, alph_size, input, 0);
  return tree;
}

uchar read_char_binary_text_form(FILE *input)
{
  uint8_t character = 0x0U;
  character = (uint8_t)read_bits_ascii(input, sizeof(uint8_t));

  return (uchar)character;
}

void preorder_traversal_read_text_form(TreeNode *root, int alph_size,
                                       FILE *input, int depth)
{
  if (root == NULL) {
    return;
  }
  static size_t recovered_characters_count = 0;
  static size_t bits_read_count = 0;

  bool bit = readbit_ascii(input);
  bits_read_count += 1;
  if (bit == INTERNAL_NODE) {
    root->character = '\0';

    TreeNode *left = (TreeNode *)malloc(sizeof(TreeNode));
    root->left = left;
    TreeNode *right = (TreeNode *)malloc(sizeof(TreeNode));
    root->right = right;

    preorder_traversal_read_text_form(left, alph_size, input, depth + 1);
    preorder_traversal_read_text_form(right, alph_size, input, depth + 1);
  }
  if (bit == LEAF_NODE) {
    root->left = NULL;
    root->right = NULL;
    root->character = read_char_binary_text_form(input);
    bits_read_count += CHAR_BIT;
    recovered_characters_count += 1;
    // #ifdef DEBUG
    //     printf("%d\n", root->character);
    // #endif
  }

  assert(bit == LEAF_NODE || bit == INTERNAL_NODE);
  if (recovered_characters_count == alph_size) {
    while (bits_read_count % CHAR_BIT != 0) {
      (void)readbit_ascii(input);
      bits_read_count += 1;
    }
    return;
  }
}
