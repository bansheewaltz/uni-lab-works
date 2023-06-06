#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"

typedef struct TStack {
  int Count;
  int Max;
  int Size;
  void* Array;
} TStack;

TStack stack_create(int max, int size);
bool stack_is_empty(TStack stack);
void stack_assign(int size, const void* src, void* dest);
void* stack_get_nth(TStack stack, int n);
void stack_push(void* value, TStack* stack);
void* stack_top(TStack stack);
void* stack_next_top(TStack stack);
void* stack_pop(TStack* stack);
void stack_destroy(TStack* stack);
void stack_print(TStack stack, int (*Print)(const void*));

#endif
