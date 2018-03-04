#ifndef SAMGYUPSAL_CHUNK_H_
#define SAMGYUPSAL_CHUNK_H_

// Includes
#include <stdint.h>

#include "value.h"
#include "vector.h"

// Types
typedef enum {
  OP_LOAD_CONSTANT,

  OP_ADD,

  OP_PRINT,

  OP_RETURN
} OpCode;

VECTOR_TEMPLATE_DECLARATIONS(CodeVector, uint8_t);
VECTOR_TEMPLATE_DECLARATIONS(ConstantVector, Value);

typedef struct {
  CodeVector code;
  ConstantVector constants;
} Chunk;

// Macros
#define MAKE_CHUNK(name)                                                       \
  Chunk name;                                                                  \
  initChunk(&name)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Declarations
void initChunk(Chunk *chunk);
void freeChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte);
uint32_t addConstant(Chunk *chunk, Value value);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SAMGYUPSAL_CHUNK_H_
