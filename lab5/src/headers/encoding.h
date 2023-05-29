#ifndef ENCODING_H_
#define ENCODING_H_

#include <stdio.h>

#include "typedefs.h"

void archive_file_text_form(TreeNode *tree, CodingInfo *codingInfo,
                            FILE *input, FILE *output);

#endif  // ENCODING_H_
