#ifndef UTILS_H
#define UTILS_H

#define SUCCESS 1
#define FAILURE 0

void print_error_terminate(char message[]);
void print_allocation_failed_terminate(char *file, int line);

#endif  // UTILS_H
