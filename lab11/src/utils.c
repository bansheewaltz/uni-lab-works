#include "utils.h"

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// cppcheck-suppress unusedFunction
void print_error_terminate(char *message) {
  assert(message != NULL);

  puts(message);
  exit(EXIT_SUCCESS);  // but actually FAILURE
}

void if_fail(bool check, char *file, int line) {
  assert(file != NULL);

  if (check == true) {
    fprintf(stdout, "Internal Error: %s:%d: %s\n", file, line, strerror(errno));
    exit(EXIT_FAILURE);
  }
}

// cppcheck-suppress unusedFunction
void array_int_print(int *arr, int arr_length, int alignment) {
  for (int i = 0; i < arr_length; ++i) {
    printf("%*d ", alignment, arr[i]);
  }
  printf("\n");
}

// cppcheck-suppress unusedFunction
int *array_int_copy(const int *arr, int arr_length) {
  int *copy = (int *)malloc(sizeof(int) * arr_length);

  if (copy != NULL) {
    for (int i = 0; i < arr_length; ++i) {
      copy[i] = arr[i];
    }
  }

  return copy;
}

// cppcheck-suppress unusedFunction
int *create_mapping_array(int arr_length) {
  int *arr = (int *)malloc(sizeof(int) * arr_length);

  if (arr != NULL) {
    for (int i = 0; i < arr_length; ++i) {
      arr[i] = i;
    }
  }

  return arr;
}

// cppcheck-suppress unusedFunction
int array_int_max(const int *arr, int arr_size) {
  assert(arr != NULL);
  int temp_max = arr[0];

  for (int i = 1; i < arr_size; ++i) {
    if (arr[i] > temp_max) {
      temp_max = arr[i];
    }
  }

  return temp_max;
}

// cppcheck-suppress unusedFunction
int count_int_digits(int n) {
  int count = 1;

  if (n < 0) {
    if (n == INT_MIN) {
      n = INT_MAX;
    } else {
      n = -n;
    }
  }

  while (n > DECIMAL_BASE - 1) {
    n /= DECIMAL_BASE;
    ++count;
  }

  return count;
}

// cppcheck-suppress unusedFunction
int array_int_print_alignment(int *arr, int arr_size) {
  assert(arr != NULL);
  int temp_max = count_int_digits(arr[0]);

  for (int i = 1; i < arr_size; ++i) {
    int digits = count_int_digits(arr[i]);
    if (digits > temp_max) {
      temp_max = digits;
    }
  }

  return temp_max;
}

// cppcheck-suppress unusedFunction
void array_int_reorder(int *arr, int *indexes, int arr_length) {
  assert(arr != NULL);
  assert(indexes != NULL);

  int temp[arr_length];

  for (int i = 0; i < arr_length; ++i) {
    temp[i] = arr[indexes[i]];
  }

  for (int i = 0; i < arr_length; ++i) {
    arr[i] = temp[i];
    indexes[i] = i;
  }
}

// cppcheck-suppress unusedFunction
void print_table(int width, int table[][width], int objects_count, int values[],
                 int weights[]) {
  for (int i = -3; i < width; ++i) {
    if (i < 0) {
      printf("%4s ", "");
    } else {
      printf("%4d ", i);
    }
  }
  printf("\n");

  for (int i = 0; i <= objects_count; ++i) {
    for (int j = -3; j < width; ++j) {
      if (i == 0 && j < 0) {
        if (j == -3) {
          printf("%4s ", "val");
        }
        if (j == -2) {
          printf("%4s ", "wgt");
        }
        if (j == -1) {
          printf("%4s ", "obj");
        }
        continue;
      }
      if (j == -3) {
        printf("%4d ", values[i - 1]);
      }
      if (j == -2) {
        printf("%4d ", weights[i - 1]);
      }
      if (j == -1) {
        printf("%4d ", i);
      }
      if (j >= 0) {
        printf("%4d ", table[i][j]);
      }
    }
    printf("\n");
  }
}
