#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool int_read(long long *n) { return scanf("%lld", n); }
bool char_read(char *c) { return scanf("%c", c); }
bool in_bounds(long long *n, int a, int b) { return a <= *n && *n <= b; }
void print_error(void) { printf("bad input"); }
bool clear_input_buffer(void) {
  int c;

  while ((c = getchar()) != '\n')
    ;
  return true;
}
bool overflow_check(int current_n, int limit) { return current_n > limit; }
bool underflow_check(int current_n, int limit) { return current_n < limit; }
// bool sum_overflow_check(int a, int b) {
//   int sum = a + b;
//   if (a >= 0 && b > 0 && sum < 0) return true;
//   if (a <= 0 && b < 0 && sum > 0) return true;
//   return false;
// }

// bool sum_overflow_check(int a, int b) {
//   return b > 0 ? a > INT_MAX - b : a > INT_MAX + b;
// }

// bool sum_overflow_check(int a, int b) {
//   int larger_num = a >= b ? a : b;
//   int smaller_num = a < b ? a : b;
//   a = larger_num;
//   b = smaller_num;

//   return b > 0 ? a > INT_MAX - b : a > INT_MAX + b;
// }

bool sum_overflow_check(int a, int b) {
  // int larger_num = a >= b ? a : b;
  // int smaller_num = a < b ? a : b;
  // a = larger_num;
  // b = smaller_num;

  if (a > 0 && b > INT_MAX - a) return 1;
  if (a < 0 && b < INT_MIN - a)
    return 1;
  else
    return 0;
}

bool read_number_of_elements(long long *n) {
  return int_read(n) && in_bounds(n, 1, 10) && clear_input_buffer();
}
void read_elements(int *array);
bool char_is_digit(char input) {
  return ('1' <= input && input <= '9') || input == '-';
}
bool element_is_good(char input) {
  return (char_is_digit(input) || input == ' ');
}
void array_print(int *array, int array_size) {
  for (int i = 0; i < array_size; ++i) {
    printf("%d%s", array[i], i == array_size - 1 ? "" : " ");
  }
}

int main(void) {
  long long number_of_elements = 0;
  bool error = false;
  int *array = NULL;
  int sum = 0;

  if (read_number_of_elements(&number_of_elements)) {
    array = malloc(sizeof(int) * number_of_elements);
    char input = 0;
    int i = 0;

    while (char_read(&input) && element_is_good(input) && !error) {
      if (char_is_digit(input)) {
        long long number = 0;
        ungetc(input, stdin);
        if (int_read(&number) && in_bounds(&number, INT_MIN, INT_MAX)) {
          array[i++] = number;
          if (sum_overflow_check(sum, number))
            error = true;
          else
            sum += number;
        } else
          error = true;

        if (overflow_check(i, number_of_elements)) error = true;
      }
    }

    if (underflow_check(i, number_of_elements)) error = true;
  } else {
    error = true;
  }

  if (error)
    print_error();
  else
    printf("%d", sum);

  free(array);
  array = 0;
  return 0;
}
