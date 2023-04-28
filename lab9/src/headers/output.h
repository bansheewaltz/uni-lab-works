#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>

void output_path_len_to_each_v(PathInfo *pathInfo, FILE *output);
void output_shortest_path(PathInfo *pathInfo, FILE *output);
void deallocate_path_info(PathInfo *pathInfo);

#endif  // OUTPUT_H
