#include "deserialization.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitutils.h"
#include "main.h"

TreeNode *deserialize_huffman_tree(int alph_size, FILE *input,
                                   uchar **characters_array)
{
  TreeNode *tree = (TreeNode *)malloc(sizeof(TreeNode));
  *characters_array = (uchar *)malloc(sizeof(uchar) * alph_size);

  preorder_traversal_deserialization(tree, alph_size, input, *characters_array,
                                     0);

  return tree;
}

uchar deserialize_char(FILE *input)
{
  int bit_position = 1;  // in uchar, from left
  uchar character = 0b00000000;

  for (; bit_position <= CHAR_BIT; ++bit_position) {
    set_bit(&character, readbit_buffered(input), bit_position);
  }

  return character;
}

void preorder_traversal_deserialization(TreeNode *root, int alph_size,
                                        FILE *input, uchar *characters_array,
                                        int depth)
{
  if (root == NULL) {
    return;
  }
  static int recovered_characters_count = 0;
  if (recovered_characters_count == alph_size) {
    return;
  }

  uint8_t bit = readbit_buffered(input);
  if (bit == INTERNAL_NODE) {
    TreeNode *left = (TreeNode *)malloc(sizeof(TreeNode));
    TreeNode *right = (TreeNode *)malloc(sizeof(TreeNode));

    root->left = left;
    root->right = right;
    preorder_traversal_deserialization(left, alph_size, input,
                                       characters_array, depth + 1);
    preorder_traversal_deserialization(right, alph_size, input,
                                       characters_array, depth + 1);
  }
  if (bit == LEAF_NODE) {
    root->left = NULL;
    root->right = NULL;
    root->character = deserialize_char(input);
    characters_array[recovered_characters_count] = root->character;
    ++recovered_characters_count;
#ifdef DEBUG
    printf("%d\n", root->character);
#endif
  }
  if (bit != INTERNAL_NODE && bit != LEAF_NODE) {
    exit(1);
  }
}
