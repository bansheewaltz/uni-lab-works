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
  ReturnCode return_value = 0;

  int vertices_count = -1;
  int edges_count = -1;
  return_value = scan_validate_parameters(&vertices_count, &edges_count);
  if (is_error(return_value)) {
    return return_value;
  }

  *graph = graph_init(vertices_count, edges_count);
  if (*graph == NULL) {
    return_value = E_MEMORY_ALLOCATION_FAIL;
    return return_value;
  }

  return_value = scan_validate_edges(*graph);
  if (is_error(return_value)) {
    return return_value;
  }

  return E_SUCCESS;
}

uint* initialize_cost(int vertices_count)
{
  uint* costs = malloc(sizeof(int) * (size_t)vertices_count);
  if (costs == NULL) {
    return NULL;
  }
  for (int i = 0; i < vertices_count; ++i) {
    costs[i] = INFINITY;
  }
  return costs;
}

void recalculate_smallest_costs(Graph* graph, bool* used, uint* costs,
                                int v_src, int* edge_srcs)
{
  int vertices_count = graph->vertices_count;
  uint* graph_matrix_array = graph->graph_array;
  for (int i = 0; i < vertices_count; ++i) {
    if (used[i]) {
      continue;
    }
    uint edge_weight = (uint)graph_matrix_array[v_src * vertices_count + i];
    if (edge_weight < costs[i]) {
      costs[i] = edge_weight;
      edge_srcs[i] = v_src;
    }
  }
}

int find_smallest_cost_vertex(uint const* costs, bool const* used,
                              int vertices_count)
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

  ReturnCode return_value = 0;

  *st_edges = calloc((size_t)edges_count * 2, sizeof(int));
  bool* used = calloc((size_t)vertices_count, sizeof(bool));
  uint* costs = initialize_cost((size_t)vertices_count);
  int* edge_srcs = calloc((size_t)vertices_count, sizeof(int));
  if (is_any_null(4, st_edges, used, costs, edge_srcs)) {
    return_value = E_MEMORY_ALLOCATION_FAIL;
    goto cleanup_and_out;
  }

  costs[0] = 0;
  for (int i = 0; i < vertices_count; ++i) {
    int v_dst = find_smallest_cost_vertex(costs, used, vertices_count);
    int v_src = edge_srcs[v_dst];

    if (costs[v_dst] == INFINITY) {
      return_value = E_NO_SPANNING_TREE;
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
  return return_value;
}

void print_result(int const* st_edges, int st_size)
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
  ReturnCode return_value = 0;

  Graph* graph = NULL;
  return_value = read_validate_graph(&graph);
  if (is_error(return_value)) {
    goto cleanup_graph;
  }

  int* st_edges = NULL;
  int st_size = 0;
  return_value = find_mst_prim_naive(graph, &st_edges, &st_size);
  if (is_error(return_value)) {
    goto cleanup_spanning_tree;
  }

  print_result(st_edges, st_size);

cleanup_spanning_tree:
  free(st_edges);
cleanup_graph:
  graph_free(graph);

  print_error_message(return_value);
  return EXIT_SUCCESS;
}
