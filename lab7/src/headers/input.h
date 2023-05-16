#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

#include "main.h"

bool scan_validate_vertices_count(int *vertices_count);
bool scan_validate_edges_count(int *edges_count, int vertices_count);
bool graph_scan_validate_parameters(int *vertices_count, int *edges_count);
bool validate_edge(int src, int dst, int vertices_count);
bool graph_scan_validate_edges(Graph *graph);

#endif  // INPUT_H
