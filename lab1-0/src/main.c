/* C Program for Bad Character Heuristic of BoyerMoore String Matching Algorithm */
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATTERN_INPUT_LIMIT 16
#define PATTERN_SIZE (PATTERN_INPUT_LIMIT + 1 + 1)  // 1 for '\n' and 1 for '\0' chars
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

int read_pattern(uchar line[], int len_limit) {
    line = (uchar *)fgets((char *)line, len_limit, stdin);
    int len_pattern = strlen((char *)line);
    if (line == NULL || !strcspn((char *)line, "\n")) {
        exit(EXIT_SUCCESS);  // but actually ERROR
    }

    line[len_pattern - 1] = '\0';  // deleting '\n' char
    return --len_pattern;
}

int update_buffer(uchar *buffer, bool *is_buffer_full) {
    uchar *buffer_half = (*is_buffer_full) ? buffer : buffer + TEXT_BUFFER_SIZE / 2;
    *is_buffer_full ^= 1;
    int n_chars_read = fread(buffer_half, sizeof(uchar), TEXT_BUFFER_SIZE / 2, stdin);

    return n_chars_read;
}

void search_substring(uchar text[], uchar pattern[], int len_pattern, int *n_chars_to_process, bool *is_buffer_full) {
    int shift_table[ASCII_SET];
    bad_char_heuristic(pattern, len_pattern, shift_table);

    int len_text = *n_chars_to_process;
    int char_ndx = len_pattern - 1;  // index of the comparison character in the text
    while (char_ndx <= len_text - 1) {
        for (int i = 0; i < len_pattern; ++i) {
            printf("%d ", char_ndx - i + 1);
            if (pattern[len_pattern - i - 1] != text[(char_ndx - i) % TEXT_BUFFER_SIZE]) {
                break;
            }
        }

        char_ndx += shift_table[(int)text[char_ndx % TEXT_BUFFER_SIZE]];
        if (char_ndx > len_text - 1) {
            len_text += update_buffer(text, is_buffer_full);
        }
    }
}

int main(void) {
    // setbuf(stdout, NULL);  // for debugging purposes
    uchar pattern[PATTERN_SIZE] = "";
    int len_pattern = read_pattern(pattern, PATTERN_SIZE);

    uchar text[TEXT_BUFFER_SIZE] = "";
    bool is_buffer_full = true;
    int n_chars_to_process = fread(text, sizeof(uchar), TEXT_BUFFER_SIZE, stdin);

    if (len_pattern > n_chars_to_process) {
        return EXIT_SUCCESS;  // but actually ERROR
    }

    search_substring(text, pattern, len_pattern, &n_chars_to_process, &is_buffer_full);

    return EXIT_SUCCESS;
}
