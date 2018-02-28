#include "debug.h"

#include <stdio.h>

#include "value.h"

// Internal Macros
#define CASE_SIMPLE_OPCODE(name, offset)                                       \
  case name:                                                                   \
    return disassembleSimpleInstruction(#name, offset)

static void printValue(Value value) { printf("%g", value); }

static int disassembleSimpleInstruction(const char *name, int offset) {
  printf("%s\n", name);

  return offset + 1;
}

static int disassembleConstantInstruction(const char *name, Chunk *chunk,
                                          int offset) {
  uint8_t constant = chunk->code[offset + 1];

  printf("%-16s %4d (", name, constant);

  printValue(chunk->constants.values[constant]);

  printf(")\n");

  return offset + 2;
}

void disassembleChunk(Chunk *chunk) {
  printf("=== Chunk Start ===\n");

  int i = 0;
  while (i < chunk->count) {
    i = disassembleInstruction(chunk, i);
  }

  printf("=== Chunk End ===\n");
}

int disassembleInstruction(Chunk *chunk, int offset) {
  printf("%04d ", offset);

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
    CASE_SIMPLE_OPCODE(OP_RETURN, offset);

    CASE_SIMPLE_OPCODE(OP_ADD, offset);

    CASE_SIMPLE_OPCODE(OP_PRINT, offset);

  case OP_LOAD_CONSTANT:
    return disassembleConstantInstruction("OP_LOAD_CONSTANT", chunk, offset);

  default:
    printf("Unknown opcode: %d\n", instruction);
    return offset + 1;
  }
}
