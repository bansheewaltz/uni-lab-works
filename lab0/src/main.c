#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_BASE 2
#define MAX_BASE 16
#define BASE_LIMITS MIN_BASE, MAX_BASE
#define MAX_INPUT_BUFFER (13 + 1)
#define MAX_INTEGER_OUTPUT_BUFFER (48 + 1)
#define MAX_FRACTIONAL_OUTPUT_BUFFER (12 + 1)
#define MAX_OUTPUT_BUFFER (MAX_FRACTIONAL_OUTPUT_BUFFER + MAX_INTEGER_OUTPUT_BUFFER)
#define DECIMAL_BASE 10
#define EXIT_SUCCESS 0
#define EXIT_ERROR 0

bool int_in_bounds(long long n, int left_bound, int right_bound) {
    return left_bound <= n && n <= right_bound;
}

void print_input_error(void) {
    printf("bad input");
}

char int_to_char(int num) {
    if (num >= 0 && num <= DECIMAL_BASE - 1) {
        return (char)(num + '0');
    } else {
        return (char)(num - DECIMAL_BASE + 'a');
    }
}

void string_reverse(char *string) {
    for (char *p1 = string, *p2 = string + strlen(string) - 1; p2 > p1; ++p1, --p2) {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
}

void decimal_to_base(char str_integer_part[], int base_new, long long int_integer_part) {
    int i = 0;

    while (int_integer_part > 0) {
        str_integer_part[i++] = int_to_char(int_integer_part % base_new);
        int_integer_part /= base_new;
    }

    str_integer_part[i] = '\0';
    string_reverse(str_integer_part);
}

bool char_in_bounds(char input, char left_limit, char right_limit) {
    return left_limit <= input && input <= right_limit;
}

bool digits_check(char input[], int base) {
    input[strcspn(input, "\n")] = '\0';
    size_t string_length = strlen(input);

    if (input[0] == '.' || input[string_length - 1] == '.') {
        return false;
    }

    bool comma_met = false;
    for (size_t i = 0; i < string_length; i++) {
        if (input[i] == '.') {
            if (comma_met == true) {
                return false;
            }
            comma_met = true;
            continue;
        }
        if ((!char_in_bounds(input[i], '0', int_to_char(base) - 1) &&
             !char_in_bounds(input[i], 'a', int_to_char(base) - 1))) {
            return false;
        }
    }

    return true;
}

void clear_input_buffer(void) {
    while (getchar() != '\n') {
    }
}

double to_power(double number, int power) {
    if (power < 0) {
        return 1 / to_power(number, -power);
    }

    double result = 1;

    while (power) {
        if (power & 1) {
            result *= number;
        }
        number *= number;
        power >>= 1;
    }

    return result;
}

void conversion(char input_buffer[], int b1, int b2, char result[]) {
    char *cursor = input_buffer;
    long long int_integer_part = strtoll(cursor, &cursor, b1);
    char str_integer_part[MAX_INTEGER_OUTPUT_BUFFER] = "0";
    if (int_integer_part) {
        decimal_to_base(str_integer_part, b2, int_integer_part);
    }
    strcat(result, str_integer_part);

    if (*cursor++ == '.') {
        char str_fractional_part[MAX_FRACTIONAL_OUTPUT_BUFFER + 1] = ".";
        double double_fractional_part = 0;

        char buffer[2] = "";
        for (int i = 0; cursor[i]; i++) {  // iterating the fraction string
            buffer[0] = cursor[i];
            double_fractional_part += strtol(buffer, 0, b1) * to_power(b1, -(i + 1));
        }

        for (int j = 1; j < MAX_FRACTIONAL_OUTPUT_BUFFER; ++j) {
            double_fractional_part *= b2;  // do multiply by base and store it in number.
            str_fractional_part[j] = int_to_char((int)double_fractional_part);  // store the int part.
            double_fractional_part -= (int)double_fractional_part;              // remove the int part.
        }
        strcat(result, str_fractional_part);
    }
}

int main(void) {
    int b1 = 0;
    int b2 = 0;

    if (!(scanf("%d %d", &b1, &b2) == 2 && int_in_bounds(b1, BASE_LIMITS) && int_in_bounds(b2, BASE_LIMITS))) {
        print_input_error();
        return EXIT_ERROR;
    }

    clear_input_buffer();
    char input_buffer[MAX_INPUT_BUFFER];
    if (fgets(input_buffer, MAX_INPUT_BUFFER, stdin) == NULL || !digits_check(input_buffer, b1)) {
        print_input_error();
        return EXIT_ERROR;
    }

    if (b1 == b2) {
        printf("%s", input_buffer);
    } else {
        char result[200] = "";
        conversion(input_buffer, b1, b2, result);
        printf("%s", result);
    }

    return EXIT_SUCCESS;
}
