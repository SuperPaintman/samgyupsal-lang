#ifndef SAMGYUPSAL_CHUNK_H_
#define SAMGYUPSAL_CHUNK_H_

// Includes
#include <stdint.h>

#include "value.h"

// Types
typedef enum {
  OP_LOAD_CONSTANT,

  OP_ADD,

  OP_PRINT,

  OP_RETURN
} OpCode;

typedef struct {
  uint32_t count;
  uint32_t capacity;
  uint8_t *code;
  ValueArray constants;
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
int addConstant(Chunk *chunk, Value value);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SAMGYUPSAL_CHUNK_H_
