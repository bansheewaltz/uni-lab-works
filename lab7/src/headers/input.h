#ifndef INPUT_H
#define INPUT_H

bool scan_validate_vertices_count(int *vertices_count);
bool scan_validate_edges_count(int *edges_count, int vertices_count);
bool scan_validate_parameters(int *vertices_count, int *edges_count);
bool validate_edge(int src, int dst, int vertices_count);
bool scan_edges(bool *graph_array, int vertices_count, int edges_count);

#endif  // INPUT_H
