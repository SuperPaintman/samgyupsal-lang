#ifndef SAMGYUPSAL_LEXER_H_
#define SAMGYUPSAL_LEXER_H_

// Includes
#include <stdbool.h>

#include "token.h"

// Macros
#define MAKE_LEXER(name, source)                                               \
  Lexer name;                                                                  \
  initLexer(&name, source)

// Types
typedef struct {
  const char *source;
  const char *current;
  const char *start;
} Lexer;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Declarations
void initLexer(Lexer *lexer, const char *source);
void freeLexer(Lexer *lexer);
Token lexToken(Lexer *lexer);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SAMGYUPSAL_LEXER_H_
