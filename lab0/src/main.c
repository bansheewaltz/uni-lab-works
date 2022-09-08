#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool char_read(char *c) { return scanf("%c", c); }
bool char_is_digit(char input) { return '0' <= input && input <= '9'; }
bool clear_input_buffer(void) {
  int c;

  while ((c = getchar()) != '\n')
    ;
  return true;
}
bool in_bounds(int *n, int a, int b) { return a <= *n && *n <= b; }
bool underflow_check(int current_n, int limit) { return current_n < limit; }
bool overflow_check(int current_n, int limit) { return current_n > limit; }
bool sum_overflow_check(int a, int b) {
  return ((a > 0 && b > INT_MAX - a) || (a <= 0 && b < INT_MIN - a));
}
void print_error(void) { printf("bad input"); }
void read_elements(int *array);
bool element_is_good(char input, int b1, bool *comma_met) {
  bool status = false;

  if (char_is_digit(input)) {
    int digit = (int)input - 48;
    if (digit < b1)
      status = true;
    else
      status = false;

  } else if (input == '.' && !*comma_met) {
    *comma_met = true;
    status = true;
  } else {
    status = false;
  }

  return status;
}

bool b1b2_input_check(int *b1, int *b2) {
  return scanf("%d %d", b1, b2) == 2 && in_bounds(b1, 2, 16) &&
         in_bounds(b2, 2, 16) && clear_input_buffer();
}

int main(void) {
  int b1 = 0, b2 = 0;
  bool error = false;
  bool comma_met = false;
  char string[13];

  if (b1b2_input_check(&b1, &b2)) {
    char input = 0;
    int i = 0;

    while (char_read(&input) && input != '\n' && !error) {
      if (element_is_good(input, b1, &comma_met))
        string[i++] = input;
      else
        error = true;
    }
    string[i] = '\0';

  } else
    error = true;

  if (error)
    printf("bad input");
  else
    printf("%s", string);
}
