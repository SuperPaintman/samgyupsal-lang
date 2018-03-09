#ifndef SAMGYUPSAL_VM_H_
#define SAMGYUPSAL_VM_H_

// Includes
#include <stdint.h>

#include "chunk.h"
#include "value.h"

// Macros
#define STACK_SIZE 128

#define MAKE_VM(name)                                                          \
  VM name;                                                                     \
  initVM(&name)

// Types
typedef struct {
  uint32_t sp;
  Value stack[STACK_SIZE];
  Object *objects;
} VM;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Definitions
void initVM(VM *vm);
void freeVM(VM *vm);
void interpret(VM *vm, Chunk *chunk);
int interpretInstruction(VM *vm, Chunk *chunk, int offset);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SAMGYUPSAL_VM_H_
