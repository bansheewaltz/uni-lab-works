#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>

void print_path_info(PathInfo *pathInfo, int n_vertices, FILE *output);
void deallocate_path_info(PathInfo *pathInfo);

#endif  // OUTPUT_H
