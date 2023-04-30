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
void array_bool_print(bool *arr, int arr_length, int alignment) {
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

int *create_indices_array(int arr_length) {
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
int array_int_alignment(int *arr, int arr_size) {
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

void array_int_reorder(int *arr, const int *mapping, int arr_length) {
  assert(arr != NULL);
  assert(mapping != NULL);

  int temp[arr_length];

  for (int i = 0; i < arr_length; ++i) {
    temp[mapping[i]] = arr[i];
  }

  for (int i = 0; i < arr_length; ++i) {
    arr[i] = temp[i];
  }
}

void array_bool_reorder(bool *arr, const int *mapping, int arr_length) {
  assert(arr != NULL);
  assert(mapping != NULL);

  bool temp[arr_length];

  for (int i = 0; i < arr_length; ++i) {
    temp[mapping[i]] = arr[i];
  }

  for (int i = 0; i < arr_length; ++i) {
    arr[i] = temp[i];
  }
}

int *create_reverse_mapping(const int mapping[], int arr_length) {
  if (mapping == NULL) {
    return NULL;
  }

  int *reverse_mapping = (int *)malloc(sizeof(int) * arr_length);

  if (reverse_mapping != NULL) {
    for (int i = 0; i < arr_length; ++i) {
      reverse_mapping[mapping[i]] = i;
    }
  }

  return reverse_mapping;
}

// cppcheck-suppress unusedFunction
// NOLINTNEXTLINE
void print_table(int width, int table[][width], int objects_count,
                 int values[],  // NOLINT
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

int int_max(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}
