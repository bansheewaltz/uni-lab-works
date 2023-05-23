#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "input.h"
#include "typehandlers.h"
#include "utils.h"

ReturnCode read_validate_graph(Graph** graph)
{
  ReturnCode returnCode = 0;

  int vertices_count = -1;
  int edges_count = -1;
  returnCode = scan_validate_parameters(&vertices_count, &edges_count);
  if (is_error(returnCode)) {
    return returnCode;
  }

  *graph = graph_init(vertices_count, edges_count);
  if (*graph == NULL) {
    returnCode = E_MEMORY_ALLOCATION_FAIL;
    return returnCode;
  }

  returnCode = scan_validate_edges(*graph);
  if (is_error(returnCode)) {
    return returnCode;
  }

  return E_SUCCESS;
}

int* initialize_cost(int vertices_count)
{
  int* costs = malloc(sizeof(int) * (size_t)vertices_count);
  if (costs == NULL) {
    return NULL;
  }
  for (int i = 0; i < vertices_count; ++i) {
    costs[i] = INFINITY;
  }
  return costs;
}

void recalculate_smallest_costs(Graph* graph, bool* used, int* costs,
                                int v_src, int* edge_srcs)
{
  int vertices_count = graph->vertices_count;
  int* graph_matrix_array = graph->graph_array;
  for (int i = 0; i < vertices_count; ++i) {
    if (used[i]) {
      continue;
    }
    int edge_weight = graph_matrix_array[v_src * vertices_count + i];
    if (edge_weight < costs[i]) {
      costs[i] = edge_weight;
      edge_srcs[i] = v_src;
    }
  }
}

int find_smallest_cost_vertex(int* costs, bool* used, int vertices_count)
{
  int v_dst = -1;
  for (int i = 0; i < vertices_count; ++i) {
    if (!used[i] && (v_dst == -1 || costs[i] < costs[v_dst])) {
      v_dst = i;
    }
  }
  return v_dst;
}

void add_st_edge(int* st_edges, int* st_size, int v_src, int v_dst)
{
  st_edges[*st_size * 2] = v_src;
  st_edges[*st_size * 2 + 1] = v_dst;
  ++(*st_size);
}

ReturnCode find_mst_prim_naive(Graph* graph, int** st_edges, int* st_size)
{
  int vertices_count = graph->vertices_count;
  int edges_count = graph->edges_count;

  ReturnCode returnCode = 0;

  *st_edges = calloc((size_t)edges_count * 2, sizeof(int));
  bool* used = calloc(vertices_count, sizeof(bool));
  int* costs = initialize_cost(vertices_count);
  int* edge_srcs = calloc(vertices_count, sizeof(int));
  if (is_any_null(4, st_edges, used, costs, edge_srcs)) {
    returnCode = E_MEMORY_ALLOCATION_FAIL;
    goto cleanup_and_out;
  }

  costs[0] = 0;
  for (int i = 0; i < vertices_count; ++i) {
    int v_dst = find_smallest_cost_vertex(costs, used, vertices_count);
    int v_src = edge_srcs[v_dst];

    if (costs[v_dst] == INFINITY) {
      returnCode = E_NO_SPANNING_TREE;
      goto cleanup_and_out;
    }
    used[v_dst] = 1;

    if (i != 0) {
      add_st_edge(*st_edges, st_size, v_src, v_dst);
    }
    recalculate_smallest_costs(graph, used, costs, v_dst, edge_srcs);
  }

cleanup_and_out:
  free(used);
  free(costs);
  return returnCode;
}

void print_result(int* st_edges, int st_size)
{
  for (int i = 0; i < st_size; ++i) {
    int edge_src = st_edges[2 * i] + 1;
    int edge_dst = st_edges[2 * i + 1] + 1;
    printf("%d %d\n", edge_src, edge_dst);
  }
}

/* mst := minimum spanning tree */
int main(void)
{
  ReturnCode returnCode = 0;

  Graph* graph = NULL;
  returnCode = read_validate_graph(&graph);
  if (is_error(returnCode)) {
    goto cleanup_graph;
  }

  int* st_edges = NULL;
  int st_size = 0;
  returnCode = find_mst_prim_naive(graph, &st_edges, &st_size);
  if (is_error(returnCode)) {
    goto cleanup_spanning_tree;
  }

  print_result(st_edges, st_size);

cleanup_spanning_tree:
  free(st_edges);
cleanup_graph:
  free(graph);

  print_error_message(returnCode);
  return EXIT_SUCCESS;
}
