#include "sort.h"

#include <stdbool.h>

static void counting_sort_int_int(int arr[], int arr_by_which[], int size,
                                  int place, bool order) {
  int max_digit = COUNTING_SORT_BASE - 1;
  int count[COUNTING_SORT_BASE] = {0};
  int arr_output[size];
  int arr_by_output[size];

  // Calculate count of elements
  for (int i = 0; i < size; ++i) {
    count[(arr_by_which[i] / place) % COUNTING_SORT_BASE]++;
  }
  // Calculate cumulative count
  if (order) {
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
    arr_output[--count[(arr_by_which[i] / place) % COUNTING_SORT_BASE]] =
        arr[i];
    arr_by_output[count[(arr_by_which[i] / place) % COUNTING_SORT_BASE]] =
        arr_by_which[i];
  }
  // Copy the sorted elements into original arr_by_which
  for (int i = 0; i < size; ++i) {
    arr[i] = arr_output[i];
    arr_by_which[i] = arr_by_output[i];
  }
}

void radix_sort_int_int(int arr[], int arr_by_which[], int arr_length,
                        bool order) {
  int max = arr_by_which[0];

  for (int i = 1; i < arr_length; ++i) {
    if (arr_by_which[i] > max) {
      max = arr_by_which[i];
    }
  }

  for (int place = 1; max / place > 0; place *= COUNTING_SORT_BASE) {
    counting_sort_int_int(arr, arr_by_which, arr_length, place, order);
  }
}
