#include "binutils.h"
#include "queue.h"

void preorder_traversal_serialization(TreeNode *root, FILE *output)
{
  if (root == NULL) {
    return;
  }

  if (!is_node_leaf(root)) {
    writebit_buffered(INTERNAL_NODE, CONTINUE, output);
    preorder_traversal_serialization(root->left, output);
    preorder_traversal_serialization(root->right, output);
  }
  if (is_node_leaf(root)) {
    writebit_buffered(LEAF_NODE, CONTINUE, output);
    serialize_char(root->character, output);
  }
}

void serialize_huffman_tree(TreeNode *tree, size_t alph_size, FILE *output)
{
  if (tree == NULL) {
    return;
  }

  uchar alph_size_as_byte = (uchar)alph_size;
  fwrite(&alph_size_as_byte, sizeof(uchar), 1, output);
  preorder_traversal_serialization(tree, output);
  writebit_buffered(0, FLUSH_BYTE, output);
}
