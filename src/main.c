#include <stdio.h>

#include "compiler.h"
#include "debug.h"
#include "vm.h"

int main() {
  const char *source = "print 1000 + 250 + 90 - 3";

  MAKE_VM(vm);
  MAKE_COMPILER(compiler, source);

  Chunk chunk = compile(&compiler);

  interpret(&vm, &chunk);
  disassembleChunk(&chunk);

  freeCompiler(&compiler);
  freeVM(&vm);

  return 0;
}
