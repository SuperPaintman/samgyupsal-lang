#ifndef SAMGYUPSAL_VALUE_H_
#define SAMGYUPSAL_VALUE_H_

// Includes
#include <stdbool.h>

// Macros
#define IS_NONE(value) ((value).type == VALUE_NONE)
#define IS_BOOLEAN(value) ((value).type == VALUE_BOOLEAN)
#define IS_NUMBER(value) ((value).type == VALUE_NUMBER)

#define AS_NONE(value) ((value).as.none)
#define AS_BOOLEAN(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)

#define MAKE_NONE() ((Value){.type = VALUE_NONE, .as = {.none = 0}})
#define MAKE_BOOLEAN(value)                                                    \
  ((Value){.type = VALUE_BOOLEAN, .as = {.boolean = value}})
#define MAKE_NUMBER(value)                                                     \
  ((Value){.type = VALUE_NUMBER, .as = {.number = value}})

// Types
typedef enum { VALUE_NONE, VALUE_BOOLEAN, VALUE_NUMBER } ValueType;

typedef struct {
  ValueType type;
  union {
    unsigned none : 1;
    bool boolean;
    double number;
  } as;
} Value;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SAMGYUPSAL_VALUE_H_
