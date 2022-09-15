#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MIN_BASE 1
#define MAX_BASE 16
#define BASE_LIMITS MIN_BASE, MAX_BASE
#define INT_LIMITS INT_MIN, INT_MAX
#define MAX_BUFFER 13

bool int_read(long long *n) {
    return scanf("%lld", n);
}
bool int_in_bounds(long long n, int left_bound, int right_bound) {
    return left_bound <= n && n <= right_bound;
}
bool read_number_of_elements(long long *n) {
    return int_read(n) && int_in_bounds(*n, BASE_LIMITS);
}
void print_input_error(void) {
    printf("bad input");
}
char int_to_char(int num) {
    if (num >= 0 && num <= 9) {
        return (char)(num + '0');
    } else {
        return (char)(num - 10 + 'A');
    }
}
void string_reverse(char *string) {
    int len = strlen(string);

    for (int i = 0; i < len / 2; i++) {
        char temp = string[i];
        string[i] = string[len - i - 1];
        string[len - i - 1] = temp;
    }
}
void decimal_to_base(char result[], int base, int decimal_num) {
    int i = 0;

    while (decimal_num > 0) {
        result[i++] = int_to_char(decimal_num % base);
        decimal_num /= base;
    }

    result[i] = '\0';
    string_reverse(result);
}
bool digits_check(const char *input, int base) {
    for (int i = 0; i < (int)strlen(input); i++) {
        if (input[i] - 48 >= base) {
            return false;
        }
    }
    return true;
}
int main(void) {
    // char input_buffer[MAX_BUFFER];
    // fgets(input_buffer, MAX_BUFFER, stdin);
    // char *cursor = input_buffer;
    // int b1 = strtol(cursor, &cursor, 10);
    // int b2 = strtol(cursor, &cursor, 10);
    // if (!int_in_bounds(b1, MIN_BASE, MAX_BASE) || !int_in_bounds(b2, MIN_BASE, MAX_BASE)) {
    //     print_input_error();
    //     return 0;
    // }
    // printf("%d %d", b1, b2);
    int b1 = 0;
    int b2 = 0;

    if (!(scanf("%d %d", &b1, &b2) == 2 && int_in_bounds(b1, BASE_LIMITS) && int_in_bounds(b2, INT_LIMITS))) {
        print_input_error();
        return 0;
    }

    fflush(stdin);
    char input_buffer[MAX_BUFFER];
    if (fgets(input_buffer, MAX_BUFFER, stdin) == NULL) {
        print_input_error();
        return 0;
    }

    if (!digits_check(input_buffer, b1)) {
        print_input_error();
        return 0;
    }
    if (b1 == b2) {
        input_buffer[strlen(input_buffer) - 1] = '\0';
        printf("%s", input_buffer);
        return 0;
    }

    char *cursor = input_buffer;
    int whole_part = strtol(cursor, &cursor, b1);
    int fractional_part = 0;
    if (*cursor == '.') {
        fractional_part = strtol(cursor + 1, &cursor, b2);
    }
    char part1[15];
    char part2[15];
    decimal_to_base(part1, b2, whole_part);
    decimal_to_base(part2, b2, fractional_part);
    printf("%s.%s", part1, part2);
    // printf("%d.%d", whole_part, fractional_part);
}
