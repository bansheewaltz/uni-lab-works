#ifndef SORT_H
#define SORT_H

#include <stdbool.h>

#define COUNTING_SORT_BASE 10
#define DESCENDING true
#define ASCENDING false

int *array_int_radix_sort(int arr[], int arr_length, bool order);

#endif  // SORT_H
