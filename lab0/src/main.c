#include <math.h>
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
    if (num >= 0 && num <= 9) {
        return (char)(num + '0');
    } else {
        return (char)(num - 10 + 'a');
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
void decimal_to_base(char str_fractional_part[], int base, long long decimal_num) {
    int i = 0;

    while (decimal_num > 0) {
        str_fractional_part[i++] = int_to_char(decimal_num % base);
        decimal_num /= base;
    }

    str_fractional_part[i] = '\0';
    string_reverse(str_fractional_part);
}
bool char_in_bounds(char input, char left_limit, char right_limit) {
    return left_limit <= input && input <= right_limit;
}
bool digits_check(const char *input, int base, bool *comma_met) {
    for (int i = 0; i < (int)strlen(input) - 1; i++) {  // -1 to don't read the newline character
        if (input[i] == '.') {
            if (*comma_met == true) {
                return false;
            }
            *comma_met = true;
            continue;
        } else if ((!char_in_bounds(input[i], '0', int_to_char(base)) &&
                    !char_in_bounds(input[i], 'a', int_to_char(base)))) {
            return false;
        }
        // if (is_lowercase_letter(input[i]) && input[i] - 'a' >= base - DECIMAL_BASE) {

        // } else if ('A' <= input[i] && input[i] <= 'F' && input[i] - 'A' >= base - 10) {
        //     return false;
        // } else if ('0' <= input[i] && input[i] <= '9' && input[i] - '0' >= base) {
        //     return false;
        // }  // else if
    }

    return true;
}
void clear_input_buffer(void) {
    while (getchar() != '\n') {
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
    bool comma_met = false;
    if (fgets(input_buffer, MAX_INPUT_BUFFER, stdin) == NULL || !digits_check(input_buffer, b1, &comma_met)) {
        print_input_error();
        return EXIT_ERROR;
    }

    if (b1 == b2) {
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        printf("%s", input_buffer);
        return EXIT_SUCCESS;
    }

    char *cursor = input_buffer;
    long long int_integer_part = strtoll(cursor, &cursor, b1);
    char str_fractional_part[14] = "";
    float float_fractional_part = 0;
    int j = 0;
    if (*cursor++ == '.') {
        str_fractional_part[j++] = '.';
        char n[2] = {0};

        for (int i = 0; cursor[i]; i++) {  // iterating the fraction string
            n[0] = cursor[i];
            float_fractional_part += strtol(n, 0, b1) * pow(b1, -(i + 1));  // converting the fraction part
        }

        while (float_fractional_part > 0)  // do multiply and get the int part until number is zero
        {
            float_fractional_part *= b2;                                 // do multiply by base and store it in number.
            str_fractional_part[j++] = (int)float_fractional_part + 48;  // store the int part.
            float_fractional_part -= (int)float_fractional_part;         // remove the int part.
        }

        for (int i = strlen(str_fractional_part); strlen(str_fractional_part) != 1 + 12; i++) {
            str_fractional_part[i] = '0';
        }
    }
    char str_integer_part[MAX_INTEGER_OUTPUT_BUFFER];
    decimal_to_base(str_integer_part, b2, int_integer_part);
    printf("%s%s", str_integer_part, str_fractional_part);

    return EXIT_SUCCESS;
}
