/* C Program for Bad Character Heuristic of Boyer
Moore String Matching Algorithm */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PATTERN_MAX_LENGTH (16 + 1 + 1)  // 1 for '\n' and 1 for '\0'
#define TEXT_MAX_LENGTH (100 + 1)
#define NO_OF_CHARS 256
#define EXIT_ERROR 0

void bad_char_heuristic(char *str, int pattern_size, int stop_symbols[]) {
    for (int i = 0; i < NO_OF_CHARS; i++) {
        stop_symbols[i] = -1;
    }
    for (int i = 0; i < pattern_size; i++) {
        stop_symbols[(int)str[i]] = i;
    }
}

void search(char *text, char *pattern) {
    int length_pattern = strlen(pattern);
    int length_text = strlen(text);
    int stop_symbols[NO_OF_CHARS];
    bad_char_heuristic(pattern, length_pattern, stop_symbols);

    int shift = 0;
    while (shift <= (length_text - length_pattern)) {
        int i = length_pattern - 1;

        while (i >= 0 && printf("%d ", shift + i + 1) && pattern[i] == text[shift + i]) {
            --i;
        }

        if (i < 0)
            shift += length_pattern;
        else if (i < length_pattern - 1)
            shift += length_pattern - 1;
        else
            shift += length_pattern - 1 - stop_symbols[(int)text[shift + i]];
    }
}

void clear_input_buffer(void) {
    while (getchar() != '\n') {
    }
}

void get_line(char line[], int line_length) {
    if (fgets(line, line_length, stdin) == NULL) {
        exit(EXIT_ERROR);
    }
    line[strcspn(line, "\n")] = '\0';
}

int main(void) {
    char pattern[PATTERN_MAX_LENGTH] = "";
    get_line(pattern, PATTERN_MAX_LENGTH);

    char text[TEXT_MAX_LENGTH] = "";
    get_line(text, TEXT_MAX_LENGTH);

    search(text, pattern);

    return EXIT_SUCCESS;
}
