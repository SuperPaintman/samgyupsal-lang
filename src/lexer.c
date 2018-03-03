#include "lexer.h"

#include <ctype.h>
#include <string.h>

static inline bool isAtEnd(Lexer *lexer) { return *lexer->current == '\0'; }

static inline char peek(Lexer *lexer, int offset) {
  if (isAtEnd(lexer)) {
    return '\0';
  }

  return lexer->current[offset - 1];
}

static inline char next(Lexer *lexer, int offset) {
  if (isAtEnd(lexer)) {
    return '\0';
  }

  char c = lexer->current[offset - 1];
  lexer->current += offset;

  return c;
}

static Token makeToken(Lexer *lexer, TokenType type) {
  return (Token){.type = type,
                 .start = lexer->start,
                 .length = lexer->current - lexer->start};
}

static inline Token makeError(const char *message) {
  return (Token){
      .type = TOKEN_ERROR, .start = message, .length = strlen(message)};
}

static inline void skipWhitespaces(Lexer *lexer) {
  do {
    if (isAtEnd(lexer)) {
      break;
    }

    char c = peek(lexer, 1);

    if (c == ' ' || c == '\t' || c == '\n') {
      next(lexer, 1);
    } else {
      break;
    }
  } while (true);
}

static inline Token scanNumberToken(Lexer *lexer) {
  while (isdigit(peek(lexer, 1))) {
    next(lexer, 1);
  }

  return makeToken(lexer, TOKEN_NUMBER);
}

void initLexer(Lexer *lexer, const char *source) {
  lexer->source = source;
  lexer->current = source;
  lexer->start = source;
}

void freeLexer(Lexer *lexer) { initLexer(lexer, lexer->source); }

Token lexToken(Lexer *lexer) {
  skipWhitespaces(lexer);

  lexer->start = lexer->current;

  if (isAtEnd(lexer)) {
    return makeToken(lexer, TOKEN_EOF);
  }

  char c = next(lexer, 1);

  if (isdigit(c)) {
    return scanNumberToken(lexer);
  }

  switch (c) {
  case '+':
    return makeToken(lexer, TOKEN_PLUS);
  case '-':
    return makeToken(lexer, TOKEN_MINUS);

  default:
    return makeError("Unexpected Token");
  }
}
