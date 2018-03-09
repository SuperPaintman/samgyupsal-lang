#include "chunk.h"

#include <stdint.h>
#include <stdlib.h>

#include "vector.h"

VECTOR_TEMPLATE_IMPLEMENTATIONS(CodeVector, uint8_t);
VECTOR_TEMPLATE_IMPLEMENTATIONS(ConstantVector, Value);

void initChunk(Chunk *chunk) {
  VECTOR_INIT(CodeVector, &chunk->code);
  VECTOR_INIT(ConstantVector, &chunk->constants);
  chunk->objects = NULL;
}

void freeChunk(Chunk *chunk) {
  VECTOR_FREE(ConstantVector, &chunk->constants);
  VECTOR_FREE(CodeVector, &chunk->code);
  freeObjects(chunk->objects);
  initChunk(chunk);
}

void writeChunk(Chunk *chunk, uint8_t byte) {
  VECTOR_ADD(CodeVector, &chunk->code, byte);
}

uint32_t addConstant(Chunk *chunk, Value value) {
  return VECTOR_ADD(ConstantVector, &chunk->constants, value);
}
