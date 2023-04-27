#include "utils.h"

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adjacency_matrix.h"
#include "typedefs.h"

void print_error_terminate(char *message) {
  assert(message != NULL);

  puts(message);
  exit(EXIT_SUCCESS);  // but actually FAILURE
}

void print_allocation_failed_terminate(char *file, int line) {
  assert(file != NULL);

  fprintf(stdout, "Internal Error: %s:%d: %s\n", file, line, strerror(errno));
  exit(17);
}