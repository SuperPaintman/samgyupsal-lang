#ifndef SAMGYUPSAL_TOKEN_H_
#define SAMGYUPSAL_TOKEN_H_

// Includes
#include <stdint.h>

// Types
typedef enum {
  TOKEN_NUMBER,

  TOKEN_IDENTIFIER,

  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_STAR,
  TOKEN_SLASH,

  TOKEN_PRINT,

  TOKEN_NEWLINE,

  TOKEN_EOF,

  TOKEN_ERROR = -1
} TokenType;

typedef struct {
  TokenType type;
  const char *start;
  uint32_t length;
} Token;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SAMGYUPSAL_TOKEN_H_
