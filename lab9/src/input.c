#include <stdbool.h>
#include <stdio.h>

#include "typedefs.h"
#include "utils.h"

#define IS_IN_BOUNDS(l, value, r) (l <= value && value <= r)

static bool scan_validate_n_of_vertices(int *N) {
  if (scanf("%i", N) && IS_IN_BOUNDS(0, *N, MAX_VERTICES_NUM)) {
    return true;
  }

  print_error_terminate("bad number of vertices");
  return false;
}

static bool scan_validate_path(int *S, int *F, int N) {
  if ((scanf("%d", S) && IS_IN_BOUNDS(1, *S, N)) &&
      (scanf("%d", F) && IS_IN_BOUNDS(1, *F, N))) {
    return true;
  }

  print_error_terminate("bad vertex");
  return false;
}

static bool scan_validate_n_of_edges(int *M, int N) {
  if (scanf("%d", M) && IS_IN_BOUNDS(0, *M, MAX_EDGES_NUM)) {
    return true;
  }

  print_error_terminate("bad number of edges");
  return false;
}

static bool scan_validate_edge_len(int *length) {
  if (scanf("%d", length) && IS_IN_BOUNDS(0, *length, MAX_EDGE_LEN)) {
    return true;
  }

  print_error_terminate("bad length");
  return false;
}

int scan_validate_edge(Edge *edge, int n_vertices) {
  if (scan_validate_path(&edge->src, &edge->dst, n_vertices) &&
      scan_validate_edge_len(&edge->weight)) {
    return SUCCESS;
  }

  return FAILURE;
}

bool scan_validate_parameters(int *N, int *S, int *F, int *M) {
  return scan_validate_n_of_vertices(N) &&  //
         scan_validate_path(S, F, *N) &&    //
         scan_validate_n_of_edges(M, *N);   //
}
