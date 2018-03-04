#include <stdio.h>

#include "compiler.h"
#include "debug.h"
#include "util.h"
#include "vm.h"

int main(int argc, char **argv) {
  const char *binname = argv[0];

  if (argc != 2) {
    printf("Usage:\n"
           "  %s <filename>\n",
           binname);

    return 1;
  }

  char *filename = argv[1];
  char *source = readFile(filename);

  if (source == NULL) {
    printf("File not found\n");
    return 1;
  }

  printf("=== Source ===\n");
  printf("%s\n", source);
  printf("==============\n");

  MAKE_VM(vm);
  MAKE_COMPILER(compiler, source);

  Chunk chunk = compile(&compiler);

  disassembleChunk(&chunk);

  printf("=== VM Results ===\n");
  interpret(&vm, &chunk);
  printf("==================\n");

  freeCompiler(&compiler);
  freeVM(&vm);

  freeFile(source);

  return 0;
}
