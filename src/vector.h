#ifndef SAMGYUPSAL_VECTOR_H_
#define SAMGYUPSAL_VECTOR_H_

// Includes
#include <stdint.h>
#include <stdlib.h>

// Macros
#define VECTOR_CAPACITY_INITIAL 8
#define VECTOR_CAPACITY_GROW(n) n * 2

#define MAKE_VECTOR(type, name)                                                \
  type name;                                                                   \
  vectorInit##type(&name)

#define VECTOR_TEMPLATE_DECLARATIONS(name, type)                               \
  typedef struct {                                                             \
    uint32_t count;                                                            \
    uint32_t capacity;                                                         \
    type *items;                                                               \
  } name;                                                                      \
                                                                               \
  void vectorInit##name(name *vector);                                         \
  void vectorFree##name(name *vector);                                         \
  uint32_t vectorCount##name(name *vector);                                    \
  uint32_t vectorAdd##name(name *vector, type item);                           \
  type vectorGet##name(name *vector, uint32_t index);                          \
  void vectorResize##name(name *vector, uint32_t capacity)

#define VECTOR_TEMPLATE_IMPLEMENTATIONS(name, type)                            \
  void vectorInit##name(name *vector) {                                        \
    vector->count = 0;                                                         \
    vector->capacity = VECTOR_CAPACITY_INITIAL;                                \
    vector->items = NULL;                                                      \
  }                                                                            \
                                                                               \
  void vectorFree##name(name *vector) { free(vector->items); }                 \
                                                                               \
  uint32_t vectorCount##name(name *vector) { return vector->count; }           \
                                                                               \
  uint32_t vectorAdd##name(name *vector, type item) {                          \
    if (vector->items == NULL) {                                               \
      vector->items = malloc(sizeof(type) * vector->capacity);                 \
    }                                                                          \
                                                                               \
    if (vector->count == vector->capacity) {                                   \
      vectorResize##name(vector, VECTOR_CAPACITY_GROW(vector->capacity));      \
    }                                                                          \
                                                                               \
    vector->items[vector->count++] = item;                                     \
                                                                               \
    return vector->count - 1;                                                  \
  }                                                                            \
                                                                               \
  type vectorGet##name(name *vector, uint32_t index) {                         \
    /*                                                                         \
    if (index >= vector->count) {                                              \
      return (type)NULL;                                                       \
    }                                                                          \
    */                                                                         \
                                                                               \
    return vector->items[index];                                               \
  }                                                                            \
                                                                               \
  void vectorResize##name(name *vector, uint32_t capacity) {                   \
    type *items = realloc(vector->items, sizeof(type) * capacity);             \
                                                                               \
    if (!items) {                                                              \
      /** @todo: add error handling */                                         \
      return;                                                                  \
    }                                                                          \
                                                                               \
    vector->items = items;                                                     \
    vector->capacity = capacity;                                               \
  }

#define VECTOR_INIT(type, vector) vectorInit##type(vector)
#define VECTOR_FREE(type, vector) vectorFree##type(vector)
#define VECTOR_COUNT(type, vector) vectorCount##type(vector)
#define VECTOR_ADD(type, vector, item) vectorAdd##type(vector, item)
#define VECTOR_GET(type, vector, index) vectorGet##type(vector, index)
#define VECTOR_RESIZE(type, vector, capacity)                                  \
  vectorResize##type(vector, capacity)

// Types
typedef void *VectorItem;

VECTOR_TEMPLATE_DECLARATIONS(Vector, VectorItem);

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Declarations
inline void initVector(Vector *vector) { VECTOR_INIT(Vector, vector); }
inline void freeVector(Vector *vector) { VECTOR_FREE(Vector, vector); }
inline uint32_t vectorCount(Vector *vector) {
  return VECTOR_COUNT(Vector, vector);
}
inline uint32_t vectorAdd(Vector *vector, VectorItem item) {
  return VECTOR_ADD(Vector, vector, item);
}
inline VectorItem vectorGet(Vector *vector, uint32_t index) {
  return VECTOR_GET(Vector, vector, index);
}
inline void vectorResize(Vector *vector, uint32_t capacity) {
  VECTOR_RESIZE(Vector, vector, capacity);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SAMGYUPSAL_VECTOR_H_
