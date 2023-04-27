#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "typedefs.h"
#include "utils.h"

#define IS_IN_BOUNDS(l, value, r) (l <= value && value <= r)

static bool scan_validate_n_of_vertices(int *N) {
  assert(N != NULL);

  if (scanf("%i", N) && IS_IN_BOUNDS(0, *N, MAX_VERTICES_NUM)) {
    return true;
  }

  print_error_terminate("bad number of vertices");
  return false;
}

static bool scan_validate_path(int *S, int *F, int N) {
  assert(S != NULL);
  assert(F != NULL);

  if ((scanf("%d", S) && IS_IN_BOUNDS(1, *S, N)) &&
      (scanf("%d", F) && IS_IN_BOUNDS(1, *F, N))) {
    return true;
  }

  print_error_terminate("bad vertex");
  return false;
}

static bool scan_validate_n_of_edges(int *M, int N) {
  assert(M != NULL);

  if (scanf("%d", M) && IS_IN_BOUNDS(0, *M, MAX_EDGES_NUM)) {
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

bool scan_validate_parameters(int *N, int *S, int *F, int *M) {
  assert(N != NULL);
  assert(S != NULL);
  assert(F != NULL);
  assert(M != NULL);

  return scan_validate_n_of_vertices(N) &&  //
         scan_validate_path(S, F, *N) &&    //
         scan_validate_n_of_edges(M, *N);   //
}
