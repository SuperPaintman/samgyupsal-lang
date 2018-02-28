#ifndef SAMGYUPSAL_UTIL_H_
#define SAMGYUPSAL_UTIL_H_

// Macros
#define ARRAY_GROW(prev, type, oldCount, newCount)                             \
  (type *)reallocate(prev, sizeof(type) * (oldCount), sizeof(type) * (newCount))

#define ARRAY_FREE(type, pointer, count)                                       \
  reallocate(pointer, sizeof(type) * (count), 0)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Declarations
void *reallocate(void *prev, size_t oldSize, size_t newSize);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SAMGYUPSAL_UTIL_H_
