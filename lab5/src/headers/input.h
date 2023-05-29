#ifndef INPUT_H_
#define INPUT_H_

// #include <stddef.h>
#include <stdio.h>

#include "typedefs.h"

void scan_chars_frequencies_from_input(CharInfo **dictionary, FILE *input);
int read_alphabet_size_checked(FILE *input);
size_t get_alphabet_size(CharInfo **chars_info);

#endif  // INPUT_H_
