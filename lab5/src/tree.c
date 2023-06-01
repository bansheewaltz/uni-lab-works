#include "tree.h"

#include <stdlib.h>

#include "main.h"
#include "queue.h"
#include "tools.h"
#include "utils.h"

TreeNode *create_tree_node(uchar character, size_t freq)
{
  TreeNode *temp = (TreeNode *)malloc(sizeof(TreeNode));
  check_null_pointer(temp);

  temp->left = temp->right = NULL;
  temp->character = character;
  temp->freq = freq;

  return temp;
}

TreeNode *build_huffman_tree(CharInfo **chars_info_array, size_t alphabet_size)
{
  if (alphabet_size == 1) {
    return create_tree_node(chars_info_array[0]->character,
                            chars_info_array[0]->freq);
  }

  TreeNode *left = NULL;
  TreeNode *right = NULL;
  TreeNode *top = NULL;
  Queue *firstQueue = create_queue(alphabet_size);
  Queue *secondQueue = create_queue(alphabet_size);

  for (size_t i = 0; i < alphabet_size; ++i) {
    enqueue(firstQueue, create_tree_node(chars_info_array[i]->character,
                                         chars_info_array[i]->freq));
  }

#ifdef DEBUG
  puts("original queue:");
  display_queue(firstQueue);
#endif
  sort_queue(firstQueue, alphabet_size, DESCENDING);
#ifdef DEBUG
  puts("sorted queue:");
  display_queue(firstQueue);
#endif
  reverse_queue(firstQueue);
#ifdef DEBUG
  puts("reversed queue:");
  display_queue(firstQueue);
#endif

  while (!(is_queue_empty(firstQueue) && is_queue_size_one(secondQueue))) {
    right = find_queue_min(firstQueue, secondQueue);
    left = find_queue_min(firstQueue, secondQueue);

    top = create_tree_node(INTERNAL_NODE_SYMBOL, left->freq + right->freq);
    top->left = left;
    top->right = right;
    enqueue(secondQueue, top);
  }

  TreeNode *huffman_tree = dequeue(secondQueue);
  destroy_queue(firstQueue);
  destroy_queue(secondQueue);

  return huffman_tree;
}

bool is_node_leaf(TreeNode *node)
{
  return !(node->left) && !(node->right);
}

void destroy_tree(struct TreeNode *root)
{
  if (root == NULL) {
    return;
  }

  destroy_tree(root->left);
  destroy_tree(root->right);

  free(root);
}

void preoder_traversal_codes_scan(TreeNode *root, CharInfo **chars_info,
                                  uint8_t code[], size_t depth)
{
  if (root->left) {
    code[depth] = TREE_LEFT_CHILD_BIT;
    preoder_traversal_codes_scan(root->left, chars_info, code, depth + 1);
  }
  if (root->right) {
    code[depth] = TREE_RIGHT_CHILD_BIT;
    preoder_traversal_codes_scan(root->right, chars_info, code, depth + 1);
  }

  if (is_node_leaf(root)) {
    chars_info[root->character]->huffman_code =
        array_uint8_duplicate(code, depth);
    chars_info[root->character]->code_len = depth;
  }
}

void scan_codes_from_huffman_tree(TreeNode *root, CharInfo **chars_info)
{
  uint8_t code[MAX_TREE_HEIGHT];
  size_t depth = 0;

  preoder_traversal_codes_scan(root, chars_info, code, depth);
}
