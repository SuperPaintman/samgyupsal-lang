#include "vm.h"

#include <stdio.h>
#include <stdlib.h>

#include "value.h"
#include "vector.h"

// Internal macros
#define INTERPRET_BINOP_TEMPLATE(name, op)                                     \
  static int interpretOp##name(VM *vm, Chunk *chunk, int offset) {             \
    Value b = pop(vm);                                                         \
    Value a = pop(vm);                                                         \
                                                                               \
    push(vm, a op b);                                                          \
                                                                               \
    return offset + 1;                                                         \
  }

static inline void push(VM *vm, Value value) { vm->stack[vm->sp++] = value; }

static inline Value pop(VM *vm) { return vm->stack[--vm->sp]; }

static int interpretLoadConstant(VM *vm, Chunk *chunk, int offset) {
  uint8_t constant = VECTOR_GET(CodeVector, &chunk->code, offset + 1);

  Value value = VECTOR_GET(ConstantVector, &chunk->constants, constant);

  push(vm, value);

  return offset + 2;
}

INTERPRET_BINOP_TEMPLATE(Add, +);
INTERPRET_BINOP_TEMPLATE(Sub, -);
INTERPRET_BINOP_TEMPLATE(Mul, *);
INTERPRET_BINOP_TEMPLATE(Div, /);

static int interpretOpPrint(VM *vm, Chunk *chunk, int offset) {
  /** @todo: In normal interpetator print must push `Null` on the stack */
  Value val = pop(vm);

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
