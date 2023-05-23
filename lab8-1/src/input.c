#include <stdbool.h>
#include <stdio.h>

#include "error.h"
#include "main.h"
#include "utils.h"

typedef struct {
  int src;
  int dst;
  int weight;
} Edge;

ReturnCode scan_validate_vertices_count(int *vertices_count)
{
  int tmp = 0;
  if (scanf("%d", &tmp) && is_in_range(tmp, 0, VERTICES_MAX_COUNT)) {
    *vertices_count = tmp;
    return E_SUCCESS;
  }
  return E_BAD_NUMBER_OF_VERTICES;
}

ReturnCode scan_validate_edges_count(int *edges_count, int vertices_count)
{
  const int EDGES_MAX_COUNT = vertices_count * (vertices_count - 1) / 2;
  int tmp = 0;
  if (scanf("%d", &tmp) && is_in_range(tmp, 0, EDGES_MAX_COUNT)) {
    *edges_count = tmp;
    return E_SUCCESS;
  }
  return E_BAD_NUMBER_OF_EDGES;
}

ReturnCode scan_validate_parameters(int *vertices_count, int *edges_count)
{
  ReturnCode returnCode = 0;
  returnCode = scan_validate_vertices_count(vertices_count);
  if (returnCode != E_SUCCESS) {
    return returnCode;
  }
  returnCode = scan_validate_edges_count(edges_count, *vertices_count);
  if (returnCode != E_SUCCESS) {
    return returnCode;
  }
  return E_SUCCESS;
}

ReturnCode scan_validate_edge(Edge *edge, int vertices_count)
{
  int scanf_value = scanf("%d %d %d", &edge->src, &edge->dst, &edge->weight);
  if (scanf_value == EOF) {
    return EOF;
  }
  if (scanf_value != 3) {
    return E_BAD_INPUT_FORMAT;
  }
  if (!is_in_range(edge->src, 1, vertices_count)) {
    return E_BAD_VERTEX;
  }
  if (!is_in_range(edge->dst, 1, vertices_count)) {
    return E_BAD_VERTEX;
  }
  if (!is_in_range(edge->weight, 1, INT_MAX)) {
    return E_BAD_LENGTH;
  }
  return E_SUCCESS;
}

void graph_add_edge(Graph *graph, Edge *edge)
{
  int *graph_array = graph->graph_array;
  int vertices_count = graph->vertices_count;
  int src = edge->src;
  int dst = edge->dst;
  graph_array[(src - 1) * vertices_count + (dst - 1)] = edge->weight;
  graph_array[(dst - 1) * vertices_count + (src - 1)] = edge->weight;
}

ReturnCode scan_validate_edges(Graph *graph)
{
  Edge edge;
  int scanned_edges_count = 0;
  ReturnCode returnCode = E_SUCCESS;

  while (returnCode == E_SUCCESS) {
    returnCode = scan_validate_edge(&edge, graph->vertices_count);
    if (returnCode == (ReturnCode)EOF) {
      break;
    }
    if (is_error(returnCode)) {
      return returnCode;
    }
    ++scanned_edges_count;
    if (scanned_edges_count > graph->edges_count) {
      return E_BAD_NUMBER_OF_LINES;
    }
    graph_add_edge(graph, &edge);
  }

  if (scanned_edges_count < graph->edges_count) {
    return E_BAD_NUMBER_OF_LINES;
  }

  return E_SUCCESS;
}
