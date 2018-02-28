#include "chunk.h"

#include <stdint.h>
#include <stdlib.h>

#include "memory.h"
#include "util.h"

void initChunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  initVarueArray(&chunk->constants);
}

void freeChunk(Chunk *chunk) {
  ARRAY_FREE(uint8_t, chunk->code, chunk->capacity);
  freeVarueArray(&chunk->constants);
  initChunk(chunk);
}

void writeChunk(Chunk *chunk, uint8_t byte) {
  if (chunk->capacity < chunk->count + 1) {
    // Grow capacity
    uint32_t originCapacity = chunk->capacity;
    chunk->capacity = CAPACITY_GROW(originCapacity);
    chunk->code =
        ARRAY_GROW(chunk->code, uint8_t, originCapacity, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;
  chunk->count++;
}

int addConstant(Chunk *chunk, Value value) {
  writeVarueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}
