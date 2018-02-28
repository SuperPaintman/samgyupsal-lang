#ifndef SAMGYUPSAL_VALUE_H_
#define SAMGYUPSAL_VALUE_H_

// Types
typedef double Value;

typedef struct {
  int capacity;
  int count;
  Value *values;
} ValueArray;

// Macros
#define MAKE_VALUE_ARRAY(name)                                                 \
  ValueArray name;                                                             \
  initVarueArray(&name)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Declarations
void initVarueArray(ValueArray *array);
void freeVarueArray(ValueArray *array);
void writeVarueArray(ValueArray *array, Value value);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SAMGYUPSAL_VALUE_H_
