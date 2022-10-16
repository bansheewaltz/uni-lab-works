/* C Program for Bad Character Heuristic of Boyer
Moore String Matching Algorithm */
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATTERN_INPUT_LIMIT 16
#define PATTERN_SIZE (PATTERN_INPUT_LIMIT + 1 + 1)  // 1 for '\n' and 1 for '\0'
#define TEXT_BUFFER_SIZE 1000
#define ASCII_SET 256
typedef unsigned char uchar;

void bad_char_heuristic(uchar *pattern, int len_pattern, int shift_table[]) {
    for (int i = 0; i < ASCII_SET; ++i) {
        shift_table[i] = len_pattern;
    }
    for (int i = 0; i < len_pattern - 1; ++i) {
        shift_table[(int)pattern[i]] = len_pattern - i - 1;
    }
}

void read_pattern(uchar line[], int len_line) {
    if (fgets((char *)line, len_line, stdin) == NULL) {
        exit(EXIT_SUCCESS);  // but actually ERROR
    }

    int temp = strcspn((char *)line, "\n");
    if (temp == 0) {
        exit(EXIT_SUCCESS);  // but actually ERROR}
    }

    line[temp] = '\0';
}

int update_buffer(uchar *buffer, bool *is_buffer_full) {
    uchar *destination = (*is_buffer_full) ? buffer : buffer + TEXT_BUFFER_SIZE / 2;
    *is_buffer_full ^= 1;
    int n_chars_read = fread(destination, sizeof(uchar), TEXT_BUFFER_SIZE / 2, stdin);
    return n_chars_read;
}

void search_substring(uchar text[], uchar pattern[], int *n_chars_to_process, bool *is_buffer_full) {
    int len_pattern = strlen((const char *)pattern);
    int shift_table[ASCII_SET];
    bad_char_heuristic(pattern, len_pattern, shift_table);

    int len_text = *n_chars_to_process;
    int shift = len_pattern - 1;
    while (shift < len_text) {
        for (int i = 0; i < len_pattern; ++i) {
            printf("%d ", shift - i + 1);
            if (pattern[len_pattern - i - 1] != text[(shift - i) % TEXT_BUFFER_SIZE]) {
                break;
            }
        }

        shift += shift_table[(int)text[shift % TEXT_BUFFER_SIZE]];
        if (shift >= len_text) {
            len_text += (*n_chars_to_process = update_buffer(text, is_buffer_full));
        }
    }
}

int main(void) {
    // setbuf(stdout, NULL);  // for debugging purposes
    uchar pattern[PATTERN_SIZE] = "";
    read_pattern(pattern, PATTERN_SIZE);

    uchar text[TEXT_BUFFER_SIZE] = "";
    bool is_buffer_full = true;
    int n_chars_to_process = fread(text, sizeof(uchar), TEXT_BUFFER_SIZE, stdin);

    if (n_chars_to_process == 0) {
        return EXIT_SUCCESS;
    }

    search_substring(text, pattern, &n_chars_to_process, &is_buffer_full);

    return EXIT_SUCCESS;
}
