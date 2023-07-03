#include "stack.h"

TStack stack_create(int max, int size)
{
  TStack stack = {0, max, size, NULL};

  if (max > 0) {
    stack.Array = calloc(max, size);
    if (stack.Array == NULL) {
      other_error(__FILE__, __LINE__);
    }
  }

  return stack;
}

bool stack_is_empty(TStack stack)
{
  return stack.Count == 0;
}

static bool stack_overflow(TStack stack)
{
  return stack.Count == stack.Max;
}

void stack_assign(int size, const void* src, void* dest)
{
  for (int i = 0; i < size; ++i) {
    ((char*)dest)[i] = ((char*)src)[i];
  }
}

void* stack_get_nth(TStack stack, int n)
{
  if (n >= stack.Max) {
    other_error(__FILE__, __LINE__);
  }

  return (char*)stack.Array + n * stack.Size;
}

void stack_push(void* value, TStack* stack)
{
  if (stack_overflow(*stack)) {
    other_error(__FILE__, __LINE__);
  }

  stack_assign(stack->Size, value, stack_get_nth(*stack, stack->Count));
  ++stack->Count;
}

void* stack_top(TStack stack)
{
  if (stack_is_empty(stack)) {
    other_error(__FILE__, __LINE__);
  }

  return stack_get_nth(stack, stack.Count - 1);
}

void* stack_next_top(TStack stack)
{
  if (stack.Count < 2) {
    other_error(__FILE__, __LINE__);
  }

  return stack_get_nth(stack, stack.Count - 2);
}

void* stack_pop(TStack* stack)
{
  if (stack_is_empty(*stack)) {
    other_error(__FILE__, __LINE__);
  }

  --stack->Count;
  return stack_get_nth(*stack, stack->Count);
}

void stack_destroy(TStack* stack)
{
  if (!stack_is_empty(*stack)) {
    free(stack->Array);
  }

  stack->Array = NULL;
  stack->Count = 0;
  stack->Max = 0;
  stack->Size = 0;
}

void stack_print(TStack stack, int (*Print)(const void*))
{
  while (!stack_is_empty(stack)) {
    if (Print(stack_pop(&stack)) == EOF) {
      other_error(__FILE__, __LINE__);
    }
  }
}
