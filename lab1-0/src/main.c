/* C Program for Bad Character Heuristic of Boyer
Moore String Matching Algorithm */
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PATTERN_INPUT_LIMIT 16
#define TEXT_MAX_LENGTH 1000
#define PATTERN_MAX_LENGTH (PATTERN_INPUT_LIMIT + 1 + 1)  // 1 for '\n' and 1 for '\0'
#define ASCII_SET (UCHAR_MAX + 1)                         // 1 for '0' value

void bad_char_heuristic(unsigned char *str, int pattern_size, int stop_symbols[]) {
    for (int i = 0; i < ASCII_SET; i++) {
        stop_symbols[i] = -1;
    }
    for (int i = 0; i < pattern_size; i++) {
        stop_symbols[(int)str[i]] = i;
    }
}

void search(unsigned char *text, unsigned char *pattern) {
    int length_pattern = strlen((const char *)pattern);
    int length_text = strlen((const char *)text);
    int stop_symbols[ASCII_SET];
    bad_char_heuristic(pattern, length_pattern, stop_symbols);

    int shift = 0;
    while (shift <= (length_text - length_pattern)) {
        int i = length_pattern - 1;

        for (; i >= 0 && printf("%d ", shift + i + 1) && pattern[i] == text[shift + i]; --i) {
        }

        if (i <= 0) {
            shift += length_pattern;
        } else if (i < length_pattern - 1) {
            shift += length_pattern - 1;
        } else {
            shift += length_pattern - 1 - stop_symbols[(int)text[shift + i]];
        }
    }
}

void get_pattern(unsigned char line[], int line_length) {
    if (fgets((char *)line, line_length, stdin) == NULL) {
        exit(EXIT_SUCCESS);  // ERROR
    }
    line[strlen((char *)line) - 1] = '\0';
}

void get_text(unsigned char line[], int line_length) {
    if (fread(line, 1, line_length, stdin) == 0) {
        exit(EXIT_SUCCESS);  // ERROR
    }
}

int main(void) {
    setbuf(stdout, NULL);  // for debugging purposes

    unsigned char pattern[PATTERN_MAX_LENGTH] = "";
    get_pattern(pattern, PATTERN_MAX_LENGTH);

    unsigned char text[TEXT_MAX_LENGTH] = "";
    get_text(text, TEXT_MAX_LENGTH);

    search(text, pattern);

    return EXIT_SUCCESS;
}
