#ifndef MAIN_H_
#define MAIN_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tree.h"
#include "typedefs.h"

// character set & memory limits
#define EIGHT_BIT_CHARACTER_SET_SIZE 256
#define CHARSET_SIZE EIGHT_BIT_CHARACTER_SET_SIZE
#define BUFFER_SIZE 4096
// radix sort options
#define COUNTING_SORT_BASE 10
#define DESCENDING true
#define ASCENDING false
// huffman tree implementation features
#define MAX_TREE_HEIGHT EIGHT_BIT_CHARACTER_SET_SIZE
#define INTERNAL_NODE_SYMBOL 'x'
// huffman tree prefix code bits
#define TREE_LEFT_CHILD_BIT 0
#define TREE_RIGHT_CHILD_BIT 1
// huffman tree serialization bits
#define INTERNAL_NODE 0
#define LEAF_NODE 1
// bit-wise writing options
#define FLUSH_BYTE true
#define CONTINUE false
// bit-wise printing options
#define NEW_LINE true
#define SAME_LINE false

struct CharInfo {
  uchar character;
  size_t freq;
  uint8_t *huffman_code;
  size_t code_len;
};

struct CodingInfo {
  size_t alphabet_size;
  TreeNode *huffman_tree;
  CharInfo **chars_info_array;
  CharInfo *chars_info_dictionary[CHARSET_SIZE];
};

#endif  // MAIN_H_
