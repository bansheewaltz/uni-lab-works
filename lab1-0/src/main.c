/* C Program for Bad Character Heuristic of Boyer
Moore String Matching Algorithm */
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#define PATTERN_MAX_LENGTH (16 + 1 + 1)  // 1 for '\n' and 1 for '\0'
#define TEXT_MAX_LENGTH (1000)
#define NO_OF_CHARS 128
#define EXIT_SUCCESS 0

void bad_char_heuristic(unsigned char *str, int pattern_size, int stop_symbols[]) {
    for (int i = 0; i < NO_OF_CHARS; i++) {
        stop_symbols[i] = -1;
    }
    for (int i = 0; i < pattern_size; i++) {
        stop_symbols[(int)str[i]] = i;
    }
}

void search(unsigned char *text, unsigned char *pattern) {
    int length_pattern = strlen((const char *)pattern);
    int length_text = strlen((const char *)text);
    int stop_symbols[NO_OF_CHARS];
    bad_char_heuristic(pattern, length_pattern, stop_symbols);

    int shift = 0;
    while (shift <= (length_text - length_pattern)) {
        int i = length_pattern - 1;

        while (i >= 0 && printf("%d ", shift + i + 1) && pattern[i] == text[shift + i]) {
            // fflush(stdout);
            --i;
        }

        if (i <= 0)
            shift += length_pattern;
        else if (i < length_pattern - 1)
            shift += length_pattern - 1;
        else
            shift += length_pattern - 1 - stop_symbols[(int)text[shift + i]];
    }
}

bool get_pattern(unsigned char line[], int line_length) {
    if (fgets((char *)line, line_length, stdin) == NULL) {
        return false;
    }
    line[strlen((char *)line) - 1] = '\0';

    return true;
}

bool get_text(unsigned char line[], int line_length) {
    if (fread(line, 1, line_length, stdin) == 0) {
        return false;
    }
    // line[strlen(line) - 1] = '\0';

    return true;
}

int main(void) {
    setbuf(stdout, NULL);  // for debugging purposes

    unsigned char pattern[PATTERN_MAX_LENGTH] = "";
    if (!get_pattern(pattern, PATTERN_MAX_LENGTH)) {
        return EXIT_SUCCESS;  // ERROR
    }

    unsigned char text[TEXT_MAX_LENGTH] = "";
    if (!get_text(text, TEXT_MAX_LENGTH)) {
        return EXIT_SUCCESS;  // ERROR
    }

    search(text, pattern);

    return EXIT_SUCCESS;
}
