#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dijkstra_algorithm.h"
#include "graph.h"
#include "utils.h"

void deallocate_path_info(PathInfo *pathInfo) {
  if (pathInfo == NULL) {
    return;
  }

  free(pathInfo->distances);
  free(pathInfo->previous_arr);
}

void output_path_len_to_each_v(PathInfo *pathInfo, FILE *output) {
  if (pathInfo == NULL) {
    return;
  }
  assert(pathInfo->distances != NULL);
  assert(pathInfo->previous_arr != NULL);
  assert(output != NULL);

  for (int i = 1; i <= pathInfo->n_vertices; ++i) {
    uint64_t distance_from_source = pathInfo->distances[i];

    if (distance_from_source == INFINITY_LENGTH) {
      fprintf(output, "oo ");
    } else if (distance_from_source > INT_MAX) {
      fprintf(output, "INT_MAX+ ");
    } else {
      fprintf(output, "%" PRId64 " ", distance_from_source);
    }
  }

  fprintf(output, "\n");
}

static int count_overflows(PathInfo *pathInfo) {
  int overflow_counter = 0;

  for (int i = 1; i <= pathInfo->n_vertices; ++i) {
    uint64_t distance = pathInfo->distances[i];
    if (distance >= INT_MAX && distance < INFINITY_LENGTH) {
      ++overflow_counter;
    }
  }

  return overflow_counter;
}

void output_shortest_path(PathInfo *pathInfo, FILE *output) {
  if (pathInfo == NULL) {
    return;
  }
  assert(pathInfo->distances != NULL);
  assert(pathInfo->previous_arr != NULL);
  assert(output != NULL);

  uint64_t src_to_dst_len = pathInfo->distances[pathInfo->dst];

  if (src_to_dst_len == INFINITY_LENGTH) {
    fprintf(output, "no path\n");
  } else if (src_to_dst_len > INT_MAX && count_overflows(pathInfo) > 2) {
    fprintf(output, "overflow\n");
  } else {
    int temp = pathInfo->dst;

    while (temp != -1) {
      printf("%d ", temp);
      temp = pathInfo->previous_arr[temp];
    }
  }
}
