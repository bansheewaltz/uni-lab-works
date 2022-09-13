#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define MIN_N_OF_ELEMENTS 1
#define MAX_N_OF_ELEMENTS 10

bool int_read(long long *n) {
    return scanf("%lld", n);
}
bool in_bounds(long long n, int l, int r) {
    return l <= n && n <= r;
}
void print_error(void) {
    printf("bad input");
}
bool read_number_of_elements(long long *n) {
    return int_read(n) && in_bounds(*n, MIN_N_OF_ELEMENTS, MAX_N_OF_ELEMENTS);
}

int main(void) {
    long long n_elements_expected = 0;

    if (!read_number_of_elements(&n_elements_expected)) {
        print_error();
        return 0;
    }

    long long sum = 0;
    long long n_elements_read = 0;
    long long number = 0;

    while (int_read(&number) && in_bounds(number, INT_MIN, INT_MAX)) {
        if (!in_bounds(sum + number, INT_MIN, INT_MAX)) {
            printf("overflow");
            return 0;
        } else {
            sum += number;
            n_elements_read++;
            if (getchar() == '\n') {
                break;
            }
        }
    }

    if (n_elements_expected != n_elements_read) {
        print_error();
        return 0;
    }

    printf("%lld", sum);
    return 0;
}
