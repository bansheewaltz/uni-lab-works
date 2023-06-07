#include "decoding.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "binutils.h"
#include "decoding_bitstring.h"
#include "main.h"
#include "tree.h"

static TreeNode *ht_read_binary_preorder_traversal(FILE *input);

void decoding(CodingInfo *codingInfo, FILE *input, FILE *output)
{
  size_t file_size = (size_t)read_file_size_binary(input);
  codingInfo->file_size = file_size;
  if (file_size == 0)
    return;

  TreeNode *tree = huffman_tree_read_binary(input);
  codingInfo->huffman_tree = tree;
  assert(tree != NULL);

  print_decoded_file(codingInfo, input, output);
}

void print_decoded_file(CodingInfo *coding_info, FILE *input, FILE *output)
{
  size_t characteres_decoded = 0;
  TreeNode *tree_root = coding_info->huffman_tree;

  bool one_bit_encoding = false;
  if (tree_root->left == NULL && tree_root->right == NULL)
    one_bit_encoding = true;

  TreeNode *current_node = tree_root;
  while (characteres_decoded != coding_info->file_size) {
    bool bit = readbit_buffered(input, false);
    if (one_bit_encoding)
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

TreeNode *huffman_tree_read_binary(FILE *input)
{
  return ht_read_binary_preorder_traversal(input);
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

typedef struct {
  TreeNode *node[CHARSET_SIZE];
  int top;
} ht_node_stack;

void stack_push(ht_node_stack *stack, TreeNode *node)
{
  stack->node[++stack->top] = node;
}

TreeNode *stack_pop(ht_node_stack *stack)
{
  return stack->node[stack->top--];
}

bool stack_is_empty(ht_node_stack *stack)
{
  return stack->top == -1;
}

TreeNode *stack_peek(ht_node_stack *stack)
{
  if (stack_is_empty(stack)) {
    return NULL;
  }
  return stack->node[stack->top];
}

void discard_pending_byte_bits(FILE *input)
{
  (void)readbit_buffered(input, true);
}

bool read_bit(FILE *input)
{
  return readbit_buffered(input, false);
}

static TreeNode *ht_read_binary_preorder_traversal(FILE *input)
{
  ht_node_stack stack;
  stack.top = -1;

  TreeNode *last_completed_innner_node = NULL;
  do {
    bool bit = read_bit(input);
    if (bit == 1 && stack_is_empty(&stack))
      return create_tree_node(deserialize_char(input), 1);

    if (bit == 0) {
      TreeNode *internal_node = create_tree_node('\0', 0);
      TreeNode *internal_node_prev = stack_peek(&stack);
      if (internal_node_prev) {
        if (internal_node_prev->left == NULL)
          internal_node_prev->left = internal_node;
        else
          internal_node_prev->right = internal_node;
      }
      stack_push(&stack, internal_node);
    }

    if (bit == 1) {
      TreeNode *leaf_node = create_tree_node(deserialize_char(input), 1);
      TreeNode *internal_node = stack_peek(&stack);
      if (internal_node->left == NULL)
        internal_node->left = leaf_node;
      else
        internal_node->right = leaf_node;
      while (internal_node->left && internal_node->right) {
        last_completed_innner_node = stack_pop(&stack);
        internal_node = stack_peek(&stack);
        if (internal_node == NULL)
          break;
      }
    }
  } while (!stack_is_empty(&stack));
  TreeNode *tree_root = last_completed_innner_node;

  discard_pending_byte_bits(input);
  return tree_root;
}

// void preorder_traversal_read_binary(TreeNode *root, int alph_size, FILE *input, int depth)
// {
//   if (root == NULL)
//     return;

//   static int recovered_characters_count = 0;

//   bool bit = readbit_buffered(input, false);
//   if (bit == INTERNAL_NODE) {
//     root->character = '\0';

//     TreeNode *left = (TreeNode *)malloc(sizeof(TreeNode));
//     root->left = left;
//     TreeNode *right = (TreeNode *)malloc(sizeof(TreeNode));
//     root->right = right;

//     preorder_traversal_read_binary(left, alph_size, input, depth + 1);
//     preorder_traversal_read_binary(right, alph_size, input, depth + 1);
//   }
//   if (bit == LEAF_NODE) {
//     root->left = NULL;
//     root->right = NULL;
//     root->character = deserialize_char(input);
//     root->freq = 1;
//     ++recovered_characters_count;
// #ifdef DEBUG
//     printf("%d\n", root->character);
// #endif
//   }
//   assert(bit == LEAF_NODE || bit == INTERNAL_NODE);
//   if (recovered_characters_count == alph_size) {
//     (void)readbit_buffered(input, true);
//     return;
//   }
// }
