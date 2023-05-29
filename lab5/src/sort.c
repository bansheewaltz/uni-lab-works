#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "main.h"

void counting_sort_nodes(TreeNode **array, size_t array_size, size_t place,
                         bool order)
{
  assert(array_size > 0);
  int max_digit = COUNTING_SORT_BASE - 1;
  int count[COUNTING_SORT_BASE] = {0};
  TreeNode **array_output =
      (TreeNode **)malloc(sizeof(TreeNode *) * array_size);
  // for (size_t i = 0; i < array_size; ++i){
  //   array_output[i] = malloc()
  // }

  // Calculate count of elements
  for (size_t i = 0; i < array_size; ++i) {
    count[(array[i]->freq / place) % 10]++;
  }
  // Calculate cumulative count
  if (order == DESCENDING) {
    for (int i = max_digit; i > 0; --i) {
      count[i - 1] += count[i];
    }
  } else {
    for (int i = 1; i < max_digit + 1; ++i) {
      count[i] += count[i - 1];
    }
  }
  // Place the elements in sorted order
  for (size_t i = array_size - 1; i < array_size; --i) {
    array_output[--count[(array[i]->freq / place) % 10]] = array[i];
  }
  // Copy the sorted elements into original array
  for (size_t i = 0; i < array_size; ++i) {
    array[i] = array_output[i];
  }
  free(array_output);
}

void radix_sort_nodes(TreeNode **array, size_t array_size, bool order)
{
  size_t max = array[0]->freq;

  for (size_t i = 1; i < array_size; ++i) {
    if (array[i]->freq > max) {
      max = array[i]->freq;
    }
  }

  for (int place = 1; max / place > 0; place *= 10) {
    counting_sort_nodes(array, array_size, place, order);
  }
}
