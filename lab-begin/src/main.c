#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define MIN_N_OF_ELEMENTS 1
#define MAX_N_OF_ELEMENTS 10

bool int_read(int *n) {
    return scanf("%d", n);
}
bool int_in_bounds(const int *n, int l, int r) {
    return l <= *n && *n <= r;
}
bool long_long_read(long long *n) {
    return scanf("%lld", n);
}
bool int_overflow_check(const long long *n, int a, int b) {
    return a <= *n && *n <= b;
}
bool sum_overflow_check(int a, int b) {
    return !((a > 0 && b > INT_MAX - a) || (a <= 0 && b < INT_MIN - a));
}
void print_error(void) {
    printf("bad input");
}
bool read_number_of_elements(int *n) {
    return int_read(n) && int_in_bounds(n, MIN_N_OF_ELEMENTS, MAX_N_OF_ELEMENTS);
}

int main(void) {
    int n_elements_expected = 0;

    if (read_number_of_elements(&n_elements_expected)) {
        int sum = 0;
        int n_elements_read = 0;
        long long number = 0;

        while (long_long_read(&number) && int_overflow_check(&number, INT_MIN, INT_MAX)) {
            if (!sum_overflow_check(sum, number)) {
                printf("overflow");
                return -1;
            } else {
                sum += number;
                n_elements_read++;
                if (getchar() == '\n') {
                    break;
                }
            }
        }
        if (n_elements_expected == n_elements_read) {
            printf("%d", sum);
            return 0;
        }
    }

    print_error();
    return -1;
}
