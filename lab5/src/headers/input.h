#ifndef INPUT_H_
#define INPUT_H_

// #include <stddef.h>
#include <stdio.h>

#include "typedefs.h"

void scan_chars_frequencies_from_file(CharInfo **chars_info, FILE *input);
int read_alphabet_size(FILE *input);
size_t get_alphabet_size(CharInfo **chars_info);

#endif  // INPUT_H_
