#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_BASE 2
#define MAX_BASE 16
#define BASE_LIMITS MIN_BASE, MAX_BASE
#define MAX_BUFFER (13 + 1)
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
void clear_input_buffer(void) {
    while (getchar() != '\n') {
    }
}
bool is_digit(char cursor) {
    return '0' <= cursor && cursor <= '9';
}
int main(void) {
    int b1 = 0;
    int b2 = 0;

    if (!(scanf("%d %d", &b1, &b2) == 2 && int_in_bounds(b1, BASE_LIMITS) && int_in_bounds(b2, BASE_LIMITS))) {
        print_input_error();
        return EXIT_ERROR;
    }

    clear_input_buffer();
    char input_buffer[MAX_BUFFER];
    if (fgets(input_buffer, MAX_BUFFER, stdin) == NULL || !digits_check(input_buffer, b1)) {
        print_input_error();
        return EXIT_ERROR;
    }

    if (b1 == b2) {
        if (!is_digit(input_buffer[strcspn(input_buffer, ".") + 1]) ||
            !is_digit(input_buffer[strcspn(input_buffer, ".") - 1])) {
            print_input_error();
            return EXIT_ERROR;
        }
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        printf("%s", input_buffer);
        return EXIT_SUCCESS;
    }

    char *cursor = input_buffer;
    int whole_part = strtol(cursor, &cursor, b1);
    char result[13] = "";
    if (*cursor == '.') {
        float f = 0;
        cursor++;
        char n[2] = {0};
        for (int i = 0; cursor[i]; i++) {  // iterating the fraction string
            n[0] = cursor[i];
            f += strtol(n, 0, b1) * pow(b1, -(i + 1));  // converting the fraction part
        }
        int j = 0;
        while (f > 0)  // do multiply and get the int part until number is zero
        {
            f *= b2;                    // do multiply by base and store it in number.
            result[j++] = (int)f + 48;  // store the int part.
            f -= (int)f;                // remove the int part.
        }
    }
    char part1[15];
    decimal_to_base(part1, b2, whole_part);
    printf("%s.%s", part1, result);
    return EXIT_SUCCESS;
}
