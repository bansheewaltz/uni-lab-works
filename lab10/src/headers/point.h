#ifndef POINT_H
#define POINT_H

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  int X;
  int Y;
} TPoint;

int scan_point(void* value);
int print_point(const void* value);
int compare_point_x(const void* a, const void* b);
int compare_point_y(const void* a, const void* b);

#endif
