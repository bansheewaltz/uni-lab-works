#include <stdbool.h>
#include <stdio.h>

#include "main.h"
#include "utils.h"

bool scan_validate_vertices_count(int *vertices_count)
{
  int tmp = 0;
  if (scanf("%d", &tmp) && is_in_range(tmp, 0, VERTICES_MAX_COUNT)) {
    *vertices_count = tmp;
    return SUCCESS;
  }
  puts("bad number of vertices");
  return FAILURE;
}

bool scan_validate_edges_count(int *edges_count, int vertices_count)
{
  int tmp = 0;
  const int EDGES_MAX_COUNT = vertices_count * (vertices_count - 1) / 2;
  if (scanf("%d", &tmp) && is_in_range(tmp, 0, EDGES_MAX_COUNT)) {
    *edges_count = tmp;
    return SUCCESS;
  }
  puts("bad number of edges");
  return FAILURE;
}

bool scan_validate_parameters(int *vertices_count, int *edges_count)
{
  if (scan_validate_vertices_count(vertices_count) &&
      scan_validate_edges_count(edges_count, *vertices_count)) {
    return SUCCESS;
  }
  return FAILURE;
}

bool validate_edge(int src, int dst, int vertices_count)
{
  if (is_in_range(src, 1, vertices_count) &&
      is_in_range(dst, 1, vertices_count)) {
    return true;
  }
  return false;
}

bool scan_validate_edges(Graph *graph)
{
  int vertices_count = graph->vertices_count;
  int edges_count = graph->edges_count;

  int edge_src = 0;
  int edge_dst = 0;
  int scanned_edges_count = 0;
  bool result = SUCCESS;
  bool bad_vertex_flag = false;

  while (scanf("%d %d", &edge_src, &edge_dst) != EOF) {
    if (validate_edge(edge_src, edge_dst, vertices_count)) {
      graph->graph_array[--edge_src * vertices_count + --edge_dst] = 1;
    } else {
      puts("bad vertex");
      result = FAILURE;
      bad_vertex_flag = true;
      break;
    }

    ++scanned_edges_count;
    if (scanned_edges_count > edges_count) {
      break;
    }
  }

  if (scanned_edges_count != edges_count || edges_count == 0) {
    result = FAILURE;
    if (bad_vertex_flag == false) {
      puts("bad number of lines");
    }
  }

  return result;
}
