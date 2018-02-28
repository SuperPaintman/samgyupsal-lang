#include "chunk.h"
#include "debug.h"
#include "value.h"
#include "vm.h"

/**
 * print 3 + 5
 */

int main() {
  MAKE_VM(vm);
  MAKE_CHUNK(chunk);

  int varA = addConstant(&chunk, 3);
  int varB = addConstant(&chunk, 5);

  writeChunk(&chunk, OP_LOAD_CONSTANT);
  writeChunk(&chunk, varA);
  writeChunk(&chunk, OP_LOAD_CONSTANT);
  writeChunk(&chunk, varB);
  writeChunk(&chunk, OP_ADD);
  writeChunk(&chunk, OP_PRINT);

  interpret(&vm, &chunk);
  // disassembleChunk(&chunk);

  freeChunk(&chunk);
  freeVM(&vm);

  return 0;
}
