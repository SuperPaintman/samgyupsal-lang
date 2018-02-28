#ifndef SAMGYUPSAL_MEMORY_H_
#define SAMGYUPSAL_MEMORY_H_

// Macro
#define CAPACITY_MIN 8
#define CAPACITY_FACTOR 2
#define CAPACITY_GROW(capacity)                                                \
  ((capacity) < CAPACITY_MIN ? CAPACITY_MIN : (capacity)*CAPACITY_FACTOR)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SAMGYUPSAL_MEMORY_H_
