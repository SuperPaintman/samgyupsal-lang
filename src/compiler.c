#include "compiler.h"

#include <stdlib.h>

// TODO
#include "debug.h"
#include <stdio.h>

// Declarations
static void compileNumber(Compiler *compiler);
static void compilePrintStatement(Compiler *compiler);
static void maybeUnary(Compiler *compiler);
static void maybeBinary(Compiler *compiler);
static void maybeExpressionStatement(Compiler *compiler);
static void maybeStatement(Compiler *compiler);

VECTOR_TEMPLATE_IMPLEMENTATIONS(TokenVector, Token *);

static inline bool isAtEnd(Compiler *compiler) {
  return compiler->current && compiler->current->type == TOKEN_EOF;
}

static inline Token *next(Compiler *compiler) {
  if (isAtEnd(compiler)) {
    return compiler->current;
  }

  Token *token = malloc(sizeof(Token));
  *token = lexToken(&compiler->lexer);

  compiler->previous = compiler->current;
  compiler->current = token;

  VECTOR_ADD(TokenVector, &compiler->tokens, token);

  return token;
}

static inline bool checkTokenType(Compiler *compiler, TokenType type) {
  return compiler->current->type == type;
}

static inline bool matchTokenType(Compiler *compiler, TokenType type) {
  if (!checkTokenType(compiler, type)) {
    return false;
  }

  next(compiler);

  return true;
}

// Emit
static inline void emitCode(Compiler *compiler, OpCode code) {
  writeChunk(&compiler->chunk, code);
}

static void emitLoadConstant(Compiler *compiler, Value value) {
  int val = addConstant(&compiler->chunk, value);

  writeChunk(&compiler->chunk, OP_LOAD_CONSTANT);
  writeChunk(&compiler->chunk, val);
}

// Compile
static void compileNumber(Compiler *compiler) {
  Value value = strtod(compiler->previous->start, NULL);

  return emitLoadConstant(compiler, value);
}

static void compilePrintStatement(Compiler *compiler) {
  maybeExpressionStatement(compiler);
  emitCode(compiler, OP_PRINT);
}

// Maybe
static void maybeUnary(Compiler *compiler) {
  if (matchTokenType(compiler, TOKEN_NUMBER)) {
    return compileNumber(compiler);
  }

  printf("Unexpected Token:\n");
  printToken(compiler->current);
  exit(1);
}

static void maybeBinary(Compiler *compiler) {
  maybeUnary(compiler);

  if (matchTokenType(compiler, TOKEN_PLUS) ||
      matchTokenType(compiler, TOKEN_MINUS)) {
    TokenType opType = compiler->previous->type;

    maybeExpressionStatement(compiler);

    switch (opType) {
    case TOKEN_PLUS:
      return emitCode(compiler, OP_ADD);
    case TOKEN_MINUS:
      return emitCode(compiler, OP_SUB);
    default:
      // TODO: add error handler
      exit(1);
    }
  }
}

static void maybeExpressionStatement(Compiler *compiler) {
  return maybeBinary(compiler);
}

static void maybeStatement(Compiler *compiler) {
  if (matchTokenType(compiler, TOKEN_PRINT)) {
    return compilePrintStatement(compiler);
  }

  return maybeExpressionStatement(compiler);
}

void initCompiler(Compiler *compiler, const char *source) {
  MAKE_LEXER(lexer, source);
  MAKE_CHUNK(chunk);

  compiler->source = source;
  compiler->lexer = lexer;
  compiler->chunk = chunk;
  compiler->previous = NULL;
  compiler->current = NULL;
  VECTOR_INIT(TokenVector, &compiler->tokens);
}

void freeCompiler(Compiler *compiler) {
  VECTOR_FREE_FULL(TokenVector, &compiler->tokens);
  freeLexer(&compiler->lexer);
  freeChunk(&compiler->chunk);
  initCompiler(compiler, compiler->source);
}

Chunk compile(Compiler *compiler) {
  // Load the first token
  next(compiler);

  while (!matchTokenType(compiler, TOKEN_EOF)) {
    maybeStatement(compiler);
  }

  return compiler->chunk;
}
