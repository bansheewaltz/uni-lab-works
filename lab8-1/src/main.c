#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "input.h"
#include "typehandlers.h"
#include "utils.h"

ReturnCode read_validate_graph(Graph* graph)
{
  ReturnCode returnCode = 0;

  int vertices_count = 0;
  int edges_count = 0;
  returnCode = scan_validate_parameters(&vertices_count, &edges_count);
  if (is_error(returnCode)) {
    return returnCode;
  }

  graph = graph_init(vertices_count, edges_count);
  if (graph == NULL) {
    returnCode = E_MEMORY_ALLOCATION_FAIL;
    return returnCode;
  }

  returnCode = scan_validate_edges(graph);
  if (is_error(returnCode)) {
    return returnCode;
  }

  returnCode = E_SUCCESS;
  return returnCode;
}

int* initialize_cost(int vertices_count)
{
  int* cost = malloc(sizeof(int) * (size_t)vertices_count);
  if (cost != NULL) {
    for (int i = 0; i < vertices_count; ++i) {
      cost[i] = INFINITY;
    }
  }
  return cost;
}

void recalculate_smallest_edges(Graph* graph, bool* used, int* cost, int v)
{
  int vertices_count = graph->vertices_count;
  int* graph_matrix_array = graph->graph_array;
  for (int i = 0; i < vertices_count; ++i) {
    int edge_weight = graph_matrix_array[i * vertices_count + v];
    if (!used[i] && edge_weight < cost[i]) {
      cost[i] = edge_weight;
    }
  }
}

int find_smallest_edge(int* cost, bool* used, int vertices_count)
{
  int v_dst = -1;
  for (int i = 0; i < vertices_count; i++) {
    if (!used[i] && (v_dst == -1 || cost[i] < cost[v_dst])) {
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

ReturnCode find_mst_prim_naive(Graph* graph, int* st_edges, int st_size)
{
  int vertices_count = graph->vertices_count;

  int returnCode = 0;

  bool* used = calloc((size_t)vertices_count, sizeof(bool));
  int* cost = initialize_cost(vertices_count);
  if (is_any_null(2, used, cost)) {
    returnCode = E_MEMORY_ALLOCATION_FAIL;
    goto out;
  }
  cost[0] = 0;

  for (int i = 0; i < vertices_count; ++i) {
    int v_src = i;
    int v_dst = find_smallest_edge(cost, used, vertices_count);
    used[v_dst] = 1;

    if (cost[v_dst] == INFINITY) {
      returnCode = E_NO_SPANNING_TREE;
      break;
    }
    add_st_edge(st_edges, &st_size, v_src, v_dst);
    recalculate_smallest_edges(graph, used, cost, v_dst);
  }

out:
  free(used);
  free(cost);
  return returnCode;
}

void print_result(int* st_edges, int st_size)
{
  for (int i = 0; i < st_size; ++i) {
    printf("%d %d\n", st_edges[2 * i], st_edges[2 * i + 1]);
  }
}

int main(void)
{
  ReturnCode returnCode = 0;
  Graph* graph = NULL;
  int* st_edges = NULL;

  returnCode = read_validate_graph(graph);
  if (is_error(returnCode)) {
    goto cleanup_and_exit;
  }

  int st_size = 0;
  returnCode = find_mst_prim_naive(graph, st_edges, st_size);
  if (is_error(returnCode)) {
    goto cleanup_and_exit;
  }

  print_result(st_edges, st_size);

cleanup_and_exit:
  if (is_error(returnCode)) {
    print_error_message(returnCode);
  }
  free(st_edges);
  free(graph);
  return EXIT_SUCCESS;
}
