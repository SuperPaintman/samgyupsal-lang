#include "lexer.h"

#include <ctype.h>
#include <string.h>

typedef struct {
  const char *name;
  size_t length;
  TokenType type;
} Keyword;

static Keyword keywords[] = {{"print", 5, TOKEN_PRINT}};

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

    if (c == ' ' || c == '\t') {
      next(lexer, 1);
    } else {
      break;
    }
  } while (true);
}

static inline Token scanIdentifierToken(Lexer *lexer) {
  while (isalpha(peek(lexer, 1))) {
    next(lexer, 1);
  }

  size_t length = lexer->current - lexer->start;
  for (uint32_t i = 0, ii = sizeof(keywords) / sizeof(Keyword); i < ii; i++) {
    Keyword *keyword = &keywords[i];

    if (keyword->length != length) {
      continue;
    }

    if (memcmp(lexer->start, keyword->name, length) != 0) {
      continue;
    }

    return makeToken(lexer, keyword->type);
  }

  return makeToken(lexer, TOKEN_IDENTIFIER);
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

  if (isalpha(c)) {
    return scanIdentifierToken(lexer);
  }

  if (isdigit(c)) {
    return scanNumberToken(lexer);
  }

  switch (c) {
  case '\n':
    return makeToken(lexer, TOKEN_NEWLINE);

  case '+':
    return makeToken(lexer, TOKEN_PLUS);
  case '-':
    return makeToken(lexer, TOKEN_MINUS);
  case '*':
    return makeToken(lexer, TOKEN_STAR);
  case '/':
    return makeToken(lexer, TOKEN_SLASH);

  default:
    return makeError("Unexpected Token");
  }
}
