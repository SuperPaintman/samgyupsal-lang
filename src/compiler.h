#ifndef SAMGYUPSAL_COMPILER_H_
#define SAMGYUPSAL_COMPILER_H_

// Includes
#include <stdint.h>

#include "chunk.h"
#include "lexer.h"
#include "vector.h"

// Macros
#define MAKE_COMPILER(name, source)                                            \
  Compiler name;                                                               \
  initCompiler(&name, source)

// Types
VECTOR_TEMPLATE_DECLARATIONS(TokenVector, Token *);

typedef struct {
  const char *source;
  Lexer lexer;
  Chunk chunk;
  Token *previous;
  Token *current;
  TokenVector tokens;
} Compiler;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Declarations
void initCompiler(Compiler *compiler, const char *source);
void freeCompiler(Compiler *compiler);
Chunk compile(Compiler *compiler);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SAMGYUPSAL_COMPILER_H_
