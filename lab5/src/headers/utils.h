#ifndef UTILS_H_
#define UTILS_H_

#include "stddef.h"
#include "typedefs.h"

int calculate_alphabet_size(int *freq_dictionary);
void deallocate_chars_info(CharInfo **chars_info_array, size_t alphabet_size);
CharInfo *create_and_initialize_char_info(uchar character);
CharInfo **get_chars_info_consistent(CharInfo **chars_info_dictionary,
                                     size_t alphabet_size);
#endif  // UTILS_H_
