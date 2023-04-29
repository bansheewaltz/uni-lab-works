#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void print_int_array(int *arr, int arr_length) {
  for (int i = 0; i < arr_length; ++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}
