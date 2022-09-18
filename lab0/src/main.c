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
bool digits_check(char input[], int base, bool *comma_met) {
    input[strcspn(input, "\n")] = '\0';

    for (int i = 0; i < (int)strlen(input); i++) {
        if (input[i] == '.') {
            if (*comma_met == true || i == 0 || i == (int)strlen(input) - 1) {
                return false;
            }
            *comma_met = true;
            continue;
        } else if ((!char_in_bounds(input[i], '0', int_to_char(base) - 1) &&
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
        printf("%s", input_buffer);
        return EXIT_SUCCESS;
    }

    char *cursor = input_buffer;
    long long int_integer_part = strtoll(cursor, &cursor, b1);
    char str_fractional_part[14] = "";
    if (*cursor++ == '.') {
        int j = 0;
        float float_fractional_part = 0;
        str_fractional_part[j++] = '.';
        char n[2] = {0};

        for (int i = 0; cursor[i]; i++) {  // iterating the fraction string
            n[0] = cursor[i];
            float_fractional_part += strtol(n, 0, b1) * pow(b1, -(i + 1));  // converting the fraction part
        }

        while (j < MAX_FRACTIONAL_OUTPUT_BUFFER) {  // do multiply and get the int part until number is zero
            float_fractional_part *= b2;            // do multiply by base and store it in number.
            str_fractional_part[j++] = int_to_char((int)float_fractional_part);  // store the int part.
            float_fractional_part -= (int)float_fractional_part;                 // remove the int part.
        }
    }

    char str_integer_part[MAX_INTEGER_OUTPUT_BUFFER] = "0";
    if (int_integer_part) {
        decimal_to_base(str_integer_part, b2, int_integer_part);
    }
    printf("%s%s", str_integer_part, str_fractional_part);

    return EXIT_SUCCESS;
}
