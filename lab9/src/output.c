#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "typedefs.h"
#include "utils.h"

void print_path_info(PathInfo *pathInfo, int n_vertices, FILE *output) {
  int overflow_counter = 0;

  for (int i = 1; i <= n_vertices; ++i) {
    uint64_t distance_from_source = pathInfo->distances[i];
    if (distance_from_source == INFINITY) {
      fprintf(output, "oo ");
      continue;
    }

    if (distance_from_source >= INT_MAX) {
      ++overflow_counter;
    }
    if (distance_from_source > INT_MAX) {
      fprintf(output, "INT_MAX+ ");
    } else {
      fprintf(output, "%" PRId64 " ", distance_from_source);
    }
  }
  fprintf(output, "\n");

  uint64_t src_to_dst_len = pathInfo->distances[pathInfo->dst];
  if (src_to_dst_len == INFINITY) {
    fprintf(output, "no path\n");
  } else if (src_to_dst_len > INT_MAX && overflow_counter > 2) {
    fprintf(output, "overflow\n");
  } else {
    int temp = pathInfo->dst;
    while (temp != -1) {
      printf("%d ", temp);
      temp = pathInfo->previous_arr[temp];
    }
  }
}
