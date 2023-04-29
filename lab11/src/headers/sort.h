#ifndef SORT_H
#define SORT_H

#include <stdbool.h>

#define COUNTING_SORT_BASE 10
#define DESCENDING true
#define ASCENDING false

void radix_sort_int_int(int arr[], int arr_by_which[], int arr_length,
                        bool order);

#endif  // SORT_H
