#include "sort.h"

#include <stdbool.h>
#include <stdlib.h>

#include "utils.h"

#define COUNTING_SORT_BASE 10

static int *array_int_counting_sort(int indices[], int arr[], int arr_length,
                                    int place, bool order) {
  int max_digit = COUNTING_SORT_BASE - 1;
  int count[COUNTING_SORT_BASE] = {0};
  int *indices_output = (int *)malloc(sizeof(int) * arr_length);
  if (indices_output == NULL) {
    return NULL;
  }
  int *arr_output = (int *)malloc(sizeof(int) * arr_length);
  if (arr_output == NULL) {
    free(indices_output);
    return NULL;
  }

  // Calculate count of elements
  for (int i = 0; i < arr_length; ++i) {
    count[(arr[i] / place) % COUNTING_SORT_BASE]++;
  }
  // Calculate cumulative count
  if (order == DESCENDING_ORDER) {
    for (int i = max_digit; i > 0; --i) {
      count[i - 1] += count[i];
    }
  } else {
    for (int i = 1; i < max_digit + 1; ++i) {
      count[i] += count[i - 1];
    }
  }
  // Place the elements in sorted order
  for (int i = arr_length - 1; i >= 0; --i) {
    indices_output[--count[(arr[i] / place) % COUNTING_SORT_BASE]] = indices[i];
    arr_output[count[(arr[i] / place) % COUNTING_SORT_BASE]] = arr[i];
  }
  // Copy the sorted elements into original arr
  for (int i = 0; i < arr_length; ++i) {
    indices[i] = indices_output[i];
    arr[i] = arr_output[i];
  }

  free(indices_output);
  free(arr_output);

  return indices;
}

int *array_int_radix_sort(int arr[], int arr_length, bool order) {
  int *indices = create_indices_array(arr_length);
  if (indices == NULL) {
    return NULL;
  }

  int max = arr[0];
  for (int i = 1; i < arr_length; ++i) {
    if (arr[i] > max) {
      max = arr[i];
    }
  }

  for (int place = 1; max / place > 0; place *= COUNTING_SORT_BASE) {
    indices = array_int_counting_sort(indices, arr, arr_length, place, order);
  }

  int *mapping = (int *)malloc(sizeof(int) * arr_length);
  if (mapping != NULL) {
    for (int i = 0; i < arr_length; ++i) {
      mapping[indices[i]] = i;
    }
  }

  free(indices);

  return mapping;
}
