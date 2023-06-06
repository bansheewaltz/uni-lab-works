#include "andrew.h"

static int compare_point_lower(const void* a, const void* b)
{
  const TPoint* A = a;
  const TPoint* B = b;

  int difference = compare_point_x(A, B);
  if (difference == 0) {
    difference = compare_point_y(A, B);
  }

  return (difference > 0) ? 1 : (difference == 0) ? 0 : -1;
}

static int compare_point_upper(const void* a, const void* b)
{
  const TPoint* A = a;
  const TPoint* B = b;

  int difference = compare_point_x(B, A);
  if (difference == 0) {
    difference = compare_point_y(B, A);
  }

  return (difference > 0) ? 1 : (difference == 0) ? 0 : -1;
}

static TPoint find_min(TVector vector)
{
  TPoint min = {INT_MAX, INT_MAX};

  for (int i = 0; i < vector.Count; ++i) {
    if (compare_point_x(get_nth_vector(vector, i), &min) == -1 ||
        (compare_point_x(get_nth_vector(vector, i), &min) == 0 &&
         compare_point_y(get_nth_vector(vector, i), &min) == -1)) {
      min = *((TPoint*)get_nth_vector(vector, i));
    }
  }

  return min;
}

static TPoint find_max(TVector vector)
{
  TPoint max = {INT_MIN, INT_MIN};

  for (int i = 0; i < vector.Count; ++i) {
    if (compare_point_x(&max, get_nth_vector(vector, i)) == -1 ||
        (compare_point_x(&max, get_nth_vector(vector, i)) == 0 &&
         compare_point_y(&max, get_nth_vector(vector, i)) == -1)) {
      max = *((TPoint*)get_nth_vector(vector, i));
    }
  }

  return max;
}

static long double vector_multiplication(TPoint a, TPoint b, TPoint c)
{
  long double dx1 = (long double)b.X - a.X;
  long double dx2 = (long double)c.X - a.X;
  long double dy1 = (long double)b.Y - a.Y;
  long double dy2 = (long double)c.Y - a.Y;

  return (dx1 * dy2) - (dy1 * dx2);
}

static TVector create_lower_vector(TVector vector, TPoint min, TPoint max)
{
  TVector set = create_vector(vector.Count, vector.Size);

  int index = 0;
  for (int i = 0; i < vector.Count; ++i) {
    long double mult = vector_multiplication(min, max, *((TPoint*)get_nth_vector(vector, i)));
    if (mult < 0) {
      assign_vector(vector.Size, get_nth_vector(vector, i), get_nth_vector(set, index));
      ++index;
    }
  }

  assign_vector(vector.Size, &min, get_nth_vector(set, index));
  assign_vector(vector.Size, &max, get_nth_vector(set, index + 1));
  set.Count = index + 2;

  return set;
}

static TVector create_upper_vector(TVector vector, TPoint min, TPoint max)
{
  TVector set = create_vector(vector.Count, vector.Size);

  int index = 0;
  for (int i = 0; i < vector.Count; ++i) {
    long double mult = vector_multiplication(min, max, *((TPoint*)get_nth_vector(vector, i)));
    if (mult > 0) {
      assign_vector(vector.Size, get_nth_vector(vector, i), get_nth_vector(set, index));
      ++index;
    }
  }

  assign_vector(vector.Size, &min, get_nth_vector(set, index));
  assign_vector(vector.Size, &max, get_nth_vector(set, index + 1));
  set.Count = index + 2;

  return set;
}

static bool rotation_check(TPoint a, TPoint b, TPoint c)
{
  return vector_multiplication(a, b, c) > 0;
}

static TStack graham_algorithm(TVector vector, int (*compare)(const void*, const void*))
{
  TStack stack = stack_create(vector.Count, vector.Size);

  if (vector.Count == 0) {
    return stack;
  }
  if (vector.Count == 1) {
    stack_push(vector.Array, &stack);
    return stack;
  }

  qsort(vector.Array, vector.Count, vector.Size, compare);

  stack_push(get_nth_vector(vector, 0), &stack);
  stack_push(get_nth_vector(vector, 1), &stack);

  for (int i = 2; i < vector.Count; ++i) {
    while (stack.Count > 1 &&
           !rotation_check(*((TPoint*)stack_top(stack)), *((TPoint*)get_nth_vector(vector, i)),
                           *((TPoint*)stack_next_top(stack)))) {
      stack_pop(&stack);
    }

    stack_push(get_nth_vector(vector, i), &stack);
  }

  return stack;
}

void andrew_algorithm(TVector vector)
{
  if (vector.Count == 0) {
    return;
  }

  if (vector.Count == 1) {
    if (print_point(vector.Array) == EOF) {
      other_error(__FILE__, __LINE__);
    }
    return;
  }

  TPoint min = find_min(vector);
  TPoint max = find_max(vector);

  TVector lowerVector = create_lower_vector(vector, min, max);
  TStack lowerStack = graham_algorithm(lowerVector, compare_point_lower);
  destroy_vector(&lowerVector);
  stack_pop(&lowerStack);
  stack_print(lowerStack, print_point);
  stack_destroy(&lowerStack);

  TVector upperVector = create_upper_vector(vector, min, max);
  TStack upperStack = graham_algorithm(upperVector, compare_point_upper);
  destroy_vector(&upperVector);
  stack_pop(&upperStack);
  stack_print(upperStack, print_point);
  stack_destroy(&upperStack);
}
