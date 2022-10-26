#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_LIMIT 10  // "0123456789"
#define INPUT_MAX_SIZE (INPUT_LIMIT + 1 + 1)  // 1 for '\n' and 1 for '\0' chars because of fgets use

bool check_occurrences(const char input[], int len_input) {
    int repetitions[10] = {0};

    for (int i = 0; i < len_input; ++i) {
        if (!isdigit(input[i]) || repetitions[input[i] - '0']++) {
            return false;
        }
    }

    return true;
}

void swap(char* a, char* b) {
    *a ^= *b;
    *b = *a ^ *b;
    *a ^= *b;
}

void reverse(char str[], int s, int e) {  // 's' for the "start", 'e' for the "end"
    for (; s < e; swap(&str[s], &str[e]), ++s, --e) {
    }
}

bool generate_next_perm(char str[], int len_str) {
    int i = len_str - 2;
    for (; i >= 0 && str[i + 1] <= str[i]; i--) {
    }

    if (str[i + 1] > str[i] && i >= 0) {
        int j = len_str - 1;
        for (; j >= 0 && str[j] <= str[i]; --j) {
        }

        swap(&str[i], &str[j]);
        reverse(str, i + 1, len_str - 1);
        return true;
    }

    return false;
}

int main() {
    char input[INPUT_MAX_SIZE] = "";
    int len_input = 0;
    int count = 0;

    if (!fgets(input, INPUT_MAX_SIZE, stdin) || !scanf("%d", &count) || !(count >= 0) || !strchr(input, '\n') ||
        !(len_input = strlen(input) - 1) || !check_occurrences(input, len_input)) {
        printf("bad input\n");
        return EXIT_SUCCESS;  // but actually ERROR
    }

    for (int i = 0; generate_next_perm(input, len_input) && i < count; ++i) {
        printf("%s", input);
    }

    return EXIT_SUCCESS;
}
