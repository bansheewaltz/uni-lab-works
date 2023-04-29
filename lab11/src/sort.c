#include "sort.h"

#include <stdbool.h>
#include <stdlib.h>

#include "utils.h"

static void array_int_counting_sort(int mapping[], int arr[], int size,
                                    int place, bool order) {
  int max_digit = COUNTING_SORT_BASE - 1;
  int count[COUNTING_SORT_BASE] = {0};
  // int mapping_output[size];
  int arr_output[size];

  // Calculate count of elements
  for (int i = 0; i < size; ++i) {
    count[(arr[i] / place) % COUNTING_SORT_BASE]++;
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
  for (int i = size - 1; i >= 0; --i) {
    // mapping_output[--count[(arr[i] / place) % COUNTING_SORT_BASE]] =
    // mapping[i];
    arr_output[--count[(arr[i] / place) % COUNTING_SORT_BASE]] = arr[i];
    mapping[i] = count[(arr[i] / place) % COUNTING_SORT_BASE];
  }
  // Copy the sorted elements into original arr
  for (int i = 0; i < size; ++i) {
    // mapping[i] = mapping_output[i];
    arr[i] = arr_output[i];
  }
  array_int_print(arr, size, 3);
  array_int_print(mapping, size, 3);
}

int *array_int_radix_sort(int arr[], int arr_length, bool order) {
  int max = arr[0];

  for (int i = 1; i < arr_length; ++i) {
    if (arr[i] > max) {
      max = arr[i];
    }
  }

  int *mapping = (int *)malloc(sizeof(int) * arr_length);
  if_fail(mapping == NULL, __FILE__, __LINE__);

  for (int place = 1; max / place > 0; place *= COUNTING_SORT_BASE) {
    array_int_counting_sort(mapping, arr, arr_length, place, order);
  }

  return mapping;
}
