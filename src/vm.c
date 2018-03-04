#include "vm.h"

#include <stdio.h>
#include <stdlib.h>

#include "value.h"
#include "vector.h"

static int interpretLoadConstant(VM *vm, Chunk *chunk, int offset) {
  uint8_t constant = VECTOR_GET(CodeVector, &chunk->code, offset + 1);

  Value value = VECTOR_GET(ConstantVector, &chunk->constants, constant);

  vm->stack[vm->sp++] = value;

  return offset + 2;
}

static int interpretOpAdd(VM *vm, Chunk *chunk, int offset) {
  Value a = vm->stack[vm->sp - 2];
  Value b = vm->stack[vm->sp - 1];

  vm->sp -= 2;

  vm->stack[vm->sp++] = a + b;

  return offset + 1;
}

static int interpretOpSub(VM *vm, Chunk *chunk, int offset) {
  Value a = vm->stack[vm->sp - 2];
  Value b = vm->stack[vm->sp - 1];

  vm->sp -= 2;

  vm->stack[vm->sp++] = a - b;

  return offset + 1;
}

static int interpretOpPrint(VM *vm, Chunk *chunk, int offset) {
  /** @todo: In normal interpetator print must push `Null` on the stack */
  vm->sp--;

  Value val = vm->stack[vm->sp];

  printf("%g\n", val);

  return offset + 1;
}

void initVM(VM *vm) { vm->sp = 0; }

void freeVM(VM *vm) { initVM(vm); }

void interpret(VM *vm, Chunk *chunk) {
  uint32_t i = 0;
  while (i < VECTOR_COUNT(CodeVector, &chunk->code)) {
    i = interpretInstruction(vm, chunk, i);
  }
}

int interpretInstruction(VM *vm, Chunk *chunk, int offset) {
  uint8_t instruction = VECTOR_GET(CodeVector, &chunk->code, offset);
  switch (instruction) {
  case OP_LOAD_CONSTANT:
    return interpretLoadConstant(vm, chunk, offset);

  case OP_ADD:
    return interpretOpAdd(vm, chunk, offset);
  case OP_SUB:
    return interpretOpSub(vm, chunk, offset);

  case OP_PRINT:
    return interpretOpPrint(vm, chunk, offset);

  default:
    printf("Unknown opcode: %d\n", instruction);

    exit(1);

    return offset + 1;
  }
}
