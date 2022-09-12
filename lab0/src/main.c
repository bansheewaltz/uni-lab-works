#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool char_read(char *c) {
    return scanf("%c", c);
}
bool char_is_digit(char input) {
    return '0' <= input && input <= '9';
}
bool clear_input_buffer(void) {
    int c;

    while ((c = getchar()) != '\n') {
        ;
    }
    return true;
}
bool in_bounds(int *n, int a, int b) {
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
void read_elements(int *array);
bool element_is_good(char input, int b1, bool *comma_met) {
    bool status = false;

    if (char_is_digit(input)) {
        int digit = (int)input - 48;
        if (digit < b1) {
            status = true;
        } else {
            status = false;
        }
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

int char_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return (int)c - '0';
    } else {
        return (int)c - 'A' + 10;
    }
}

char int_to_char(int num) {
    if (num >= 0 && num <= 9) {
        return (char)(num + '0');
    } else {
        return (char)(num - 10 + 'A');
    }
}

int base_to_decimal(char *string, int base) {
    int len = strlen(string);
    int power = 1;
    int result = 0;

    for (int i = len - 1; i >= 0; i--) {
        result += char_to_int(string[i]) * power;
        power *= base;
    }

    return result;
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

void print_result_string(char *string) {
    printf("%s", string);
}

void slice(const char *str, char *result, int start, int end) {
    strncpy(result, str + start, end - start);
}

int main(void) {
    int b1 = 0, b2 = 0;
    bool error = false;
    bool comma_met = false;
    int comma_position = 0;
    char string[13];

    if (b1b2_input_check(&b1, &b2)) {
        char input = 0;
        int i = 0;

        while (char_read(&input) && input != '\n' && !error) {
            if (element_is_good(input, b1, &comma_met)) {
                comma_position = i;
                string[i++] = input;
            } else {
                error = true;
            }
        }
        string[i] = '\0';
    } else {
        error = true;
    }

    if (error) {
        print_error();
    } else {
        if (b1 == b2) {
            print_result_string(string);
        } else if (comma_met) {
            int whole_part = base_to_decimal(string, b1);
            int fractional_part = base_to_decimal(string, b1);
            char whole_part_str[11];
            char fractional_part_str[11];

            decimal_to_base(whole_part_str, b2, whole_part);
            slice(string, fractional_part_str, comma_position, strlen(string));
            decimal_to_base(fractional_part_str, b2, fractional_part);
            strcpy(string, whole_part_str);
            //   string[comma_position] = '.';
            //   strcat(string, fractional_part_str);
            print_result_string(string);
        } else {
            int decimal_form = base_to_decimal(string, b1);
            printf("%d\n", decimal_form);
            decimal_to_base(string, b2, decimal_form);
            print_result_string(string);
        }
    }
}
