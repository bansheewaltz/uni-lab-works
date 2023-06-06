#include "vector.h"

TVector create_vector(int max, int size)
{
  TVector vector = {0, max, size, NULL};

  if (max > 0) {
    vector.Array = calloc(max, size);
    if (vector.Array == NULL) {
      other_error(__FILE__, __LINE__);
    }
  }

  return vector;
}

static bool is_empty_vector(TVector vector)
{
  return vector.Count == 0;
}

void assign_vector(int size, const void* src, void* dest)
{
  for (int i = 0; i < size; ++i) {
    ((char*)dest)[i] = ((char*)src)[i];
  }
}

void* get_nth_vector(TVector vector, int n)
{
  if (n >= vector.Max) {
    other_error(__FILE__, __LINE__);
  }

  return (char*)vector.Array + n * vector.Size;
}

TVector input_vector(int count, int size, int (*Scan)(void*))
{
  TVector vector = create_vector(count, size);

  for (int i = 0; i < count; ++i) {
    if (Scan(get_nth_vector(vector, i)) == EOF) {
      destroy_vector(&vector);
      bad_number_of_lines_error();
    }

    ++vector.Count;
  }

  return vector;
}

void destroy_vector(TVector* vector)
{
  if (!is_empty_vector(*vector)) {
    free(vector->Array);
  }

  vector->Array = NULL;
  vector->Count = 0;
  vector->Max = 0;
  vector->Size = 0;
}
