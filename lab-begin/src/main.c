#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool char_read(char *c) {
    return scanf("%c", c);
}
bool char_is_digit(char input) {
    return ('1' <= input && input <= '9') || input == '-';
}
bool int_read(long long *n) {
    return scanf("%lld", n);
}
bool clear_input_buffer(void) {
    int c;

    while ((c = getchar()) != '\n') {
        ;
    }
    return true;
}
bool in_bounds(long long *n, int a, int b) {
    return a <= *n && *n <= b;
}
bool underflow_check(int current_n, int limit) {
    return current_n < limit;
}
bool overflow_check(int current_n, int limit) {
    return current_n > limit;
}
bool sum_overflow_check(int a, int b) {
    return ((a > 0 && b > INT_MAX - a) || (a <= 0 && b < INT_MIN - a));
}
void print_error(void) {
    printf("bad input");
}
bool read_number_of_elements(long long *n) {
    return int_read(n) && in_bounds(n, 1, 10) && clear_input_buffer();
}
bool element_is_good(char input) {
    return (char_is_digit(input) || input == ' ');
}

int main(void) {
    long long number_of_elements = 0;
    bool error = false;
    int *array = NULL;
    int sum = 0;
    bool error_outputed = false;

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

                    if (sum_overflow_check(sum, number)) {
                        error = true;
                        printf("overflow");
                        error_outputed = true;
                    } else {
                        sum += number;
                    }
                } else {
                    error = true;
                }

                if (overflow_check(i, number_of_elements)) {
                    error = true;
                }
            }
        }

        if (underflow_check(i, number_of_elements)) {
            error = true;
        }
    } else {
        error = true;
    }

    if (!error_outputed) {
        if (error) {
            print_error();
        } else {
            printf("%d", sum);
        }
    }

    free(array);
    array = 0;
    return 0;
}
