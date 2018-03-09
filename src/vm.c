#include "vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "value.h"
#include "vector.h"

// Internal macros
#define INTERPRET_BINOP_TEMPLATE(name, op)                                     \
  static int interpretOp##name(VM *vm, Chunk *chunk, int offset) {             \
    Value *b = pop(vm);                                                        \
    Value *a = pop(vm);                                                        \
                                                                               \
    if (!IS_NUMBER(*a) || !IS_NUMBER(*b)) {                                    \
      printf("Operands must be numbers\n");                                    \
                                                                               \
      exit(1);                                                                 \
    }                                                                          \
    push(vm, MAKE_NUMBER(AS_NUMBER(*a) op AS_NUMBER(*b)));                     \
                                                                               \
    return offset + 1;                                                         \
  }

static inline void push(VM *vm, Value value) { vm->stack[vm->sp++] = value; }

static inline Value *pop(VM *vm) { return &vm->stack[--vm->sp]; }

static inline int concatenateStrings(VM *vm, Chunk *chunk, Value *strA,
                                     Value *strB, int offset) {
  ObjectString *a = AS_STRING(*strA);
  ObjectString *b = AS_STRING(*strB);

  uint32_t length = a->length + b->length;
  char *buf = malloc(sizeof(char) * (length + 1));

  memcpy(buf, a->chars, a->length);
  memcpy(buf + a->length, b->chars, b->length);
  buf[length] = '\0';

  Value value = MAKE_OBJECT(makeString(buf, length));

  Object *next = chunk->objects;

  chunk->objects = AS_OBJECT(value);
  chunk->objects->next = next;

  push(vm, value);

  return offset + 1;
}

static int interpretLoadConstant(VM *vm, Chunk *chunk, int offset) {
  uint8_t constant = VECTOR_GET(CodeVector, &chunk->code, offset + 1);

  Value value = VECTOR_GET(ConstantVector, &chunk->constants, constant);

  push(vm, value);

  return offset + 2;
}

static int interpretOpAdd(VM *vm, Chunk *chunk, int offset) {
  Value *b = pop(vm);
  Value *a = pop(vm);

  if (IS_STRING(*a) && IS_STRING(*b)) {
    return concatenateStrings(vm, chunk, a, b, offset);
  }

  if (IS_NUMBER(*a) && IS_NUMBER(*b)) {
    push(vm, MAKE_NUMBER(AS_NUMBER(*a) + AS_NUMBER(*b)));

    return offset + 1;
  }

  printf("Operands must be numbers\n");
  exit(1);
}

INTERPRET_BINOP_TEMPLATE(Sub, -);
INTERPRET_BINOP_TEMPLATE(Mul, *);
INTERPRET_BINOP_TEMPLATE(Div, /);

static int interpretOpPrint(VM *vm, Chunk *chunk, int offset) {
  /** @todo: In normal interpetator print must push `Null` on the stack */
  Value *val = pop(vm);

  switch (val->type) {
  case VALUE_NONE:
    printf("None");
    break;
  case VALUE_BOOLEAN:
    printf(AS_BOOLEAN(*val) ? "True" : "False");
    break;
  case VALUE_NUMBER:
    printf("%g", AS_NUMBER(*val));
    break;
  case VALUE_OBJECT:
    if (!IS_STRING(*val)) {
      printf("UnexpectedObject");
      break;
    }

    printf("%s", AS_CSTRING(*val));
    break;
  default:
    // Unreachable
    printf("???");
  }

  printf("\n");

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
  case OP_NONE:
    push(vm, MAKE_NONE());
    return offset + 1;
  case OP_TRUE:
    push(vm, MAKE_BOOLEAN(true));
    return offset + 1;
  case OP_FALSE:
    push(vm, MAKE_BOOLEAN(false));
    return offset + 1;

  case OP_LOAD_CONSTANT:
    return interpretLoadConstant(vm, chunk, offset);

  case OP_ADD:
    return interpretOpAdd(vm, chunk, offset);
  case OP_SUB:
    return interpretOpSub(vm, chunk, offset);
  case OP_MUL:
    return interpretOpMul(vm, chunk, offset);
  case OP_DIV:
    return interpretOpDiv(vm, chunk, offset);

  case OP_PRINT:
    return interpretOpPrint(vm, chunk, offset);

  default:
    printf("Unknown opcode: %d\n", instruction);

    exit(1);

    return offset + 1;
  }
}
