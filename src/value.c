#include "value.h"

#include <stdint.h>
#include <stdlib.h>

#include "memory.h"
#include "util.h"

void initVarueArray(ValueArray *array) {
  array->values = NULL;
  array->capacity = 0;
  array->count = 0;
}

void freeVarueArray(ValueArray *array) {
  ARRAY_FREE(Value, array->values, array->capacity);

  initVarueArray(array);
}

void writeVarueArray(ValueArray *array, Value value) {
  if (array->capacity < array->count + 1) {
    // Grow capacity
    uint32_t originCapacity = array->capacity;
    array->capacity = CAPACITY_GROW(originCapacity);
    array->values =
        ARRAY_GROW(array->values, Value, originCapacity, array->capacity);
  }

  array->values[array->count] = value;
  array->count++;
}
