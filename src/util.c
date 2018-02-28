#include "util.h"

#include <stdlib.h>

void *reallocate(void *prev, size_t oldSize, size_t newSize) {
  return realloc(prev, newSize);
}
