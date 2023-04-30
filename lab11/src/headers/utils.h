#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#define SUCCESS 1
#define FAILURE 0
#define DECIMAL_BASE 10

void print_error_terminate(char *message);
void if_fail(bool check, char *file, int line);
void array_int_print(int *arr, int arr_length, int alignment);
void array_bool_print(bool *arr, int arr_length, int alignment);
int *array_int_copy(const int *arr, int arr_length);
int *create_indices_array(int arr_length);
int array_int_max(const int *arr, int arr_size);
int count_int_digits(int n);
int array_int_alignment(int *arr, int arr_size);
void array_int_reorder(int *arr, int *indexes, int arr_length);
void array_bool_reorder(bool *arr, int *indexes, int arr_length);
int *create_reverse_mapping(int mapping[], int arr_length);
void print_table(int width, int table[][width], int objects_count, int values[],
                 int weights[]);

#endif  // UTILS_H
