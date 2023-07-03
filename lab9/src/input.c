#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "graph.h"
#include "main.h"
#include "utils.h"

#define IS_IN_BOUNDS(l, value, r) (l <= value && value <= r)

static bool scan_validate_n_of_vertices(int *V) {
  assert(V != NULL);

  if (scanf("%i", V) && IS_IN_BOUNDS(0, *V, MAX_VERTICES_NUM)) {
    return true;
  }

  print_error_terminate("bad number of vertices");
  return false;
}

static bool scan_validate_path(int *src, int *dst, int V) {
  assert(src != NULL);
  assert(dst != NULL);

  if ((scanf("%d", src) && IS_IN_BOUNDS(1, *src, V)) &&
      (scanf("%d", dst) && IS_IN_BOUNDS(1, *dst, V))) {
    return true;
  }

  print_error_terminate("bad vertex");
  return false;
}

static bool scan_validate_n_of_edges(int *E, int V) {
  assert(E != NULL);

  if (scanf("%d", E) && IS_IN_BOUNDS(0, *E, MAX_EDGES_NUM)) {
    return true;
  }

  print_error_terminate("bad number of edges");
  return false;
}

static bool scan_validate_edge_len(int *length) {
  assert(length != NULL);

  if (scanf("%d", length) && IS_IN_BOUNDS(0, *length, MAX_EDGE_LEN)) {
    return true;
  }

  print_error_terminate("bad length");
  return false;
}

int scan_validate_edge(Edge *edge, int n_vertices) {
  assert(edge != NULL);

  if (scan_validate_path(&edge->src, &edge->dst, n_vertices) &&
      scan_validate_edge_len(&edge->weight)) {
    return SUCCESS;
  }

  return FAILURE;
}

bool scan_validate_parameters(int *V, int *src, int *dst, int *E) {
  assert(V != NULL);
  assert(src != NULL);
  assert(dst != NULL);
  assert(E != NULL);

  return scan_validate_n_of_vertices(V) &&    //
         scan_validate_path(src, dst, *V) &&  //
         scan_validate_n_of_edges(E, *V);     //
}
