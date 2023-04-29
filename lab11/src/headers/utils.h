#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#define SUCCESS 1
#define FAILURE 0

void print_error_terminate(char *message);
void if_fail(bool check, char *file, int line);
void print_int_array(int *arr, int arr_length);
void copy_int_array(int *arr, int arr_length);

#endif  // UTILS_H
