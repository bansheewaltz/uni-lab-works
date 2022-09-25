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
            shift += length_pattern - 1 - stop_symbols[text[shift + i]];
    }
}

void clear_input_buffer(void) {
    while (getchar() != '\n') {
    }
}

int main(void) {
    char pattern[PATTERN_MAX_LENGTH] = "";
    // char buffer1;
    // char buffer2;
    // sscanf("%c", &buffer1, &buffer2);
    // if (buffer1 == "\\" && buffer2 == "x") {
    //     ungetc(buffer2, stdin);
    //     ungetc(buffer1, stdin);
    //     scanf("\\x%2hhx", &next)
    // }
    // ungetc(buffer2, stdin);
    // ungetc(buffer1, stdin);
    fgets(pattern, PATTERN_MAX_LENGTH, stdin);
    // printf("%s", pattern);
    // if (pattern[strlen(pattern) - 1] != '\n') {
    //     clear_input_buffer();
    // }
    pattern[strcspn(pattern, "\n")] = '\0';

    char text[TEXT_MAX_LENGTH] = "";
    fgets(text, TEXT_MAX_LENGTH, stdin);
    text[strcspn(text, "\n")] = '\0';

    search(text, pattern);

    return EXIT_SUCCESS;
}
