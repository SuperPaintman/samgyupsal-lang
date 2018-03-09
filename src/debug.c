#include "debug.h"

#include <stdio.h>

#include "value.h"
#include "vector.h"

// Internal Macros
#define CASE_SIMPLE_OPCODE(name, offset)                                       \
  case name:                                                                   \
    return disassembleSimpleInstruction(#name, offset)

static void printValue(Value value) {
  switch (value.type) {
  case VALUE_NONE:
    printf("None");
    break;
  case VALUE_BOOLEAN:
    printf(AS_BOOLEAN(value) ? "True" : "False");
    break;
  case VALUE_NUMBER:
    printf("%g", AS_NUMBER(value));
    break;
  case VALUE_OBJECT:
    if (!IS_STRING(value)) {
      printf("UnexpectedObject");
      break;
    }

    printf("\"%s\"", AS_CSTRING(value));
    break;
  default:
    // Unreachable
    printf("???");
  }
}

static int disassembleSimpleInstruction(const char *name, int offset) {
  printf("%s\n", name);

  return offset + 1;
}

static int disassembleConstantInstruction(const char *name, Chunk *chunk,
                                          int offset) {
  uint8_t constant = VECTOR_GET(CodeVector, &chunk->code, offset + 1);

  printf("%-16s %4d (", name, constant);

  printValue(VECTOR_GET(ConstantVector, &chunk->constants, constant));

  printf(")\n");

  return offset + 2;
}

void disassembleChunk(Chunk *chunk) {
  printf("=== Chunk Start ===\n");

  uint32_t i = 0;
  while (i < VECTOR_COUNT(CodeVector, &chunk->code)) {
    i = disassembleInstruction(chunk, i);
  }

  printf("=== Chunk End ===\n");
}

int disassembleInstruction(Chunk *chunk, int offset) {
  printf("%04d ", offset);

  uint8_t instruction = VECTOR_GET(CodeVector, &chunk->code, offset);
  switch (instruction) {
    CASE_SIMPLE_OPCODE(OP_RETURN, offset);

    CASE_SIMPLE_OPCODE(OP_NONE, offset);
    CASE_SIMPLE_OPCODE(OP_TRUE, offset);
    CASE_SIMPLE_OPCODE(OP_FALSE, offset);

    CASE_SIMPLE_OPCODE(OP_ADD, offset);
    CASE_SIMPLE_OPCODE(OP_SUB, offset);
    CASE_SIMPLE_OPCODE(OP_MUL, offset);
    CASE_SIMPLE_OPCODE(OP_DIV, offset);

    CASE_SIMPLE_OPCODE(OP_PRINT, offset);

  case OP_LOAD_CONSTANT:
    return disassembleConstantInstruction("OP_LOAD_CONSTANT", chunk, offset);

  default:
    printf("Unknown opcode: %d\n", instruction);
    return offset + 1;
  }
}

void printToken(Token *token) {
  printf("#<Token %d '%*.*s'>\n", token->type, token->length, token->length,
         token->start);
}
