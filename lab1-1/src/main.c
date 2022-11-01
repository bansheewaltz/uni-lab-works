/* Rabin-Karp algorithm protocol printing program*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATTERN_INPUT_LIMIT 16
#define PATTERN_SIZE (PATTERN_INPUT_LIMIT + 1 + 1)  // 1 for '\n' and 1 for '\0' because of fgets and strlen use
#define TEXT_BUFFER_SIZE 500  // min = PATTERN_INPUT_LIMIT * 2 because of buffer updates by halfs
typedef unsigned char uchar;

int update_buffer(char buffer[], size_t window_pos) {
    char *buffer_half = window_pos ? buffer + TEXT_BUFFER_SIZE / 2 : buffer;
    return fread(buffer_half, sizeof(char), TEXT_BUFFER_SIZE / 2, stdin);
}

void search_substring(const char pattern[], int len_pattern, char text[], int len_text_buf) {
    int hash_pattern = 0;
    int hash_window = 0;  // window as in 'sliding window technique'
    int pow_max = 1;

    for (int i = 0; i < len_pattern; ++i) {  // hashing
        hash_pattern += ((uchar)pattern[i] % 3) * pow_max;
        hash_window += ((uchar)text[i] % 3) * pow_max;
        pow_max *= 3;
    }
    pow_max /= 3;
    printf("%d ", hash_pattern);

    size_t len_text = len_text_buf;
    for (size_t window_pos = 0; window_pos <= len_text - len_pattern; ++window_pos) {  // sliding window iteration
        if (hash_pattern == hash_window) {  // collision check
            for (int i = 0; i < len_pattern; ++i) {
                printf("%zu ", window_pos + i + 1);  // protocol printing
                if (pattern[i] != text[(window_pos + i) % TEXT_BUFFER_SIZE]) {
                    break;
                }
            }
        }

        hash_window = (hash_window - (uchar)text[window_pos % TEXT_BUFFER_SIZE] % 3) / 3 +  // hash update
                      (uchar)text[(window_pos + len_pattern) % TEXT_BUFFER_SIZE] % 3 * pow_max;
        if (window_pos + len_pattern == len_text - 1) {  // buffer update
            len_text += update_buffer(text, len_text % TEXT_BUFFER_SIZE);
        }
    }
}

int main(void) {
    setbuf(stdout, NULL);  // for debugging purposes

    char pattern[PATTERN_SIZE] = "";
    if (!fgets(pattern, PATTERN_SIZE, stdin) || !strchr(pattern, '\n')) {  // emptiness and overflow check
        return EXIT_SUCCESS;  // but actually FAILURE
    }
    int len_pattern = strlen(pattern) - 1;  // to ignore '\n' char

    char text[TEXT_BUFFER_SIZE] = "";
    int len_text_buf = fread(text, sizeof(char), TEXT_BUFFER_SIZE, stdin);
    if (len_pattern > len_text_buf || !len_pattern) {  // only '\n' pattern check
        len_text_buf ? 0 : printf("0");  // check if text is empty
        return EXIT_SUCCESS;  // but actually FAILURE
    }

    search_substring(pattern, len_pattern, text, len_text_buf);

    return EXIT_SUCCESS;
}
